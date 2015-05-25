/*  PROGRAM T2SEQ
    sequential version of 1-dimensional array operation   */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main (int argc,char **argv)
{
    	double  suma, *a, *b, *c, *d;
    	int     i, j;

        int n;
        if(argc!=2)
        {
                printf("n=?\n");
                scanf("%d",&n);
        }
        else
        {
                n = atoi(argv[1]);
                printf("n=%d\n",n);
        }


    	FILE  *fp;
	a = (double*)malloc(sizeof(double)*n);
	b = (double*)malloc(sizeof(double)*n);
	c = (double*)malloc(sizeof(double)*n);
	d = (double*)malloc(sizeof(double)*n);

/*     read  'input.dat',  compute and write out the result   */

    	fp = fopen( "input.dat", "r");
    	fread( (void *)b, sizeof(double)*n, 1, fp );
    	fread( (void *)c, sizeof(double)*n, 1, fp );
    	fread( (void *)d, sizeof(double)*n, 1, fp );
    	fclose( fp );
    	suma = 0.;
    	for (i = 0; i < n; i++)
	{
		  a[i] = b[i] + c[i] * d[i];
		  suma += a[i];
    	}
/*
    	for (i = 0; i < n; i+=40) 
	{
		 printf( "%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",
                a[i],a[i+5],a[i+10],a[i+15],a[i+20],a[i+25],a[i+30],a[i+35]);
    	}
*/
    	cout<<"Sum of A = "<<suma<<endl;
    	return 0;
} 
