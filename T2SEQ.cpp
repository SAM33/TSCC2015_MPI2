/*  PROGRAM T2SEQ
    sequential version of 1-dimensional array operation   */

#include <iostream>
#define  n  200
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main ()
{
    	double  suma, a[n], b[n], c[n], d[n];
    	int     i, j;
    	FILE  *fp;

/*     test data generation and write out to file  'input.dat'   */

    	for (i = 0; i < n; i++) 
	{
      		j=i+1;
	  	b[i] = 3. / (double) j + 1.0;
	  	c[i] = 2. / (double) j + 1.0;
	  	d[i] = 1. / (double) j + 1.0;
    	}
    	fp = fopen( "input.dat", "w");
    	fwrite( (void *)&b, sizeof(b), 1, fp );
    	fwrite( (void *)&c, sizeof(c), 1, fp );
    	fwrite( (void *)&d, sizeof(d), 1, fp );
    	fclose( fp );

/*     read  'input.dat',  compute and write out the result   */

    	fp = fopen( "input.dat", "r");
    	fread( (void *)&b, sizeof(b), 1, fp );
    	fread( (void *)&c, sizeof(c), 1, fp );
    	fread( (void *)&d, sizeof(d), 1, fp );
    	fclose( fp );
    	suma = 0.;
    	for (i = 0; i < n; i++)
	{
		  a[i] = b[i] + c[i] * d[i];
		  suma += a[i];
    	}
    	for (i = 0; i < n; i+=40) 
	{
		 printf( "%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",
                a[i],a[i+5],a[i+10],a[i+15],a[i+20],a[i+25],a[i+30],a[i+35]);
    	}
    	cout<<"Sum of A = "<<suma<<endl;
    	return 0;
} 
