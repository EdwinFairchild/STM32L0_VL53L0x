#ifndef fifo_h_H_
#define fifo_h_H_

//----------------------|  Includes    |--------------------------
#include <stdint.h>
#include <stdbool.h>

//----------------------| Defines varaibles |--------------------------

#define NULLSTATE -1 //using -1 as a value to tell me the head /tail are still in unused state

//-----TESTING ppreprocessor defines
/*
#ifndef  RING_SIZE 
    #define RING_SIZE 10
#endif

#ifndef FIFO_RING
    volatile char mybuffer[RING_SIZE];
    #define FIFO_RING mybuffer

#endif
*/
//-----TESTING pointer init
int *FIFO_RING;
int RING_SIZE = 10;

volatile int FIFO_HEAD = NULLSTATE; //initializing both head and tail to the NULLSTATE
volatile int FIFO_TAIL = NULLSTATE;

//----------------------| Fucntion Prototypes |--------------------------
bool    fifo_insert(uint8_t data);
int     fifo_del(void);
void    fifo_print(void);
bool    fifo_init(int *buff_location, int len);
float   fifo_average(void);
//----------------------| Fucntion  |--------------------------


#endif