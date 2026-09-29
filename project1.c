#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

int *buffer;
int buffer_size;
int counter_limit;

int in = 0;
int out = 0;
int count = 0;

int counter = 0;

//Producer thread
DWORD WINAPI producer(LPVOID param){
    int next_produced;
    while(counter < counter_limit){
        if (counter >= counter_limit)
            break;
        
        while (count == buffer_size)
        ; //do nothing

        next_produced = rand() % 10;

        buffer[in] = next_produced;

        printf("Producer: produced %d at buffer[%d]\n", next_produced, in);

        in = (in + 1) % buffer_size;

        count++;
        counter++;
    }

    return 0;
}

//Consumer thread
DWORD WINAPI consumer(LPVOID param){
    int next_consumed;

    while (counter < counter_limit){
        
         if (counter >= counter_limit)
            break;

        while (count == 0)
        ;

        next_consumed = buffer[out];

        printf("Consumer consumed %d from buffer[%d]\n", next_consumed,out);

        out = (out + 1) % buffer_size;

        count --;
        counter++;

    }

    return 0;
}

int main(int argc, char *argv[]){
    HANDLE producer_thread;
    HANDLE consumer_thread;

    if(argc != 3){
        printf("Usage %s <buffer_size> <counter_limit>\n", argv[0]);
        return 1;
    }
    buffer_size = atoi(argv[1]);
    counter_limit = atoi(argv[2]);

    if(buffer_size <= 0 || counter_limit <= 0){
        printf("Buffer size and counter limit must be greater than 0\n");
        return 1;
    }

    buffer = malloc(buffer_size * sizeof(int));

    if (buffer == NULL){
        printf("Unable to allocate buffer \n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    printf("Buffer size: %d\n", buffer_size);
    printf("Counter limit: %d\n\n", counter_limit);


    producer_thread = CreateThread(
        NULL,
        0,
        producer,
        NULL,
        0,
        NULL
    );

    if(producer_thread == NULL){
        printf("Failed to create producer thread\n");
        free(buffer);
        return 1;
    }

    consumer_thread = CreateThread(
        NULL,
        0,
        consumer,
        NULL,
        0,
        NULL
    );

    if (consumer_thread == NULL){
        printf("Failed to create consumer thread\n");
        free(buffer);
        return 1;
    }

   
    WaitForSingleObject(producer_thread,INFINITE);
    WaitForSingleObject(consumer_thread, INFINITE);

    printf("\n Final Counter: %d\n", counter);

    CloseHandle(producer_thread);
    CloseHandle(consumer_thread);

    free(buffer);

    return 0;

}