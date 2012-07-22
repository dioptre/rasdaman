/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
*/

#include "mymalloc/mymalloc.h"
/*------------------------------------------------------------------------*/
/*  childs.c - child process handling.                                    */
/*                                                                        */
/*------------------------------------------------------------------------*/
/*  Comments:                                                             */
/*      - intended purpose:                                               */
/*          Keep all functions related to child processes in one place.   */
/*      - Status:                                                         */
/*          - Functions to Create and Initialize a child is thrown in.    */
/*          - Should think over the actual design - looks somewhat        */
/*            messy now. It should be something more clean and of         */
/*            generic use, so it can be reused in future projects.        */
/*          - Nevertheless -- it works, and there should be no creeping   */
/*            surprises in the code... [FLW Alert... ;-)]                 */
/*------------------------------------------------------------------------*/
/*
 * RCS:
 *   $RCSfile: childs.c,v $ $Revision: 1.6 $ $State: Exp $
 *   $Locker:  $
 */


#include   "defs.h"
#include   "protos.h"
#include   "types.h"
#include   "server.h"
#include   "http-defs.h"
#include   "http.h"

extern struct ServerBase Server;


/****** childs/InitChild *****************************************************
*
*   NAME
*       InitChild -- data initializition after forking the child process.
*
*   SYNOPSIS
*       int InitChild( struct ClientBase *Client );
*
*   FUNCTION
*
*   RESULT
*
*   NOTES
*
*   BUGS
*
*   SEE ALSO
*
******************************************************************************
*
*/

rc_t InitChild( struct ClientBase *Client )
{
    char *UnknownIP = "0.0.0.0";

    /* Init ClientBase  */
    strcpy( Client->Host.IPAddrString, inet_ntoa( Client->Socket.sin_addr ) );
    if( Client->Host.IPAddrString == NULL )
        strcpy( Client->Host.IPAddrString, UnknownIP );
    Client->Host.IPAddress = inet_addr( Client->Host.IPAddrString );

    Client->Comm.ConnStatus      = CONN_UNDEFINED;
    InitHTTPMsg( &Client->Response );
    InitReqInfo( &Client->Request );

    return( OK );
}



/****** childs/NewChild ******************************************************
*
*   NAME
*
*
*   SYNOPSIS
*
*
*   FUNCTION
*
*
*   INPUTS
*
*
*   RESULT
*
*
*   NOTES
*      ###  Temporary fork() wrapper.
*           Will be later expanded into something more usefull...
*
*   BUGS
*
*
*   SEE ALSO
*
*
******************************************************************************
*
*/

pid_t NewChild( struct ChildBase *List, struct FDsets *PDSets, struct ClientBase *Client )
{
    struct ChildBase *Child;

    Child = (struct ChildBase*)mymalloc( sizeof( struct ChildBase ) );
    if( Child == NULL )  // Failure? => QUIT.
        ErrorMsg( E_SYS, FAIL, "FAIL:  NewChild(): malloc() failed!" );

    // Open pipe
    if( pipe( Child->PD ) < 0 )
    {
        ErrorMsg( E_SYS, ERROR, "ERROR: NewChild(): pipe() failed." );
        Child->PipeStatus = PIPE_CLOSED;
    }
    else
    {
        Child->PipeStatus = PIPE_OPEN;
    }

    Child->PId = fork();

    if( Child->PId < 0 )  // fork() Error? => QUIT.
        ErrorMsg( E_SYS, FAIL, "FAIL:  NewChild(): fork() failed!" );

    if( Child->PId > 0 )  // parent process?
    {
        // Add Pipe-Descriptors to PipeSets
        if( Child->PipeStatus == PIPE_OPEN )
        {
            FD_SET( Child->PD[0], &PDSets->Read );
            if( Child->PD[0] > PDSets->MaxFD )
                PDSets->MaxFD = Child->PD[0];
            close( Child->PD[1] );
        }
        // Add Child to List
        AddChild( List, Child );
    }
    else  // child process
    {
        InitChild( Client );
        // Remember PD to parent
        Client->Pipe = Child->PD[1];
        close( Child->PD[0] );
        free( Child );
    }

    return( Child->PId );
}


void CleanupChild( struct ChildBase *List, struct FDsets *PDSets, pid_t PId )
{
    struct ChildBase *Ptr;
    struct ChildBase *Tmp;

    LogMsg( LG_SERVER, DEBUG, "DEBUG: Begin CleanupChild ..." );

    if(PDSets == NULL)
        LogMsg( LG_SERVER, DEBUG, "DEBUG: ########### PDSets is NULL!" );
    if(PId == (pid_t)0)
        LogMsg( LG_SERVER, DEBUG, "DEBUG: ########### ChildPid is NULL!" );
    if(List == NULL)
        LogMsg( LG_SERVER, DEBUG, "DEBUG: ########### ChildBase is NULL!" );

    if( List->PId > 0 )
    {
        for( Ptr = List; Ptr->next != List; Ptr = Ptr->next )
        {
            if( Ptr->next->PId == PId )
            {
                Tmp = Ptr->next;
                break;
            }
        }
        // Close pipe
        close( Tmp->PD[0] );
        // Remove Pipe-Descriptors from PipeSets
        FD_CLR( Tmp->PD[0], &PDSets->Read );
        RemChild( List, Tmp );
    }
}


void AddChild( struct ChildBase *List, struct ChildBase *Child )
{
    Child->next = List->next;
    Child->prev = List;
    List->next->prev = Child;
    List->next  = Child;
    List->PId++;
}


void RemChild( struct ChildBase *List, struct ChildBase *Child )
{
    Child->prev->next = Child->next;
    Child->next->prev = Child->prev;
    List->PId--;
    free( Child );
}


struct ChildBase *GetChild( struct ChildBase *List, pid_t PId )
{
    struct ChildBase *Ptr;

    if( List->PId > 0 )
    {
        for( Ptr = List; Ptr->next != List; Ptr = Ptr->next )
        {
            if( Ptr->next->PId == PId )
                return( Ptr->next );
        }
        return( NULL );
    }
    else
        return( NULL );
}
