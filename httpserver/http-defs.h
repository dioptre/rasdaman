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
/*  http-defs.h - defines for HTTP.                                       */
/*------------------------------------------------------------------------*/


#ifndef _HTTP_DEFS_H
#define _HTTP_DEFS_H

#define    AUTH_MAX_RETRIES             1

/*
 */

#define    HTTP_REQUEST                 1
#define    HTTP_RESPONSE                2


/* ReqInfo/RespInfo structure states when reading/parsing requests/responses
 */

#define    RI_EMPTY                     0
#define    RI_READ_OK                   1
#define    RI_READ_ERROR                2
#define    RI_PARSE_OK                  3
#define    RI_PARSE_WARN                4   /* trouble when parsing MsgHeaders    */
#define    RI_PARSE_ERROR               5   /* Request Line/Status Line not found */

/* Known Protocols:
 */

#define    HTTP_0_9                     0
#define    HTTP_0_9_STRING              ""
#define    HTTP_1_0                     1
#define    HTTP_1_0_STRING              "HTTP/1.0"
#define    HTTP_1_1                     2
#define    HTTP_1_1_STRING              "HTTP/1.1"


/* HTTP Method Keys:
 */

#define    MKEY_NONE                    0
#define    MKEY_GET                     1
#define    MKEY_HEAD                    2
#define    MKEY_PUT                     3
#define    MKEY_POST                    4
#define    MKEY_DELETE                  5
#define    MKEY_OPTIONS                 6
#define    MKEY_TRACE                   7
#define    MKEY_UNKNOWN                 999

#define    NUM_MKEYS                    7


/* HTTP Status Codes:
 */

#define    STATUS_UNKNOWN                        0
#define    STATUS_UNDEFINED                      0
#define    STATUS_Continue                       100
#define    STATUS_Switching_Protocols            101
#define    STATUS_OK                             200
#define    STATUS_Created                        201
#define    STATUS_Accepted                       202
#define    STATUS_Non_Authoritative_Information  203
#define    STATUS_No_Content                     204
#define    STATUS_Reset_Content                  205
#define    STATUS_Partial_Content                206
#define    STATUS_Multiple_Choices               300
#define    STATUS_Moved_Permanently              301
#define    STATUS_Moved_Temporarily              302
#define    STATUS_See_Other                      303
#define    STATUS_Not_Modified                   304
#define    STATUS_Use_Proxy                      305
#define    STATUS_Switch_Proxy                   306
#define    STATUS_Bad_Request                    400
#define    STATUS_Unauthorized                   401
#define    STATUS_Payment_Required               402
#define    STATUS_Forbidden                      403
#define    STATUS_Not_Found                      404
#define    STATUS_Method_Not_Allowed             405
#define    STATUS_Not_Acceptable                 406
#define    STATUS_Proxy_Authentication_Required  407
#define    STATUS_Request_Timeout                408
#define    STATUS_Conflict                       409
#define    STATUS_Gone                           410
#define    STATUS_Length_Required                411
#define    STATUS_Precondition_Failed            412
#define    STATUS_Request_Entity_Too_Large       413
#define    STATUS_Request_URI_Too_Long           414
#define    STATUS_Unsupported_Media_Type         415
#define    STATUS_Requested_Range_Not_Valid      416
#define    STATUS_Expectation_Failed             419
#define    STATUS_Internal_Server_Error          500
#define    STATUS_Not_Implemented                501
#define    STATUS_Bad_Gateway                    502
#define    STATUS_Service_Unavailable            503
#define    STATUS_Gateway_Timeout                504
#define    STATUS_HTTP_Version_Not_Supported     505
#define    STATUS_Redirection_Failed             506


/* HTTP Header Keys:
 */

#define    HKEY_CONTINUE                0
#define    HKEY_Accept                  1
#define    HKEY_Accept_Charset          2
#define    HKEY_Accept_Encoding         3
#define    HKEY_Accept_Language         4
#define    HKEY_Accept_Ranges           5
#define    HKEY_Age                     6
#define    HKEY_Allow                   7
#define    HKEY_Authorization           8
#define    HKEY_Cache_Control           9
#define    HKEY_Compliance              10
#define    HKEY_Connection              11
#define    HKEY_Content_Base            12
#define    HKEY_Content_Encoding        13
#define    HKEY_Content_Language        14
#define    HKEY_Content_Length          15
#define    HKEY_Content_Location        16
#define    HKEY_Content_MD5             17
#define    HKEY_Content_Range           18
#define    HKEY_Content_Type            19
#define    HKEY_Date                    20
#define    HKEY_ETag                    21
#define    HKEY_Expect                  22
#define    HKEY_Expires                 23
#define    HKEY_From                    24
#define    HKEY_Host                    25
#define    HKEY_If_Modified_Since       26
#define    HKEY_If_Match                27
#define    HKEY_If_None_Match           28
#define    HKEY_If_Range                29
#define    HKEY_If_Unmodified_Since     30
#define    HKEY_Keep_Alive              31
#define    HKEY_Last_Modified           32
#define    HKEY_Location                33
#define    HKEY_Max_Forwards            34
#define    HKEY_Non_Compliance          35
#define    HKEY_Pragma                  36
#define    HKEY_Proxy_Authenticate      37
#define    HKEY_Proxy_Authorization     38
#define    HKEY_Public                  39
#define    HKEY_Range                   40
#define    HKEY_Referer                 41
#define    HKEY_Retry_After             42
#define    HKEY_Server                  43
#define    HKEY_Set_Proxy               44
#define    HKEY_Transfer_Encoding       45
#define    HKEY_Upgrade                 46
#define    HKEY_User_Agent              47
#define    HKEY_Vary                    48
#define    HKEY_Warning                 49
#define    HKEY_WWW_Authenticate        50
#define    HKEY_UNKNOWN                 999

#define    NUM_HKEYS                    50


/* Language Keys:
 */

    /*  These numbers are intentionally choosen:  */
    /*    Space left for upgrades.                */
    /*  See also:  ISP 639 / Version 1            */

#define    LANG_UNKNOWN                 0
#define    LANG_DE                      22
#define    LANG_EN                      25
#define    LANG_EO                      26
#define    LANG_ES                      27
#define    LANG_FR                      34
#define    LANG_IT                      53


/* Security Realm Keys:
 */

#define    REALM_ERROR                 -1
#define    REALM_UNKNOWN                0
#define    REALM_UNDEFINED              0
#define    REALM_IPCLASS_A              1
#define    REALM_IPCLASS_B              2
#define    REALM_IPCLASS_C              3
#define    REALM_IPADDRESS              4
#define    REALM_HOSTNAME               10
#define    REALM_DOMAIN                 11


#endif  /*  _HTTP_DEFS_H not defined  */
