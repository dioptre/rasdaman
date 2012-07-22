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
/*------------------------------------------------------------------------*/
/*  http.h - typedefs and structures for HTTP.                            */
/*------------------------------------------------------------------------*/
/*
 * RCS:
 *   $RCSfile: http.h,v $ $Revision: 1.3 $ $State: Exp $
 *   $Locker:  $
 */

#ifndef _HTTP_H
#define _HTTP_H


struct KeywordKey
{
    char *Keyword;
    int   Key;
};

struct HTTPError
{
    int   Code;
    char *Reason;
    char *Message_en;
    char *Message_de;
};

/*  HTTP Message structure
 */

struct HTTPMsg
{
    char *Head;
    char *Body;
    int   BodySize;
};


struct URLComps
{
    char *Protocol;
    char *Servername;
    char *Path;
    char *Extra;
};


struct Version
{
    int   Major;
    int   Minor;
};


struct RequestLine
{
    char *Vanilla;
    int   Method;
    struct URLComps URL;
    struct Version  Version;
};

struct StatusLine
{
    char *Vanilla;
    struct Version  Version;
    int   Status;
    char *Reason;
};

struct MsgHeader
{
    struct MsgHeader *Next;
    int   Field;
    char *Content;
};


/*  Data Structure for HTTP Request
 */

struct ReqInfo
{
    int                State;
    char              *HeadBuff;
    size_t             HeadSize;
    char              *BodyBuff;
    size_t             BodySize;
    struct RequestLine Line;
    struct MsgHeader  *First;
    struct MsgHeader  *Last;
    char              *Body;
};

/*  Data Structure for HTTP Response
 */

struct RespInfo
{
    int                State;
    char              *HeadBuff;
    size_t             HeadSize;
    char              *BodyBuff;
    size_t             BodySize;
    struct StatusLine  Line;
    struct MsgHeader  *First;
    struct MsgHeader  *Last;
    char              *Body;
};

#endif  /*  _HTTP_H not defined  */
