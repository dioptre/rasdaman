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
/
/**
 * SOURCE: gen_pattern.cc
 *
 * MODULE: rasodmg
 *
 * PURPOSE: generate patterns for r_StatTiling.
 *
 * COMMENTS:
 *		None
*/

/* 
   ATENTION: The format of the input file for using with this program is:

     number_of_patterns minterval_domain
     d_dim1 d_dim2 ... d_dimn
     percentage minterval_pattern
     ...

   Example:
   
     1000 [1:800,1:600]
     10 10
     0.40 [10:600,30:300]
     0.60 [70:500,400:500]

     Creates a file with 1000 patterns on a 800x600 domain.
     The pixels are generated having borders with a maximum variation of
     10 pixels from the specified interest zones. And 2 interest zones
     are specified, one for 40% of the patterns, another for 60%.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream.h>
#include <time.h>
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "raslib/dlist.hh"

const int BUF_SIZE = 200;

char* in_filename;
char* out_filename;
int total_patterns;
int dim;
int* delta;
r_Minterval* domain;

struct IArea
{
    r_Minterval iarea;
    double percent;

    IArea(r_Minterval& area, double percentage)
      : iarea(area), percent(percentage)
    {
    }
};

DList<IArea*> IAreas;

void parse(int argc, char** argv)
{
  if ((argc == 1) || ((argc == 2) && (strcmp(argv[1], "-h") == 0)))
  {
    cout << "Usage: " << argv[0] << " [input_filename] [output_filename]"
         << endl;
    exit(0);
  }
  
  if (argc != 3)
  {
    cout << "Usage: " << argv[0] << " [input_filename] [output_filename]"
         << endl;
    exit(0);
  }

  in_filename = argv[1];
  out_filename = argv[2];
}

void get_specification()
{
  char buf[BUF_SIZE+1], buf2[BUF_SIZE+1];
  double perc;

  ifstream is(in_filename);
  if (!is)
  {
    cout << "Could not open " << in_filename << "." << endl;
    exit(0);
  }

  cout << "Reading parameters... " << endl;

  is >> total_patterns;
  is.getline(buf, BUF_SIZE);
  domain = new r_Minterval(buf);
  dim = domain->dimension();

  if (total_patterns <= 0)
  {
    cout << "Invalid number of patterns: " << total_patterns << endl;
    exit(0);
  }

  if (dim <= 0)
  {
    cout << "Invalid domain: " << *domain << endl;
    delete domain;
    exit(0);
  }

  cout << endl << "Patterns: " << total_patterns << endl;
  cout << "Domain: " << *domain << endl;
  cout << "Dimension: " << dim << "  (";

  delta = new int[dim];

  for (int i=0; i<dim; i++)
  {
    is >> delta[i];
    cout << "*";
  }

  cout << ")" << endl;
  cout << "Reading patterns... (";
  
  while (!is.eof())
  {
    is >> perc;
    is.getline(buf, BUF_SIZE);
    if (!is.bad() && sscanf(buf, "%s", buf2) == 1)
    {
      r_Minterval area(buf2);
      IArea* ia = new IArea(area, perc);
      IAreas += ia;

      cout << "*";
    }
    else
      break;
  }

  cout << ") -Done- " << endl;
  
  is.close();
}

void generate_patterns()
{
  srand((unsigned int) time(NULL));


  DListIterator<IArea*> it = IAreas.create_iterator();

  ofstream os(out_filename);
  if (!os)
  {
    cout << "Could not open " << out_filename << "." << endl;
    exit(0);
  }
  
  while (it.not_done())
  {
    IArea* ia = *it;
    ++it;

    int total = (int)(ia->percent * total_patterns);

    for (int i=0; i<total; i++)
    {
      os << "[";
      
      for (int j=0; j<dim; j++)
      {
        long xmin = ia->iarea[j].low();
        long xmax = ia->iarea[j].high();
        long total = xmax-xmin;

        if (delta[j] > total)
	  delta[j] = total;
         
        long dxmin = (rand() % (2*delta[j] + 1)) - delta[j];
        long dxmax = (rand() % (2*delta[j] + 1)) - delta[j];

        xmin = xmin + dxmin;
        if (xmin < (*domain)[j].low())
          xmin = (*domain)[j].low() + ((*domain)[j].low() - xmin);
            
        xmax = xmax + dxmax;
	if (xmax > (*domain)[j].high())
          xmax = (*domain)[j].high() - (xmax - (*domain)[j].high());

        if (xmin > xmax)
        {
          long temp = xmin;
          xmin = xmax;
          xmax = temp;
        }

        os << xmin << ":" << xmax;

        if (j != dim-1)
          os << ",";
      }
      
      os << "]" << endl;
    }
  }

  delete [] delta;
  delete domain;
}

int main(int argc, char** argv)
{
  parse(argc, argv);
  get_specification();
  generate_patterns();
  
  return 0;
}
