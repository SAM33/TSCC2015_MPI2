#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 
 
int main(void)
{
	int i;
	int x[10];
	for(i=0 ; i<10 ; i++)
		x[i]=0;
        for(i=0 ; i<10 ; i++)
                printf("x[%d]=%d\n", i,x[i]);
	#pragma omp parallel
	{
		#pragma omp for lastprivate(x) 
		for(i=0;i<10;i++)
		{
			x[i]=i*10;
			printf("Thread%d: x[%d]=%d\n",omp_get_thread_num(),i,x[i]);
		}
	}
	for(i=0 ; i<10 ; i++)
		printf("x[%d]=%d\n", i,x[i]);
}


