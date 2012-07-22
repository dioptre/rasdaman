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

/*************************************************************
 *
 * INCLUDE: test_params.cc
 *
 * MODULE:  raslib
 *
 * PURPOSE:
 *
 * COMMENTS:
 *
 ************************************************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"


RMINITGLOBALS('C')

using namespace std;


typedef struct test_params_s
{
    r_Parse_Params *pp;
    int zlevel;
    double fidelity;
    char *lstream;
} test_params_t;


static void test_params(test_params_t &params, char *str)
{
    cout << "Processing (" << str << ") ..." << endl;

    params.pp->process(str);

    cout << "zlevel = " << params.zlevel
         << ", fidelity = " << params.fidelity
         << ", stream = <" << params.lstream << ">" << endl;

    // string was allocated with strdup, free now
    free(str);
}


int main(int argc, char *argv[])
{
    r_Parse_Params pp;
    test_params_t params = {&pp, -1, -1.0, NULL};

    pp.add("zlevel", &params.zlevel, r_Parse_Params::param_type_int);
    pp.add("fidelity", &params.fidelity, r_Parse_Params::param_type_double);
    pp.add("stream", &params.lstream, r_Parse_Params::param_type_string);

    // Note: must use strdup() because Purify doesn't find reads over the end of
    // static strings!
    test_params(params, strdup("0123"));
    test_params(params, strdup("zlevel, fidelity=0.1"));
    test_params(params, strdup("zlevel=a"));
    test_params(params, strdup("zlevel=6"));
    test_params(params, strdup("\"zlevel=6\""));
    test_params(params, strdup("fidelity=0.90"));
    test_params(params, strdup("stream=rle"));
    test_params(params, strdup("stream=eee "));
    test_params(params, strdup("stream=\"rle\""));
    test_params(params, strdup("zlevel=9, fidelity=0.75, stream=zlib"));
    test_params(params, strdup("stream=rle, zlevel=5"));
    test_params(params, strdup("zlevel=1, foobar=hello"));
    test_params(params, strdup("foobar=hello, zlevel=2"));
    test_params(params, strdup("  stream=\"zlib\"  "));
    test_params(params, strdup("  foobar=\"hello, you there\", stream=\"rle\""));

    if (params.lstream != NULL)
        delete [] params.lstream;

    return 0;
}
