#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

double max(double a, double b)
{
	if(a >= b)
      	return a;
   	else
      	return b;
}

int main (int argc, char **argv)
{
        int ntotal;
        if(argc!=2)
        {
                printf("ntotal=?\n");
                scanf("%d",&ntotal);
        }
        else
        {
                ntotal = atoi(argv[1]);
                printf("ntotal=%d\n",ntotal);
        }
        double amax;
        double *a = (double*)malloc(sizeof(double)*ntotal);
        double *b = (double*)malloc(sizeof(double)*ntotal);
        double *c = (double*)malloc(sizeof(double)*ntotal);
        double *d = (double*)malloc(sizeof(double)*ntotal);
        int i,j;
        FILE *fp;
        /*read 'input.dat',compute,and write out the result   */
        fp=fopen("input.dat","r");
        fread((void *)b,sizeof(double)*ntotal,1,fp);
        fread((void *)c,sizeof(double)*ntotal,1,fp );
        fread((void *)d,sizeof(double)*ntotal,1,fp );
        fclose(fp);

  	amax=-1.0e12;
  	for (i=1;i<ntotal-1;i++) 
	{
      		a[i]=c[i]*d[i]+(b[i-1]+2.0*b[i]+b[i+1])*0.25;
      		amax=max(amax,a[i]);
   	}
/*
   	for (i=0;i<ntotal;i+=40) 
	{
		printf( "%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",
	              a[i],a[i+5],a[i+10],a[i+15],a[i+20],a[i+25],a[i+30],a[i+35]);
   	}
*/
   	cout<<"MAXIMUM VALUE OF A ARRAY is "<<amax<<endl;
   	return 0;
}
