#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define MAX_THREADS 2 //producer and consumer threads 

int *buffer;
int buffer_size;
int counter_limit;

int in = 0;
int out =  0;
int count = 0;
int counter = 0;

//Producer Thread: 

DWORD WINAPI producer(LPVOID n){
    int next_produced;

    printf("Producer thread started\n");

    while(counter < counter_limit){
        while(count == buffer_size){
            ; // do nothing 
        }
        if (counter >= counter_limit){
            break;
        }
        next_produced = rand() % 10;
        buffer[in] = next_produced;
        printf("Producer produced %d at buffer [%d]\n", next_produced, in);
        
        in = (in+1)%buffer_size;

        count++;
        counter++;
    }
    printf("Producer thread terminating");
    return 0;

}


//Consumer thread 
DWORD WINAPI consumer(LPVOID n){
    int next_consumed;
    printf("Consumer thread started\n");
    
    while (counter < counter_limit){
        while (count == 0){
            ; // do nothing 
        }
        if(counter >= counter_limit){
            break;
        }
        next_consumed = buffer[out];
        printf("Consumer consumed %d from buffer[%d]\n", next_consumed, out);

        out = (out+1)%buffer_size;

        count--;
        counter++;
    }
    printf("Consumer thread terminating\n");
    return 0;
}

int main(int argc, char *argv[]){
    HANDLE hThreads[MAX_THREADS];
    DWORD id[MAX_THREADS];
    DWORD waiter;


    buffer_size = atoi(argv[1]);
    counter_limit = atoi(argv[2]);

    if(buffer_size <= 0 || counter_limit <= 0){
        printf("Buffer size and counter limit must be greater than 0\n");
        return 1;
    }

    buffer = malloc(buffer_size * sizeof(int));

    if(buffer==NULL){
      printf("Failed to allocate buffer\n");
      return 1;

    }

    srand((unsigned int)time(NULL));
     printf("Buffer size: %d\n", buffer_size);
     printf("Counter limit: %d\n\n",counter_limit);

    hThreads[0] = CreateThread(
        NULL,
        0,
        producer,
        NULL,
        0,
        &id[0]
    );
    
   hThreads[1] = CreateThread(
        NULL,
        0,
        consumer,
        NULL,
        0,
        &id[1]
    );

    if(hThreads[0] == NULL || hThreads[1] == NULL){
        printf("Failed to create thread\n");
        free(buffer);
        return 1;
    }

    waiter = WaitForMultipleObjects(
        MAX_THREADS,
        hThreads,
        TRUE,
        INFINITE
    );

    printf("\nFinal Counter:%d\n", counter);

    for(int i = 0; i<MAX_THREADS; i++){
        CloseHandle(hThreads[i]);
    }

    free(buffer);

    return 0;
}
