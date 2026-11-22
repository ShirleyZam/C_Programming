#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <string> 

using namespace std; 

bool waitSemaphore(int &S){
    if (S>0){
        S--;
        return true;
    }
    return false;
}

void signalSemaphore(int &S, int maximum, string name){
    if (S>= maximum){
        cout << "\n PANIC: Invalid singal" << name << "\n";
        exit(EXIT_FAILURE);
    }
    S++;
}

//Global Variables 

int readerSlots = 2;
int mutex = 1;
int rw_mutex = 1;

int read_count =0;

int activeReaders = 0;
int activeWriters = 0;

int readerPC[3] = {0,0,0};
int writerPC[2] = {0,0};

bool readerCompleted[3] = {false,false,false};
bool writerCompleted[2] = {false, false};

