#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Semaphore operations
bool P(int &S) {
    if (S > 0) {
        S--;
        return true;
    }
    return false;
}

void V(int &S, int maximum, const string &name) {
    if (S >= maximum) {
        cout << "\nPANIC: Invalid signal on " << name << "\n";
        return;
    }
    S++;
}

// Global variables
int readerSlots = 2;
int readerCountLock = 1;
int resourceAccessLock = 1;

int read_count = 0;

int activeReaders = 0;
int activeWriters = 0;

// Separate program counters
int readerPC[3] = {0, 0, 0};
int writerPC[2] = {0, 0};

// Critical Section status
bool readerInCS[3] = {false, false, false};
bool writerInCS[2] = {false, false};

// Completion status
bool readerCompleted[3] = {false, false, false};
bool writerCompleted[2] = {false, false};

bool panicDetected = false;

// Panic message
void panicMessage(const string &message) {
    cout << "\nPANIC: " << message << "\n";
    panicDetected = true;
}


// Check synchronization rules
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

    if (readerCountLock < 0 || readerCountLock > 1) {
        panicMessage("Invalid reader count lock");
        return;
    }

    if (resourceAccessLock < 0 || resourceAccessLock > 1) {
        panicMessage("Invalid resource access lock");
        return;
    }

    int readerStatusCount = 0;
    int writerStatusCount = 0;

    for (int i = 0; i < 3; i++) {
        if (readerInCS[i])
            readerStatusCount++;
    }

    for (int i = 0; i < 2; i++) {
        if (writerInCS[i])
            writerStatusCount++;
    }

    if (readerStatusCount != activeReaders) {
        panicMessage("Reader CS status does not match active reader count");
        return;
    }

    if (writerStatusCount != activeWriters) {
        panicMessage("Writer CS status does not match active writer count");
        return;
    }
}

// Reader process
void R1() {

    switch (readerPC[0]) {

        case 0:
            if (P(readerSlots)) {
                cout << "I'm R1 in the entry and there are "
                     << activeReaders << " readers and "
                     << activeWriters << " writers\n";
                readerPC[0]++;
            }
            break;

        case 1:
            if (P(readerCountLock))
                readerPC[0]++;
            break;

        case 2:
            read_count++;
            readerPC[0] = (read_count == 1) ? 3 : 4;
            break;

        case 3:
            if (P(resourceAccessLock))
                readerPC[0]++;
            break;

        case 4:
            V(readerCountLock, 1, "readerCountLock");
            readerPC[0]++;
            break;

        case 5:
            cout << "I'm R1 in the CS and there are "
                 << activeReaders << " other readers and "
                 << activeWriters << " writers\n";

            activeReaders++;
            readerInCS[0] = true;
            readerPC[0]++;
            break;

        case 6:
            cout << "I'm R1 leaving\n";

            activeReaders--;
            readerInCS[0] = false;
            readerPC[0]++;
            break;

        case 7:
            if (P(readerCountLock))
                readerPC[0]++;
            break;

        case 8:
            read_count--;
            readerPC[0] = (read_count == 0) ? 9 : 10;
            break;

        case 9:
            V(resourceAccessLock, 1, "resourceAccessLock");
            readerPC[0]++;
            break;

        case 10:
            V(readerCountLock, 1, "readerCountLock");
            readerPC[0]++;
            break;

        case 11:
            V(readerSlots, 2, "readerSlots");
            readerPC[0]++;
            break;

        case 12:
            readerCompleted[0] = true;
            readerPC[0]++;
            break;
    }

    checkSync();
}

void R2() {

    switch (readerPC[1]) {

        case 0:
            if (P(readerSlots)) {
                cout << "I'm R2 in the entry and there are "
                     << activeReaders << " readers and "
                     << activeWriters << " writers\n";
                readerPC[1]++;
            }
            break;

        case 1:
            if (P(readerCountLock))
                readerPC[1]++;
            break;

        case 2:
            read_count++;
            readerPC[1] = (read_count == 1) ? 3 : 4;
            break;

        case 3:
            if (P(resourceAccessLock))
                readerPC[1]++;
            break;

        case 4:
            V(readerCountLock, 1, "readerCountLock");
            readerPC[1]++;
            break;

        case 5:
            cout << "I'm R2 in the CS and there are "
                 << activeReaders << " other readers and "
                 << activeWriters << " writers\n";

            activeReaders++;
            readerInCS[1] = true;
            readerPC[1]++;
            break;

        case 6:
            cout << "I'm R2 leaving\n";

            activeReaders--;
            readerInCS[1] = false;
            readerPC[1]++;
            break;

        case 7:
            if (P(readerCountLock))
                readerPC[1]++;
            break;

        case 8:
            read_count--;
            readerPC[1] = (read_count == 0) ? 9 : 10;
            break;

        case 9:
            V(resourceAccessLock, 1, "resourceAccessLock");
            readerPC[1]++;
            break;

        case 10:
            V(readerCountLock, 1, "readerCountLock");
            readerPC[1]++;
            break;

        case 11:
            V(readerSlots, 2, "readerSlots");
            readerPC[1]++;
            break;

        case 12:
            readerCompleted[1] = true;
            readerPC[1]++;
            break;
    }

    checkSync();
}

void R3() {

    switch (readerPC[2]) {

        case 0:
            if (P(readerSlots)) {
                cout << "I'm R3 in the entry and there are "
                     << activeReaders << " readers and "
                     << activeWriters << " writers\n";
                readerPC[2]++;
            }
            break;

        case 1:
            if (P(readerCountLock))
                readerPC[2]++;
            break;

        case 2:
            read_count++;
            readerPC[2] = (read_count == 1) ? 3 : 4;
            break;

        case 3:
            if (P(resourceAccessLock))
                readerPC[2]++;
            break;

        case 4:
            V(readerCountLock, 1, "readerCountLock");
            readerPC[2]++;
            break;

        case 5:
            cout << "I'm R3 in the CS and there are "
                 << activeReaders << " other readers and "
                 << activeWriters << " writers\n";

            activeReaders++;
            readerInCS[2] = true;
            readerPC[2]++;
            break;

        case 6:
            cout << "I'm R3 leaving\n";

            activeReaders--;
            readerInCS[2] = false;
            readerPC[2]++;
            break;

        case 7:
            if (P(readerCountLock))
                readerPC[2]++;
            break;

        case 8:
            read_count--;
            readerPC[2] = (read_count == 0) ? 9 : 10;
            break;

        case 9:
            V(resourceAccessLock, 1, "resourceAccessLock");
            readerPC[2]++;
            break;

        case 10:
            V(readerCountLock, 1, "readerCountLock");
            readerPC[2]++;
            break;

        case 11:
            V(readerSlots, 2, "readerSlots");
            readerPC[2]++;
            break;

        case 12:
            readerCompleted[2] = true;
            readerPC[2]++;
            break;
    }

    checkSync();
}


// Writer process
void W1() {

    switch (writerPC[0]) {

        case 0:
            if (P(resourceAccessLock)) {
                cout << "I'm W1 in the entry and there are "
                     << activeReaders << " readers and "
                     << activeWriters << " writers\n";

                writerPC[0]++;
            }
            break;

        case 1:
            cout << "I'm W1 in the CS and there are "
                 << activeReaders << " readers and "
                 << activeWriters << " other writers\n";

            activeWriters++;
            writerInCS[0] = true;
            writerPC[0]++;
            break;

        case 2:
            cout << "I'm W1 leaving\n";

            activeWriters--;
            writerInCS[0] = false;
            writerPC[0]++;
            break;

        case 3:
            V(resourceAccessLock, 1, "resourceAccessLock");
            writerPC[0]++;
            break;

        case 4:
            writerCompleted[0] = true;
            writerPC[0]++;
            break;
    }

    checkSync();
}

void W2() {

    switch (writerPC[1]) {

        case 0:
            if (P(resourceAccessLock)) {
                cout << "I'm W2 in the entry and there are "
                     << activeReaders << " readers and "
                     << activeWriters << " writers\n";

                writerPC[1]++;
            }
            break;

        case 1:
            cout << "I'm W2 in the CS and there are "
                 << activeReaders << " readers and "
                 << activeWriters << " other writers\n";

            activeWriters++;
            writerInCS[1] = true;
            writerPC[1]++;
            break;

        case 2:
            cout << "I'm W2 leaving\n";

            activeWriters--;
            writerInCS[1] = false;
            writerPC[1]++;
            break;

        case 3:
            V(resourceAccessLock, 1, "resourceAccessLock");
            writerPC[1]++;
            break;

        case 4:
            writerCompleted[1] = true;
            writerPC[1]++;
            break;
    }

    checkSync();
}



// Main function
int main() {

    srand(time(0));

    cout << "Readers: 3\n";
    cout << "Writers: 2\n";
    cout << "readerSlots = 2\n";
    cout << "readerCountLock = 1\n";
    cout << "resourceAccessLock = 1\n";

    int completedCount = 0;

    while (completedCount < 5 && !panicDetected) {

        int available[5];
        int count = 0;

        // Find unfinished readers
        for (int i = 0; i < 3; i++) {
            if (!readerCompleted[i]) {
                available[count++] = i;
            }
        }

        // Find unfinished writers
        for (int i = 0; i < 2; i++) {
            if (!writerCompleted[i]) {
                available[count++] = i + 3;
            }
        }

        if (count == 0)
            break;

        // Randomly select a process
        int selected = available[rand() % count];


       switch (selected) {

    case 0:
        R1();
        break;

    case 1:
        R2();
        break;

    case 2:
        R3();
        break;

    case 3:
        W1();
        break;

    case 4:
        W2();
        break;
}

        if (panicDetected) {
            cout << "\nSimulation stopped because of a panic.\n";
            break;
        }


        // Count completed processes
        completedCount = 0;

        for (int i = 0; i < 3; i++) {
            if (readerCompleted[i])
                completedCount++;
        }

        for (int i = 0; i < 2; i++) {
            if (writerCompleted[i])
                completedCount++;
        }
    }

    if (!panicDetected) {
        cout << "\nAll five processes completed.\n";
    }

    cout << "\nFinal read_count: " << read_count << "\n";
    cout << "Final readerSlots: " << readerSlots << "\n";
    cout << "Final readerCountLock: " << readerCountLock << "\n";
    cout << "Final resourceAccessLock: " << resourceAccessLock << "\n";

    return 0;
}
