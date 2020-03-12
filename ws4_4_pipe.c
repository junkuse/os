#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
int main(int argc,char *argv[])
{
	int i,pfds1[2],pfds2[2],pfds3[2],n,r,f=1,n1,n2,n3;
	pipe(pfds1);
	pipe(pfds2);
	pipe(pfds3);
	n=atoi(argv[1]);
	r=atoi(argv[2]);
	if(!fork())
	{
		for(i=1;i<=n;i++)
		{
			f=f*i;
		}
		close(pfds1[0]);
		write(pfds1[1],&f,sizeof(f));
		f=1;
		for(i=1;i<=r;i++)
		{
			f=f*i;
		}
		close(pfds2[0]);
		write(pfds2[1],&f,sizeof(f));
		f=1;
		for(i=1;i<=(n-r);i++)
		{
			f=f*i;
		}
		close(pfds3[0]);
		write(pfds3[1],&f,sizeof(f));
	}
	else
	{
		wait(0);
		close(pfds1[1]);
		read(pfds1[0],&n1,sizeof(int));
		printf("the n factorial is %d\n",n1);
		close(pfds2[1]);
                read(pfds2[0],&n2,sizeof(int));
                printf("the r factorial is %d\n",n2);
		close(pfds3[1]);
                read(pfds3[0],&n3,sizeof(int));
                printf("the n-r factorial is %d\n",n3);
		printf("NCR value is %d\n",n1/(n2*n3));
	}
}
