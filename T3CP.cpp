
/*    PROGRAM T3CP   
      Boundary data exchange with computing partition without data partition 
      Using MPI_Send, MPI_Recv to distribute input data
*/

#include <iostream>
#include <mpi.h>
#define ntotal 200
using namespace std;

int main (int argc,char ** argv)
{
	double amax,gmax,a[ntotal],b[ntotal],c[ntotal],d[ntotal];
   	int i;
   	FILE *fp;
   	int nproc,myid,istart,iend,icount,r_nbr,l_nbr,lastp;
   	int itag,isrc,idest,istart1,icount1,istart2,iend1,istartm1,iendp1;
   	int gstart[16],gend[16],gcount[16];
   	MPI_Status istat[8];
   	MPI_Comm comm;
   	extern double max(double, double);
	extern void startend(int nproc,int is1,int is2,int gstart[16],int gend[16],int gcount[16]);

   	MPI_Init(&argc,&argv);
   	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
   	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
   	comm=MPI_COMM_WORLD;

   	startend (nproc,0,ntotal-1,gstart,gend,gcount);
   	istart=gstart[myid];
   	iend=gend[myid];
   	icount=gcount[myid];
   	lastp=nproc-1;
   	cout<<"NPROC,MYID,ISTART,IEND = "<<nproc<<" "<<myid<<" "<<istart<<" "<<iend<<endl;

	istartm1=istart-1;
   	iendp1=iend+1;

   	istart2=istart;
   	if (myid == 0) istart2=istart+1;
   	iend1=iend;
   	if(myid == lastp ) iend1=iend-1;  

   	l_nbr=myid-1;
   	r_nbr=myid+1;
   	if (myid == 0)   l_nbr=MPI_PROC_NULL;
   	if (myid == lastp) r_nbr=MPI_PROC_NULL;

	/* READ'input.dat',and distribute input data */

  	if ( myid==0)  
	{
    		fp = fopen( "input.dat", "r");
    		fread( (void *)&b, sizeof(b), 1, fp );
    		fread( (void *)&c, sizeof(c), 1, fp );
    		fread( (void *)&d, sizeof(d), 1, fp );
    		fclose( fp );

    		for (idest = 1; idest < nproc; idest++)  
		{
      			istart1=gstart[idest];
      			icount1=gcount[idest];
      			itag=10;
      			MPI_Send ((void *)&b[istart1], icount1, MPI_DOUBLE, idest, itag, comm);
      			itag=20;
      			MPI_Send ((void *)&c[istart1], icount1, MPI_DOUBLE, idest, itag, comm);
      			itag=30;
      			MPI_Send ((void *)&d[istart1], icount1, MPI_DOUBLE, idest, itag, comm);
    		}
  	}
  	else 
	{
    		isrc=0;
    		itag=10;
    		MPI_Recv ((void *)&b[istart], icount, MPI_DOUBLE, isrc, itag, comm, istat);
    		itag=20;
    		MPI_Recv ((void *)&c[istart], icount, MPI_DOUBLE, isrc, itag, comm, istat);
    		itag=30;
    		MPI_Recv ((void *)&d[istart], icount, MPI_DOUBLE, isrc, itag, comm, istat);
  	}

	/* Exchange data outside the territory */
   	itag=110;
   	MPI_Sendrecv((void *)&b[iend],1,MPI_DOUBLE,r_nbr,itag,
                (void *)&b[istartm1],1,MPI_DOUBLE,l_nbr,itag,comm,istat);
   	itag=120;
   	MPI_Sendrecv((void *)&b[istart],1,MPI_DOUBLE,l_nbr,itag,
                (void *)&b[iendp1],1,MPI_DOUBLE,r_nbr,itag,comm,istat);

      	/* Compute, gather and write out the computed result */
   	amax= -1.0e12;
   	for (i=istart2;i<=iend1;i++) 
	{
     		a[i]=c[i]*d[i]+(b[i-1]+2.0*b[i]+b[i+1])*0.25;
     		amax=max(amax,a[i]);
   	}
   	itag=130;
   	if (myid>0)  
	{
     		idest=0;
     		MPI_Send((void *)&a[istart],icount,MPI_DOUBLE,idest,itag,comm);
   	}
   	else   
	{
     		for (isrc=1;isrc<nproc;isrc++)  
		{
       			istart1=gstart[isrc];
       			icount1=gcount[isrc];
       			MPI_Recv((void *)&a[istart1],icount1,MPI_DOUBLE,isrc,itag,comm,istat);
     		}
   	}
   	MPI_Allreduce((void *)&amax,(void *)&gmax,1,MPI_DOUBLE,MPI_MAX,comm);
   	amax=gmax;
   	if(myid == 0)  
	{ 
     		for (i=0;i<ntotal;i+=40) 
		{
			printf( "%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",
		              a[i],a[i+5],a[i+10],a[i+15],a[i+20],a[i+25],a[i+30],a[i+35]);
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

void startend(int nproc,int is1,int is2,int gstart[16],int gend[16],int gcount[16])
{
        int i,ilength,iblock,ir;
        ilength=is2-is1+1;
        iblock=ilength/nproc;
        ir=ilength-iblock*nproc;
        for (i=0;i<nproc;i++)
        {
                if (i<ir)
                {
                        gstart[i]=is1+i*(iblock+1);
                        gend[i]=gstart[i]+iblock;
                }
                else
                {
                        gstart[i]=is1+i*iblock+ir;
                        gend[i]=gstart[i]+iblock-1;
                }
                if (ilength<1)
                {
                        gstart[i]=1;
                        gend[i]=0;
                }
                gcount[i]=gend[i]-gstart[i]+1;
        }
}
