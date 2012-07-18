/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

/* $Id: shhopt.h,v 1.4 2000/09/20 14:41:56 widmann Exp $ */
#ifndef SHHOPT_H
#define SHHOPT_H

/* constants for recognized option types. */
typedef enum {
    OPT_END,               /* nothing. used as ending element. */
    OPT_FLAG,              /* no argument following. sets variable to 1. */
    OPT_STRING,            /* string argument. */
    OPT_INT,               /* signed integer argument. */
    OPT_UINT,              /* unsigned integer argument. */
    OPT_LONG,              /* signed long integer argument. */
    OPT_ULONG             /* unsigned long integer argument. */
} optArgType;

/* flags modifying the default way options are handeled. */
#define OPT_CALLFUNC  1    /* pass argument to a function. */

typedef struct {
    char       shortName;  /* Short option name. */
    char       *longName;  /* Long option name, no including '--'. */
    optArgType type;       /* Option type. */
    void       *arg;       /* Pointer to variable to fill with argument,
                            * or pointer to function if Type == OPT_FUNC. */
    int        flags;      /* Modifier flags. */
} optStruct;


void optSetFatalFunc(void (*f)(const char *, ...));
void optParseOptions(int *argc, char *argv[],
		     optStruct opt[], int allowNegNum);

#endif
