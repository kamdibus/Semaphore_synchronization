#include "ipclib.h"

int main()
{
    int m_fd;
    int *buffer, *used,
        *readA, *readB,
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
        down(full);
        down(full);
        down(mutex);

        printf("Consumer ");
        // getchar();
        printf("\tentered CS\n");

        buffer = attach_mem(m_fd);

        used = buffer + USED;
        readA = buffer + RDA;
        readB = buffer + RDB;
        indexr = buffer + INDEXR;
        readFrom = buffer + *indexr;

        if( (*readA && !(*readB)) || (!(*readA) && *readB) || (*readA && *readB && *used==1) )
        {
            printf("\tConsuming one:\t%d, A: %d, B: %d\n", *readFrom, *readA, *readB);

            --*used;
            *indexr = (*indexr + 1) % SIZE;

            up(empty);
            *readA = 0;
            up(ra);
            *readB = 0;
            up(rb);

            up(full);
            getchar(); //rare case - one in 125, so stopping here to demonstrate that it happends
        }
        else if(*readA && *readB)
        {
            printf("\tConsuming both\t, A: %d, B: %d\n", *readA, *readB);

            --*used;

            up(empty);
            *indexr = (*indexr + 1) % SIZE;
            printf("\tfirst: %d, second: %d\n", *indexr, *indexr+1);

            --*used;
            *indexr = (*indexr + 1) % SIZE;

            up(empty);

            *readA = 0;
            up(ra);
            *readB = 0;
            up(rb);
        }
        else
        {
            printf("\tConsument waits, no one has read the element\n");
            up(full);
            up(full);

        }
        printf("\tConsument leaves CS \n");

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
