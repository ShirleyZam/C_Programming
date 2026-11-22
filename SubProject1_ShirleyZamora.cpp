#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
 
using namespace std;
 
bool P(int &S) {
    if (S > 0) {
        S--;
        return true;
    }
    return false;
}
 
void V(int &S, int maximum, const string &name) {
    if (S >= maximum) {
        cout << "\n PANIC: Invalid signal on " << name << "\n";
        return;
    }
    S++;
}
 
// Global variables
// (renamed from "mutex" to avoid clashing with std::mutex under "using namespace std")
int readerSlots = 2;
int mutexSem = 1;
int rw_mutex = 1;
 
int read_count = 0;
 
int activeReaders = 0;
int activeWriters = 0;
 
int readerPC[3] = {0, 0, 0};
int writerPC[2] = {0, 0};
 
bool readerCompleted[3] = {false, false, false};
bool writerCompleted[2] = {false, false};
bool panicDetected = false;
 
void panicMessage(const string &message) {
    cout << "\n PANIC: " << message << "\n";
    panicDetected = true;
}
 
void checkSync() {
    if (activeReaders > 2) {
        panicMessage("More than 2 readers are active");
        return;
    }
    if (activeWriters > 1) {
        panicMessage("More than 1 writer is active");
        return;
    }
    if (activeReaders > 0 && activeWriters > 0) {
        panicMessage("Reader and Writer active simultaneously");
        return;
    }
    if (read_count < 0 || read_count > 2) {
        panicMessage("Invalid read count");
        return;
    }
    if (readerSlots < 0 || readerSlots > 2) {
        panicMessage("Invalid reader slots");
        return;
    }
    if (mutexSem < 0 || mutexSem > 1) {
        panicMessage("Invalid mutex value");
        return;
    }
    if (rw_mutex < 0 || rw_mutex > 1) {
        panicMessage("Invalid rw_mutex value");
        return;
    }
}
 
void runReader1() {
 
    cout << "Reader " << "R1" << " | PC = " << readerPC[0] << " : ";
 
    switch (readerPC[0]) {
        case 0:
            if (P(readerSlots)) {
                cout << "Has reader slot\n";
                readerPC[0]++;
            } else {
                cout << "Waiting for reader slot\n";
            }
            break;
 
        case 1:
            if (P(mutexSem)) {
                cout << "Has mutex\n";
                readerPC[0]++;
            } else {
                cout << "Waiting for mutex\n";
            }
            break;
 
        case 2:
            read_count++;
            cout << "Incremented read_count to " << read_count << ".\n";
            readerPC[0] = (read_count == 1) ? 3 : 4;
            break;
 
        case 3:
            if (P(rw_mutex)) {
                cout << "First reader has rw_mutex\n";
                readerPC[0]++;
            } else {
                cout << "Waiting for rw_mutex\n";
            }
            break;
 
        case 4:
            V(mutexSem, 1, "mutex");
            cout << "Released mutex\n";
            readerPC[0]++;
            break;
 
        case 5:
            if (activeWriters != 0)
                panicMessage("R1 entered while writer active");
            activeReaders++;
            cout << "Reading shared resource\n";
            readerPC[0]++;
            break;
 
        case 6:
            activeReaders--;
            cout << "Finished reading\n";
            readerPC[0]++;
            break;
 
        case 7:
            if (P(mutexSem)) {
                cout << "Has mutex\n";
                readerPC[0]++;
            } else {
                cout << "Waiting to get mutex\n";
            }
            break;
 
        case 8:
            read_count--;
            cout << "Decremented read_count to " << read_count << "\n";
            readerPC[0] = (read_count == 0) ? 9 : 10;
            break;
 
        case 9:
            V(rw_mutex, 1, "rw_mutex");
            cout << "Last reader released rw_mutex\n";
            readerPC[0]++;
            break;
 
        case 10:
            V(mutexSem, 1, "mutex");
            cout << "Released mutex\n";
            readerPC[0]++;
            break;
 
        case 11:
            V(readerSlots, 2, "readerSlots");
            cout << "Released reader slot\n";
            readerPC[0]++;
            break;
 
        case 12:
            cout << "Reader " << "R1" << " completed\n";
            readerCompleted[0] = true;
            readerPC[0]++;
            break;
    }
    checkSync();
}
 
void runReader2() {
 
    cout << "Reader " << "R2" << " | PC = " << readerPC[1] << " : ";
 
    switch (readerPC[1]) {
        case 0:
            if (P(readerSlots)) {
                cout << "Has reader slot\n";
                readerPC[1]++;
            } else {
                cout << "Waiting for reader slot\n";
            }
            break;
 
        case 1:
            if (P(mutexSem)) {
                cout << "Has mutex\n";
                readerPC[1]++;
            } else {
                cout << "Waiting for mutex\n";
            }
            break;
 
        case 2:
            read_count++;
            cout << "Incremented read_count to " << read_count << ".\n";
            readerPC[1] = (read_count == 1) ? 3 : 4;
            break;
 
        case 3:
            if (P(rw_mutex)) {
                cout << "First reader has rw_mutex\n";
                readerPC[1]++;
            } else {
                cout << "Waiting for rw_mutex\n";
            }
            break;
 
        case 4:
            V(mutexSem, 1, "mutex");
            cout << "Released mutex\n";
            readerPC[1]++;
            break;
 
        case 5:
            if (activeWriters != 0)
                panicMessage("R2 entered while writer active");
            activeReaders++;
            cout << "Reading shared resource\n";
            readerPC[1]++;
            break;
 
        case 6:
            activeReaders--;
            cout << "Finished reading\n";
            readerPC[1]++;
            break;
 
        case 7:
            if (P(mutexSem)) {
                cout << "Has mutex\n";
                readerPC[1]++;
            } else {
                cout << "Waiting to get mutex\n";
            }
            break;
 
        case 8:
            read_count--;
            cout << "Decremented read_count to " << read_count << "\n";
            readerPC[1] = (read_count == 0) ? 9 : 10;
            break;
 
        case 9:
            V(rw_mutex, 1, "rw_mutex");
            cout << "Last reader released rw_mutex\n";
            readerPC[1]++;
            break;
 
        case 10:
            V(mutexSem, 1, "mutex");
            cout << "Released mutex\n";
            readerPC[1]++;
            break;
 
        case 11:
            V(readerSlots, 2, "readerSlots");
            cout << "Released reader slot\n";
            readerPC[1]++;
            break;
 
        case 12:
            cout << "Reader " << "R2" << " completed\n";
            readerCompleted[1] = true;
            readerPC[1]++;
            break;
    }
    checkSync();
}
 
void runReader3() {
 
    cout << "Reader " << "R3" << " | PC = " << readerPC[2] << " : ";
 
    switch (readerPC[2]) {
        case 0:
            if (P(readerSlots)) {
                cout << "Has reader slot\n";
                readerPC[2]++;
            } else {
                cout << "Waiting for reader slot\n";
            }
            break;
 
        case 1:
            if (P(mutexSem)) {
                cout << "Has mutex\n";
                readerPC[2]++;
            } else {
                cout << "Waiting for mutex\n";
            }
            break;
 
        case 2:
            read_count++;
            cout << "Incremented read_count to " << read_count << ".\n";
            readerPC[2] = (read_count == 1) ? 3 : 4;
            break;
 
        case 3:
            if (P(rw_mutex)) {
                cout << "First reader has rw_mutex\n";
                readerPC[2]++;
            } else {
                cout << "Waiting for rw_mutex\n";
            }
            break;
 
        case 4:
            V(mutexSem, 1, "mutex");
            cout << "Released mutex\n";
            readerPC[2]++;
            break;
 
        case 5:
            if (activeWriters != 0)
                panicMessage("R3 entered while writer active");
            activeReaders++;
            cout << "Reading shared resource\n";
            readerPC[2]++;
            break;
 
        case 6:
            activeReaders--;
            cout << "Finished reading\n";
            readerPC[2]++;
            break;
 
        case 7:
            if (P(mutexSem)) {
                cout << "Has mutex\n";
                readerPC[2]++;
            } else {
                cout << "Waiting to get mutex\n";
            }
            break;
 
        case 8:
            read_count--;
            cout << "Decremented read_count to " << read_count << "\n";
            readerPC[2] = (read_count == 0) ? 9 : 10;
            break;
 
        case 9:
            V(rw_mutex, 1, "rw_mutex");
            cout << "Last reader released rw_mutex\n";
            readerPC[2]++;
            break;
 
        case 10:
            V(mutexSem, 1, "mutex");
            cout << "Released mutex\n";
            readerPC[2]++;
            break;
 
        case 11:
            V(readerSlots, 2, "readerSlots");
            cout << "Released reader slot\n";
            readerPC[2]++;
            break;
 
        case 12:
            cout << "Reader " << "R3" << " completed\n";
            readerCompleted[2] = true;
            readerPC[2]++;
            break;
    }
    checkSync();
}
 
void runWriter1() {
 
    cout << "Writer " << "W1" << " | PC = " << writerPC[0] << " : ";
 
    switch (writerPC[0]) {
        case 0:
            if (P(rw_mutex)) {
                cout << "W1" << " has rw_mutex\n";
                writerPC[0]++;
            } else {
                cout << "Waiting for rw_mutex\n";
            }
            break;
 
        case 1:
            if (activeReaders != 0 || activeWriters != 0) {
                panicMessage("W1 entered while another process active");
                return;
            }
            activeWriters++;
            cout << "Writing to shared resource\n";
            writerPC[0]++;
            break;
 
        case 2:
            activeWriters--;
            cout << "Finished writing\n";
            writerPC[0]++;
            break;
 
        case 3:
            V(rw_mutex, 1, "rw_mutex");
            cout << "Released rw_mutex\n";
            writerPC[0]++;
            break;
 
        case 4:
            cout << "Writer " << "W1" << " completed\n";
            writerCompleted[0] = true;
            writerPC[0]++;
            break;
    }
    checkSync();
}
 
void runWriter2() {
 
    cout << "Writer " << "W2" << " | PC = " << writerPC[1] << " : ";
 
    switch (writerPC[1]) {
        case 0:
            if (P(rw_mutex)) {
                cout << "W2" << " has rw_mutex\n";
                writerPC[1]++;
            } else {
                cout << "Waiting for rw_mutex\n";
            }
            break;
 
        case 1:
            if (activeReaders != 0 || activeWriters != 0) {
                panicMessage("W2 entered while another process active");
                return;
            }
            activeWriters++;
            cout << "Writing to shared resource\n";
            writerPC[1]++;
            break;
 
        case 2:
            activeWriters--;
            cout << "Finished writing\n";
            writerPC[1]++;
            break;
 
        case 3:
            V(rw_mutex, 1, "rw_mutex");
            cout << "Released rw_mutex\n";
            writerPC[1]++;
            break;
 
        case 4:
            cout << "Writer " << "W2" << " completed\n";
            writerCompleted[1] = true;
            writerPC[1]++;
            break;
    }
    checkSync();
}
 
int main() {
    srand(time(0));
 
    cout << "Readers: 3\n";
    cout << "Writers: 2\n";
    cout << "readerSlots = 2\n";
    cout << "mutex = 1\n";
    cout << "rw_mutex = 1\n";
 
    int completedCount = 0;
    int turn = 1;
 
    while (completedCount < 5 && !panicDetected) {
        int available[5];
        int count = 0;
 
        for (int i = 0; i < 3; i++)
            if (!readerCompleted[i])
                available[count++] = i;
 
        for (int i = 0; i < 2; i++)
            if (!writerCompleted[i])
                available[count++] = i + 3;
 
        int selected = available[rand() % count];
 
        cout << "\nTurn " << turn++ << " | ";
 
        switch (selected) {
            case 0: runReader1(); break;
            case 1: runReader2(); break;
            case 2: runReader3(); break;
            case 3: runWriter1(); break;
            case 4: runWriter2(); break;
        }
 
        if (panicDetected) {
            cout << "\nSimulation stopped because of a panic.\n";
            break;
        }
 
        completedCount = 0;
        for (int i = 0; i < 3; i++)
            if (readerCompleted[i]) completedCount++;
        for (int i = 0; i < 2; i++)
            if (writerCompleted[i]) completedCount++;
    }
 
    if (!panicDetected)
        cout << "\nAll five processes completed\n";
 
    cout << "Final read_count: " << read_count << "\n";
    cout << "Final readerSlots: " << readerSlots << "\n";
    cout << "Final mutex: " << mutexSem << "\n";
    cout << "Final rw_mutex: " << rw_mutex << "\n";
 
    return 0;
}

