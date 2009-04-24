#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>


#include "conversion/convertor.hh"
#include "conversion/bmp.hh"

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

int main(int argc, char *argv[])
{
  char filename[256] = "Somewhere.bmp";
  long fsize=0;
  FILE *fp=NULL;
  char *data=NULL;
  r_Minterval interv(1);
  r_Minterval imgInterv;
  r_Conv_BMP *bmp=NULL;
  r_convDesc desc;
  r_Type *baseType=NULL;
  char *imgData=NULL;

  if (argc > 1)
    strcpy(filename, argv[1]);

  fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    cerr << "Unable to open file " << filename << endl;
    exit(-1);
  }

  fseek(fp, 0, SEEK_END);
  fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  data = new char[fsize];
  fread(data, 1, fsize, fp);
  fclose(fp);

  interv << r_Sinterval((r_Range)0, (r_Range)fsize-1);

  cout << "Convert from BMP..." << endl;
  bmp = new r_Conv_BMP(data, interv, r_Convertor::ctype_char);
  desc = bmp->convertFrom();
  baseType = desc.destType;
  imgData = desc.dest;
  imgInterv = desc.destInterv;
  delete [] data;
  data=NULL;
  delete bmp;
  bmp=NULL;

  cout << "Convert to BMP..." << endl;
  bmp = new r_Conv_BMP(imgData, imgInterv, baseType);
  desc = bmp->convertTo("compress=1");
  fsize = desc.destInterv[0].high() - desc.destInterv[0].low() + 1;

  fp = fopen("result.bmp", "wb");
  fwrite(desc.dest, 1, fsize, fp);
  fclose(fp);

  free(desc.dest);
  desc.dest=NULL;
  delete desc.destType;
  desc.destType=NULL;
  delete bmp; 
  bmp=NULL;

  cout << "Clean up..." << endl;

  delete baseType;
  baseType=NULL;
  free(imgData);
  imgData=NULL;

  return 0;
}
