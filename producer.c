#include "ipclib.h"
#include <limits.h>

int main(int argc, char **argv)
{
    int m_fd, value;
    int *buffer, *used, *indexw, *writeTo;

    sem_t *mutex, *empty, *full;

    value = 1;

    mutex = open_sem(MUTEX);
    empty = open_sem(EMPTY);
    full = open_sem(FULL);

    m_fd = get_mem();
    srand(time(NULL));

    while(1)
    {
        down(empty);
        down(mutex);

        printf("Producer");

        // getchar();

        printf("\tentered CS \n ");

        buffer = attach_mem(m_fd);

        used = buffer + USED;
        indexw = buffer + INDEXW;
        writeTo = buffer + *indexw;
        *writeTo = value;

        value %= INT_MAX;
        ++value;

        *indexw = ( *indexw + 1 ) % SIZE;
        ++*used;

        printf("\tproduced \t%d\tElems: \t%d\n", *writeTo, *used);
        printf("\tLeaves CS\n");

        up(full);
        detach_mem(buffer);
        up(mutex);
        nsleep( (100 + rand() % 101) * MICNAN );
    }

    close_sem(mutex);
    close_sem(empty);
    close_sem(full);

    return 0;
}
