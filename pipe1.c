#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
        int     fd[2], nbytes;
        pid_t   childpid;
        char    string[] = "Hello Welcome to OS!\n";
        char    readbuffer[80];

        pipe(fd);
        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(childpid == 0)
        {
                /* Child process closes up input side of pipe */
                close(fd[0]);

                /* Send "string" through the output side of pipe */
                write(fd[1], string, (strlen(string)+1));
                exit(0);
        }
        else
        {
                /* Parent process closes up output side of pipe */
                close(fd[1]);

                /* Read in a string from the pipe */
                nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                printf("Received string: %s", readbuffer);
        }
        
        return(0);
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t childpid;
    int i, fib_sum=0, fib1=1, fib2=1, temp, status;

    int fd[2];
    int val = 0;

    // create pipe descriptors
    pipe(fd);

    childpid = fork();
    if(childpid != 0)  // parent
    {
        close(fd[1]);
        // read the data (blocking operation)
        read(fd[0], &val, sizeof(val));

        printf("Parent received value: %d\n", val);
        // close the read-descriptor
        close(fd[0]);
    }
    else  // child
    {
        // writing only, no need for read-descriptor:
        close(fd[0]);

        for(i=1; i<=12; i++)
        {
            temp = fib1;
            fib_sum = fib1+fib2;
            fib1 = fib_sum;
            fib2 = temp;
        }

        // send the value on the write-descriptor:
        write(fd[1], &fib_sum, sizeof(fib_sum)); 
        printf("Child send value: %d\n", fib_sum);

        // close the write descriptor:
        close(fd[1]);

        return fib_sum;
    }
}
