/* Program T2CP
   computing partition without data partition of 1-D arrays    */

#include <iostream>
#include <mpi.h>
#define n 200
using namespace std;

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


int main(int argc,char **argv)
{
	double suma,a[n],b[n],c[n],d[n];
	int i;
	FILE *fp;
	int nproc,myid,istart,iend,icount;
	int itag,isrc,idest,istart1,icount1;
	int gstart[16],gend[16],gcount[16];
	MPI_Status istat[8];
	MPI_Comm comm;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	startend(nproc,0,n-1,gstart,gend,gcount);
	istart=gstart[myid];
	iend=gend[myid];
	comm=MPI_COMM_WORLD;
	cout<<"NPROC, MYID, ISTART, IEND = "<<nproc<<", "<<myid<<", "<<istart<<", "<<iend<<endl;
	
	//Read input.dat, compute and write result
	if (myid==0)
	{
		fp=fopen("input.dat","r");
		fread((void*)&b,sizeof(b),1,fp);
                fread((void*)&c,sizeof(c),1,fp);
                fread((void*)&d,sizeof(d),1,fp);
		fclose(fp);
		for (idest=1;idest<nproc;idest++)
		{
			istart1=gstart[idest];
			icount1=gcount[idest];
			itag=10;
			MPI_Send((void*)&b[istart1],icount1,MPI_DOUBLE,idest,itag,comm);
			itag=20;
                        MPI_Send((void*)&c[istart1],icount1,MPI_DOUBLE,idest,itag,comm);
                        itag=30;
                        MPI_Send((void*)&d[istart1],icount1,MPI_DOUBLE,idest,itag,comm);
                }
	}

	else
	{
		icount=gcount[myid];
		isrc=0;
		itag=10;
                MPI_Recv((void*)&b[istart],icount,MPI_DOUBLE,isrc,itag,comm,istat);
                itag=20;
                MPI_Recv((void*)&c[istart],icount,MPI_DOUBLE,isrc,itag,comm,istat);
                itag=30;
                MPI_Recv((void*)&d[istart],icount,MPI_DOUBLE,isrc,itag,comm,istat);
	}

	//Compute,collect computed resuld and write out data
	for(i=istart;i<=iend;i++)
	{
		a[i]=b[i]+c[i]*d[i];
	}

	itag=110;
	if(myid>0)
	{
		icount=gcount[myid];
		idest=0;
		MPI_Send((void*)&a[istart],icount,MPI_DOUBLE,idest,itag,comm);
	}
	else
	{
		for (isrc=1;isrc<nproc;isrc++)
		{
			icount1=gcount[isrc];
			istart1=gstart[isrc];
			MPI_Recv((void*)&a[istart1],icount1,MPI_DOUBLE,isrc,itag,comm,istat);
		}
	}

	if (myid==0)
	{
		for (i=0;i<n;i+=40)
		{
			 printf( "%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",
		                a[i],a[i+5],a[i+10],a[i+15],a[i+20],a[i+25],a[i+30],a[i+35]);
		}

		suma=0.0;
		for (i=0;i<n;i++)
		{
			suma+=a[i];
		}
		cout<<"Sum of A = "<<suma<<endl;
	}

	MPI_Finalize();
	return 0;
}

			

		
         


