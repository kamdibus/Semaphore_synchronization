#include "ipclib.h"

int main()
{

    sem_unlink(MUTEX);
    sem_unlink(EMPTY);
    sem_unlink(FULL);
    sem_unlink(RA);
    sem_unlink(RB);

    shm_unlink(MNAME);
    remove_mem();

    return 0;
}

