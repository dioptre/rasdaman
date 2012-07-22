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

#include "clientcomm/clnt_control/clnt_control.h"
#include "clientcomm/rpcif.h"

#include <stdio.h>

void client_control(CLIENT *cl, unsigned int rq, char *in)
{
    clnt_control(cl, rq, in);
}

void client_destroy(CLIENT *cl)
{
    clnt_destroy(cl);
}

CLIENT* client_create(char *host, u_long prog, u_long vers, char *prot)
{
    return clnt_create(host, prog, vers, prot);
}

extern void xdr_free(xdrproc_t proc, char *objp );

void xdrfree( ntxdr_Type xdrType, char *objp )
{
    switch( xdrType )
    {
    case ntxdr_ExecuteQueryRes:
        xdr_free( xdr_ExecuteQueryRes, objp );
        break;
    case ntxdr_ExecuteUpdateRes:
        xdr_free( xdr_ExecuteUpdateRes, objp );
        break;
    case ntxdr_GetMDDRes:
        xdr_free( xdr_GetMDDRes, objp );
        break;
    case ntxdr_GetTileRes:
        xdr_free( xdr_GetTileRes, objp );
        break;
    case ntxdr_GetCollRes:
        xdr_free( xdr_GetCollRes, objp );
        break;
    case ntxdr_GetCollOIdsRes:
        xdr_free( xdr_GetCollOIdsRes, objp );
        break;
    case ntxdr_OIdRes:
        xdr_free( xdr_OIdRes, objp );
        break;
    case ntxdr_ObjectTypeRes:
        xdr_free( xdr_ObjectTypeRes, objp );
        break;
    case ntxdr_GetTypeStructureRes:
        xdr_free( xdr_GetTypeStructureRes, objp );
        break;
    case ntxdr_GetElementRes:
        xdr_free( xdr_GetElementRes, objp );
        break;
    case ntxdr_ServerStatRes:
        xdr_free( xdr_ServerStatRes, objp );
        break;
    default:
        printf( "Error: xdr_free of a unknown type." );
    }
}



// Looks like ntohl and htonl are missing in all libs on our NT versions...
u_long PASCAL FAR ntohl(u_long x)
{
    return (u_long)((x & 0xff) << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | ((x >> 24) & 0xff);
}

u_long PASCAL FAR htonl(u_long x)
{
    // swapping the endianness works either way...
    return ntohl(x);
}
