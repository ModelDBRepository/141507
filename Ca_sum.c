/* 05/29/03 P. Kudela now multi-class supported */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include "lnet.h"

#define SAMPLING 100000 /* for 100000 Hz sampling */

char flags[100];
FILE *cfg;


int main(int argc ,char *argv[])
{

  FILE **Ca_inp, *Ca_out;
  int iproc, i,j,nproc,npr;
  char fname[100],name[100],outname[100];
  int itime,*shnr,ns,no_kind,runtime,timemax;
  double Ca_temp,Ca_summer,Ca_ave;
 

  if(argc<4)
    {fprintf(stderr,"USAGE:\n\tCa_sum <inpname> <outname> <runtime(secs)>\n\t no ext in names\n");return 1;}
  

  strcpy(name,argv[1]);
  strcat(name,".node");
  strcpy(outname,argv[2]);

  if((cfg = fopen(name,"r"))==NULL)
    {fprintf(stderr,"ERROR:fopen(%s)\n",name);return 2;}
  if(fscanf(cfg,"%i\n",&no_kind)!=1)
      {fprintf(stderr,"ERROR:fread(%s)\n",name);return 2;}
  if((shnr=(int*)calloc(no_kind,sizeof(int)))==NULL)
    {fprintf(stderr,"ERROR:calloc(shnr)\n");return 1;}

  for(i=0;i<no_kind;i++)
    {
    if(fscanf(cfg,"%i\n",shnr+i)!=1)
      {fprintf(stderr,"ERROR:fread(%s)\n",name);return 2;}
    }

  if(fscanf(cfg,"%i\n",&ns)!=1)
      {fprintf(stderr,"ERROR:fread(%s)\n",name);return 2;}
  if(fscanf(cfg,"%i",&npr)!=1)
      {fprintf(stderr,"ERROR:fread(%s)\n",name);return 2;}
  fclose(cfg);
  //npr=atoi(argv[3]);
  nproc=npr*npr;

  if((Ca_inp=(FILE**)calloc(nproc,sizeof(FILE*)))==NULL)
  {fprintf(stderr,"ERROR:calloc(Ca_inp)\n");return 1;}

  runtime=atoi(argv[3]);
  timemax=SAMPLING*runtime;
  
  if((Ca_out = fopen(outname,"w"))==NULL)
	 {fprintf(stderr,"ERROR:fopen(%s)\n",outname);return 2;} 

  for(iproc=0;iproc<nproc;iproc++)
     {
       sprintf(fname,"Ca_%s.%d",argv[1],iproc);
       if((Ca_inp[iproc] = fopen(fname,"r"))==NULL)
	 {fprintf(stderr,"ERROR:fopen(%s)\n",fname);return 2;} 
     }
     
  for(itime=0;itime<timemax;itime++){
 
      Ca_summer=0;

      for(iproc=0;iproc<nproc;iproc++)
      {
           fread(&Ca_temp,sizeof(double),1,Ca_inp[iproc]);
	   Ca_summer+=Ca_temp;
      }

      Ca_ave=Ca_summer/nproc;

      if(fwrite(&Ca_ave,sizeof(double),1,Ca_out)!=1)
           {fprintf(stderr,"ERROR:fopen(%s)\n",outname);return 2;} 

  }

     
  fclose(Ca_out);

  for(iproc=0;iproc<nproc;iproc++)
       {
	   fclose(Ca_inp[iproc]);
       }

}
