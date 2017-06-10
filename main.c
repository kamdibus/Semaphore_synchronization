#include "ipclib.h"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sighandler(int a)
{
    remove_sem(MUTEX);
    remove_sem(EMPTY);
    remove_sem(FULL);
    remove_sem(RA);
    remove_sem(RB);
    remove_mem();
    exit(1);
}

int main(int argc, char **argv)
{
    sem_t *mutex;

    //children pid
    pid_t cpid;		// int opisujacy id procesu

    //initialize shared memory
    init_mem();

    //initialize semaphores
    init_sem(MUTEX, 0);
    init_sem(EMPTY, SIZE);
    init_sem(FULL, 0);
    init_sem(RA, 1);
    init_sem(RB, 1);

    signal(SIGINT, sighandler);

    //fork and execute children
    cpid = fork();
    if(!cpid)
    {
        execvp("./rb", argv);
    }
    cpid = fork();
    if(!cpid)
    {
        execvp("./ra", argv);
    }
    cpid = fork();
    if(!cpid)
    {
        execvp("./c", argv);
    }
    cpid = fork();
    if(!cpid)
    {
        execvp("./p", argv);
    }

    //begin execution
    printf("\n\n\n\n Begin Program \n\n");

    mutex = open_sem(MUTEX);
    up(mutex);
    close_sem(mutex);

    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    //unlink semaphore names
    remove_sem(MUTEX);
    remove_sem(EMPTY);
    remove_sem(FULL);
    remove_sem(RA);
    remove_sem(RB);
    remove_mem();

    return 0;
}
