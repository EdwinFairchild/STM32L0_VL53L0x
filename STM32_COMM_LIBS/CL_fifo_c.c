
#include "CL_fifo_h.h"
bool fifo_init(int *buff_location, int len)
{
    FIFO_RING = buff_location;
    if (len >= 2)
    {
        RING_SIZE = len;
        return true;
    }
    else
    {
        return false;
    }
}
bool fifo_insert(uint8_t data)
{
    if (FIFO_TAIL != NULLSTATE) //if buffer is not NULLSTATE, meaning its been initialized before
    {

        if (((FIFO_TAIL + 1) % RING_SIZE) == FIFO_HEAD) // check if buffer will be full if we add to tail
        {
            //if we will be full then delete from head
            FIFO_HEAD = ((FIFO_HEAD + 1) % RING_SIZE);
        }
        FIFO_TAIL = (FIFO_TAIL + 1) % RING_SIZE; // increment the fifo
    }
    else //if FIFO is in NULLSTATE then initialize the head and tail
    {
        //initiate buffer
        FIFO_HEAD = 0;
        FIFO_TAIL = 0;
    }

    FIFO_RING[FIFO_TAIL] = data;

    return false;
}

int fifo_del(void)
{
    //always delete from head because it has the oldest data
    if (FIFO_TAIL != NULLSTATE) // check for empty unitialized FIFO, NULLSTATE
    {
        int temp = FIFO_RING[FIFO_HEAD]; // get the current data were about to delete
        if (FIFO_HEAD != FIFO_TAIL)
        {
            FIFO_HEAD = ((FIFO_HEAD + 1) % RING_SIZE); //moves head forward to free up a spot back one spot
                                                       // printf("\nDeleted: %d", temp);
            return temp;                               //retruns the data we just delted, user may ignore if data not needed
        }

        //since we incremented the head we now must check if it caught up to the tail.
        //only time head can catch tail deu to being incremtented is if the buffer is empty
        //i think we still have one piece of data left at the current position of the head after
        //the incrementation we did above
        if (FIFO_HEAD == FIFO_TAIL)
        {
            FIFO_HEAD = NULLSTATE;
            FIFO_TAIL = NULLSTATE;
            //printf("\nDeleted: %d", temp);
            return temp;
        }
	    return 1;
    }
    else // if buffer is empty return -1
    {
        // printf("FIFO EMPTY");
        return -1;
    }
}

float fifo_average(void)
{
    float average = 0.0;
    int i;
    for (i = 0; i < RING_SIZE; i++)
    {
        average += FIFO_RING[i];
    }

    return (average / RING_SIZE);
}

void fifo_print(void)
{
    //====Prints from Head -> Tail --- Oldest data frist read from left to right

    //check if fifo is not empty

    int temp_FIFO_TAIL = FIFO_TAIL;
    int temp_FIFO_HEAD = FIFO_HEAD;
  //  printf("\n");

    if (temp_FIFO_TAIL > -1)
    {
        /*
        if head is ahead of tail that means tail wrapped around and is now behind head
        so from head--> to last index of buffer is old data
        and from 0 to tail is newer data. try to visualize it!!
        | 23 | 24 | 43 |  | 78 | 98| 12 | 90 |
                    T        H     
        */
        if (temp_FIFO_HEAD > temp_FIFO_TAIL)
        {
            while (temp_FIFO_HEAD < RING_SIZE)
            {
              //  printf("%d | ", FIFO_RING[temp_FIFO_HEAD++]);
            }
            temp_FIFO_HEAD = 0;
            while (temp_FIFO_HEAD <= temp_FIFO_TAIL)
            {
               // printf("%d | ", FIFO_RING[temp_FIFO_HEAD++]);
            }
        }
        else
        {
            while (temp_FIFO_HEAD <= temp_FIFO_TAIL)
            {
               // printf("%d | ", FIFO_RING[temp_FIFO_HEAD++]);
            }
        }
    }
    else
    {
       // printf("FIFO EMPTY");
    }

    //printf("\n---Head: %d Tail: %d---", FIFO_HEAD, FIFO_TAIL);
}
