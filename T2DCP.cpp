/*    PROGRAM T2DCP  */

#include  <iostream>
#include  <mpi.h>
#define   ntotal  200
#define   np  4
#define   n   50
using namespace std;

int main(int argc,char **argv)
{

/*     Data & Computational Partition Using MPI_Scatter, MPI_Gather
       value of n must be modified when run on other than 4 processors
*/
	
	int i;
 	FILE *fp;
   	double a[n], b[n], c[n], d[n], t[ntotal], suma, sumall;
   	int nproc, myid, istart, iend, iroot, idest;
   	MPI_Comm comm;
   	//MPI_Status istat[8];
   	extern int mod;

   	MPI_Init (&argc, &argv);
   	MPI_Comm_size (MPI_COMM_WORLD, &nproc);
   	MPI_Comm_rank (MPI_COMM_WORLD, &myid);
   	comm = MPI_COMM_WORLD;
   	istart = 0;
   	iend = n-1;

	//read input data and distribute input data
   	if (nproc != np)  
	{
      		cout<<"nproc not equal to np, program will stop"<<endl; 
      		MPI_Finalize();
      		return 0;
   	}

	if (myid == 0) 
	{
      		fp = fopen( "input.dat", "r");
      		fread( (void *)&t, sizeof(t), 1, fp );
   	}    
   	iroot=0;
   	MPI_Scatter((void *)&t, n, MPI_DOUBLE, (void *)&b, n, MPI_DOUBLE, iroot, comm);
   	if(myid == 0) 
	{
      		fread( (void *)&t, sizeof(t), 1, fp );
   	}    
   	MPI_Scatter((void *)&t, n, MPI_DOUBLE, (void *)&c, n, MPI_DOUBLE, iroot, comm);
   	if(myid == 0) 
	{
      		fread( (void *)&t, sizeof(t), 1, fp );
   	}    
   	MPI_Scatter((void *)&t, n, MPI_DOUBLE, (void *)&d, n, MPI_DOUBLE, iroot, comm);
      	
	//compute, gather computed data, and write out the result
      	suma=0.0;
	/*for(i=0; i<ntotal; i++) {  */
   	for(i=istart; i<=iend; i++) 
	{
      		a[i]=b[i]+c[i]*d[i];
      		suma=suma+a[i];
   	}
   	idest=0;
   	MPI_Gather((void *)&a, n, MPI_DOUBLE, (void *)&t, n, MPI_DOUBLE, idest, comm);
   	MPI_Reduce((void *)&suma, (void *)&sumall, 1, MPI_DOUBLE, MPI_SUM, idest, comm);
   	if(myid == 0) 
	{   
     		for (i = 0; i < ntotal; i+=40) 
		{
			 printf( "%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",
		                a[i],a[i+5],a[i+10],a[i+15],a[i+20],a[i+25],a[i+30],a[i+35]);
     		}
        	cout<<"sum of A= "<<sumall<<endl;
   	}

   	MPI_Finalize();
   	return 0;
}

