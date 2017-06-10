#include "ipclib.h"

int main()
{
    int m_fd;
    int *buffer, *used,
        *readB,
        *indexr, *readFrom;

    sem_t *mutex, *empty, *full, *ra, *rb;

    mutex = open_sem(MUTEX);
    empty = open_sem(EMPTY);
    full = open_sem(FULL);
    ra = open_sem(RA);
    rb = open_sem(RB);

    m_fd = get_mem();

    srand(time(NULL));

    while(1)
    {
        down(rb);
        down(full);
        down(mutex);

        printf("Reader B ");
        // getchar();
        printf("\tentered CS\n");

        buffer = attach_mem(m_fd);

        used = buffer + USED;
        readB = buffer + RDB;
        indexr = buffer + INDEXR;
        readFrom = buffer + *indexr;

        printf("\tread \t%d\tas first or second\n", *readFrom);
        *readB = 1;

        up(full);

        printf("\tB leaves CS\tElems:\t%d\n", *used);

        detach_mem(buffer);
        up(mutex);
        nsleep( (100 + rand() % 101) * MICNAN );
    }//while

    close_sem(mutex);
    close_sem(empty);
    close_sem(full);
    close_sem(ra);
    close_sem(rb);

    return 0;
}
