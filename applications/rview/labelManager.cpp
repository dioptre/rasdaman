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
 *  PURPOSE:
 *
 *  The label manager class is used to lookup label:value pairs
 *  stored in a text file. The constructor reads in the file whose
 *  name is passed to it and sorts the non-empty, non-commentary
 *  lines (commentary lines have a '#' as the first non-white
 *  character in a line) in ascending order to make binary searching
 *  possible. The binary search is performed by the lookup member
 *  function.
 *
 *  COMMENTS:   None
 */




#include <stdio.h>
#include <string.h>
#include "iostream.h"



#include "labelManager.hh"





/*
 *  Load the resource file, parse and sort it.
 */

labelManager::labelManager(const char *resourceFile)
{
    FILE *fp;
    size_t filesize;
    int i;
    char c, *b, *upper;

    buffer = NULL;
    lineTable = NULL;
    lines = 0;
    strncpy(badSymbol, "???", 4);

    if ((fp = fopen(resourceFile, "r")) == NULL)
    {
        cerr << "Unable to open resource file " << resourceFile << endl;
        return;
    }

    // Position to end of file to determine its length.
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);

    if ((buffer = new char[filesize+1]) == NULL)
    {
        cerr << "Not enough memory for buffer!" << endl;
    }

    fseek(fp, 0, SEEK_SET);
    // Read in the file and make sure it's terminated by a newline char.
    fread((void*)buffer, 1, filesize, fp);
    buffer[filesize] = '\n';
    fclose(fp);

    upper = buffer + filesize;
    b = buffer;

    // Pass 1: calculate how much memory is needed for the lineTable.
    do
    {
        // for (i=0; b[i] != '\n'; i++) {cout << b[i];} cout << endl;
        // Ignore leading whitespace
        do
        {
            c = *b++;
        }
        while ((c == ' ') || (c == '\t'));
        // Commentary line?
        if (c == '#')
        {
            do
            {
                c = *b++;
            }
            while (c != '\n');
        }
        // Line not empty?
        else if (c != '\n')
        {
            lines++;
            i = 0;
            i = (b - buffer) - 1;
            // Make sure it contains a colon!
            do
            {
                c = *b++;
                if (c == ':')
                {
                    i = -1;
                }
            }
            while (c != '\n');
            if (i >= 0)
            {
                cerr << "Bad line format (missing colon): ";
                while (buffer[i] != '\n')
                {
                    cout << buffer[i];
                    i++;
                }
                cout << endl;
                delete [] buffer;
                buffer = NULL;
                lines = 0;
                return;
            }
        }
    }
    while (b < upper);

    if ((lineTable = new char*[lines]) == NULL)
    {
        cerr << "Not enough memory for lineTable!" << endl;
        delete [] buffer;
        buffer = NULL;
        lines = 0;
        return;
    }

    b = buffer;
    i = 0;

    // Pass 2: fill in lineTable
    do
    {
        do
        {
            c = *b++;
        }
        while ((c == ' ') || (c == '\t'));
        if (c == '#')
        {
            do
            {
                c = *b++;
            }
            while (c != '\n');
        }
        else if (c != '\n')
        {
            lineTable[i++] = b-1;
            do
            {
                c = *b++;
            }
            while (c != '\n');
            // terminate all non-empty lines with '\0'
            *(b-1) = '\0';
        }
    }
    while (b < upper);

    if ((unsigned int)i != lines)
    {
        cerr << "Fatal error: passes incompatible!" << endl;
    }

    sortResources(0, lines-1);

    /*cout << lines << " label definitions found." << endl;
    for (i=0; i<lines; i++)
    {
      cout << lineTable[i] << endl;
    }*/
}



/*
 *  Quicksort, using the label identifiers as key.
 */

void labelManager::sortResources(int from, int to)
{
    int i, j;
    char *swap, *l, *m;

    while (from < to)
    {
        i = (from + to) / 2;
        swap = lineTable[from];
        lineTable[from] = lineTable[i];
        lineTable[i] = swap;
        j = from;

        for (i=from+1; i<=to; i++)
        {
            l = lineTable[from];
            m = lineTable[i];
            while ((*l == *m) && (*l != ':'))
            {
                l++;
                m++;
            }
            // End of first idf reached ==> second string can at best be =, not <
            if (*l == ':') continue;
            // If end of second idf was reached the second string is <. Otherwise check chars.
            if ((*m == ':') || (*m < *l))
            {
                j++;
                swap = lineTable[j];
                lineTable[j] = lineTable[i];
                lineTable[i] = swap;
            }
        }
        swap = lineTable[from];
        lineTable[from] = lineTable[j];
        lineTable[j] = swap;

        // Select cheaper recursion branch
        if ((j - from) < (to - j))
        {
            sortResources(from, j-1);
            from = j+1;
        }
        else
        {
            sortResources(j+1, to);
            to = j-1;
        }
    }
}




/*
 *  Destructor: just frees all dynamically allocated memory.
 */

labelManager::~labelManager(void)
{
    if (lineTable != NULL) delete [] lineTable;
    if (buffer != NULL) delete [] buffer;
}




/*
 *  Look up a symbol and return a read-only pointer to the symbol value. If
 *  this symbol doesn't exist it returns a pointer to the badSymbol-member.
 */

char *labelManager::lookup(const char *symbol)
{
    int at, step, iter;
    char *s, *l;

    if (lines == 0) return badSymbol;
    at = (lines+1)/2;
    step = (at+1)/2;
    iter = lines << 1;
    if (at >= (int)lines) at = lines-1;
    // Do a binary search over the sorted items
    while (iter != 0)
    {
        // The symbol may be terminated by a colon or any character <= 32
        s = (char*)symbol;
        l = lineTable[at];
        while ((*s == *l) && (*l != ':'))
        {
            s++;
            l++;
        }
        // Was the symbol's end reached?
        if (((unsigned char)(*s) <= 32) || (*s == ':'))
        {
            // Both read to the end. Success.
            if (*l == ':') return (l+1);
            // Symbol's end reached but not label's ==> label >, i.e. step down
            at -= step;
            if (at < 0) at = 0;
        }
        else
        {
            // label's end reached ==> label <, i.e. step up. Otherwise use chars.
            if ((*l == ':') || (*l < *s))
            {
                at += step;
                if (at >= (int)lines) at = lines-1;
            }
            else
            {
                at -= step;
                if (at < 0) at = 0;
            }
        }
        step = (step+1)/2;
        iter >>= 1;
        //cout << at << ", " << step << ", " << iter << ": " << lineTable[at] << endl;
    }
    return badSymbol;
}




/*
 *  Returns the number of label definitions.
 */

int labelManager::numberOfLabels(void)
{
    return lines;
}



/*
 *  Returns a read-only pointer to the index-th label in the sorted table.
 *  Normally this shouldn't be used from an outside application, but it
 *  may be handy for debugging.
 */

char *labelManager::returnLabelNumber(unsigned int index)
{
    if (index >= lines)
    {
        return NULL;
    }
    return lineTable[index];
}




/* TEST
int main(int argc, char *argv[])
{
  int i;
  char *b;
  labelManager lman("labels.txt");

  for (i=0; i<lman.numberOfLabels(); i++)
  {
    b = lman.returnLabelNumber(i);
    while (*b != ':') cout << *b++;
    cout << ": " << lman.lookup(lman.returnLabelNumber(i)) << endl;
  }

  cout << lman.lookup("hubba") << endl;

  return 0;
}
*/
