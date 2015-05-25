

/*    PROGRAM T3DCP_1   
      Boundary data exchange with data & computing partition 
      Using MPI_Gather, MPI_Scatter to gather & scatter data
*/

#include <iostream>
#include <mpi.h>
#define  ntotal  200
#define  n      50 
#define  np     4
using namespace std;

int main(int argc,char **argv)
{
   	double amax,gmax,a[n+2],b[n+2],c[n+2],d[n+2],t[ntotal];
   	int i;
   	FILE *fp;
   	int nproc,myid,istart,iend,istart2,iend1,istartm1,iendp1;
   	int r_nbr,l_nbr,lastp,iroot,itag;
   	MPI_Status istat[8];
   	MPI_Comm comm;
   	extern double max(double, double);

   	MPI_Init(&argc,&argv);
   	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

   	comm=MPI_COMM_WORLD;
   	istart=1;
   	iend=n;
   	lastp=nproc-1;
   	cout<<"NPROC,MYID,ISTART,IEND = "<<nproc<<" "<<myid<<" "<<istart<<" "<<iend<<endl;

   	istartm1=istart-1;
   	iendp1=iend+1;

   	istart2=istart;
   	if(myid == 0) istart2=2;
   	iend1=iend;
   	if(myid == lastp ) iend1=iend-1;  

   	l_nbr = myid - 1;
   	r_nbr = myid + 1;
   	if(myid == 0)     l_nbr=MPI_PROC_NULL;
   	if(myid == lastp)  r_nbr=MPI_PROC_NULL;

	/* READ'input.dat',and distribute input data */
  	if( myid==0) 
	{
    		fp=fopen("input.dat","r");
    		fread( (void *)&t,sizeof(t),1,fp);
  	}
  	iroot=0;
  	MPI_Scatter((void *)&t,n,MPI_DOUBLE,(void *)&b[1],n,MPI_DOUBLE,iroot,comm);
  	if( myid==0) 
    	fread((void *)&t,sizeof(t),1,fp);
  	MPI_Scatter((void *)&t,n,MPI_DOUBLE,(void *)&c[1],n,MPI_DOUBLE,iroot,comm);
  	if( myid==0) 
	{
    		fread((void *)&t,sizeof(t),1,fp);
    		fclose(fp);
  	}
  	MPI_Scatter((void *)&t,n,MPI_DOUBLE,(void *)&d[1],n,MPI_DOUBLE,iroot,comm);
      
	/* Exchange data outside the territory */

  	itag=110;
  	MPI_Sendrecv((void *)&b[iend],1,MPI_DOUBLE,r_nbr,itag,
               (void *)&b[istartm1],1,MPI_DOUBLE,l_nbr,itag,comm,istat);
  	itag=120;
  	MPI_Sendrecv((void *)&b[istart],1,MPI_DOUBLE,l_nbr,itag,
               (void *)&b[iendp1],1,MPI_DOUBLE,r_nbr,itag,comm,istat);

     	/* Compute, gather and write out the computed result */
  	amax= -1.0e12;
  	for (i=istart2; i<=iend1; i++) 
	{
    		a[i]=c[i]*d[i]+(b[i-1]+2.0*b[i]+b[i+1])*0.25;
    		amax=max(amax,a[i]);
  	}
  	MPI_Gather((void *)&a[istart],n,MPI_DOUBLE,(void *)&t,n,MPI_DOUBLE,iroot,comm);
  	MPI_Allreduce((void *)&amax,(void *)&gmax,1,MPI_DOUBLE,MPI_MAX,comm);
  	amax=gmax;
  	if(myid == 0)  
	{ 
    		for (i=0;i<ntotal;i+=40) 
		{
			printf( "%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",
		             t[i],t[i+5],t[i+10],t[i+15],t[i+20],t[i+25],t[i+30],t[i+35]);
    		}
    	cout<<"MAXIMUM VALUE OF ARRAY A is "<<amax<<endl;
  	}
  	MPI_Finalize();
  	return 0;
}
  double max(double a, double b)
{
  if(a >= b)
    return a;
  else
    return b;
}

