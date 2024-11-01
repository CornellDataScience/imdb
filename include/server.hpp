#include <sys/shm.h>
#include <sys/ipc.h>

const int MAX_QUEUE_SIZE = 100;

const int MAX_MESSAGE_SIZE = 100;

//struct to represent a message
struct Message {
    char size;
    char data[MAX_MESSAGE_SIZE];
    
}


//struct to represent message queue
struct MessageQueue {
    int start;
    int size;
    //data type to represent byte 
    Message messages [MAX_QUEUE_SIZE];
}


//header for read fucntion
void read(*shm_buff, *shm_start, *buff);

//header for write function
void write(*shm_buff, *shm_start, *shm_size, *buff);

c
