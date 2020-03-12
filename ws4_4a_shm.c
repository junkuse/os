#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>
struct memory
{
	int factorial[3];
};
int main(int argc,char *argv[])
{
	int shmid,n,r,i,f=1;
	int pid1,pid2,pid3;
	key_t shmkey;
	struct memory *shmmemory;
	shmkey=1234;
	n=atoi(argv[1]);
	r=atoi(argv[2]);
	pid1=fork();
	if(pid1==0)
	{
		shmid=shmget(shmkey,sizeof(struct memory),IPC_CREAT|0666);
		if(shmid<0)
		{
			printf("shmget error\n");
			exit(1);
		}
		printf("child process got the shared memory\n");
		shmmemory=(struct memory*)shmat(shmid,NULL,0);
		if((int)shmmemory==-1)
		{
			printf("shmat error\n");
			exit(1);
		}
		printf("child process attached to the shared memory\n");
		for(i=1;i<=n;i++)
		{
			f=f*i;
		}
		shmmemory->factorial[0]=f;
		shmdt((void *)shmmemory);
	}
	else
	{
		pid2=fork();
		if(pid2==0)
		{
			shmid=shmget(shmkey,sizeof(struct memory),IPC_CREAT|0666);
			if(shmid<0)
                	{
                        	printf("shmget error\n");
                        	exit(1);
                	}
                	printf("child process got the shared memory\n");
                	shmmemory=(struct memory*)shmat(shmid,NULL,0);
                	if((int)shmmemory==-1)
                	{
                       		printf("shmat error\n");
                        	exit(1);
                	}
                	printf("child process attached to the shared memory\n");
                	f=1;
			for(i=1;i<=r;i++)
                	{
                        	f=f*i;
                	}
                	shmmemory->factorial[1]=f;
                	shmdt((void *)shmmemory);
		}
		else
		{
			pid3=fork();
			if(pid3==0)
			{
				shmid=shmget(shmkey,sizeof(struct memory),IPC_CREAT|0666);
        		        if(shmid<0)
        		        {
                       			printf("shmget error\n");
                        		exit(1);
                		}
                		printf("child process got the shared memory\n");
                		shmmemory=(struct memory*)shmat(shmid,NULL,0);
                		if((int)shmmemory==-1)
                		{
                        		printf("shmat error\n");
                        		exit(1);
                		}
                		printf("child process attached to the shared memory\n");
                		f=1;
				for(i=1;i<=(n-r);i++)
                		{
                        		f=f*i;
                		}
                		shmmemory->factorial[2]=f;
                		shmdt((void *)shmmemory);
			}
			else
			{
				wait(0);
				shmid=shmget(shmkey,sizeof(struct memory),0666);
				if(shmid<0)
                		{
                        		printf("shmget error\n");
                        		exit(1);
                		}
				shmmemory=(struct memory*)shmat(shmid,NULL,0);
                		if((int)shmmemory==-1)
                		{
                        		printf("shmat error\n");
                        		exit(1);
                		}
                		printf("parent process attached to the shared memory\n");
				printf("the NCR value is %d",shmmemory->factorial[0]/(shmmemory->factorial[1],shmmemory->factorial[2]));
				shmdt((void *)shmmemory);
			}
		}
	}
}
