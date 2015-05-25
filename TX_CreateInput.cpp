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

    	for (i = 0; i < n; i++) 
	{
      		j=i+1;
	  	b[i] = 3. / (double) j + 1.0;
	  	c[i] = 2. / (double) j + 1.0;
	  	d[i] = 1. / (double) j + 1.0;
    	}
    	fp = fopen( "input.dat", "w");
    	fwrite( (void *)b, sizeof(double)*n, 1, fp );
    	fwrite( (void *)c, sizeof(double)*n, 1, fp );
    	fwrite( (void *)d, sizeof(double)*n, 1, fp );
    	fclose( fp );
    	cout<<"--------- ok ---------"<<endl;
    	return 0;
} 
