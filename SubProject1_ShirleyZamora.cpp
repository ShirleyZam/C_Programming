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

// Semaphores
int readerSlots = 2;
int readerCountLock = 1;
int resourceAccessLock = 1;

// Counters
int read_count = 0;
int activeReaders = 0;
int activeWriters = 0;

// Per-process state (3 readers, 2 writers)
int readerPC[3] = {0, 0, 0};
int writerPC[2] = {0, 0};
bool readerInCS[3] = {false, false, false};
bool writerInCS[2] = {false, false};
bool readerCompleted[3] = {false, false, false};
bool writerCompleted[2] = {false, false};

bool panicDetected = false;

void panicMessage(const string &message) {
    cout << "\nPANIC: " << message << "\n";
    panicDetected = true;
}

// Check synchronization rules
void checkSync() {
    if (activeReaders > 2)
        return panicMessage("More than 2 readers are active");
    if (activeWriters > 1)
        return panicMessage("More than 1 writer is active");
    if (activeReaders > 0 && activeWriters > 0)
        return panicMessage("Reader and Writer active simultaneously");
    if (read_count < 0 || read_count > 2)
        return panicMessage("Invalid read count");
    if (readerSlots < 0 || readerSlots > 2)
        return panicMessage("Invalid reader slots");
    if (readerCountLock < 0 || readerCountLock > 1)
        return panicMessage("Invalid reader count lock");
    if (resourceAccessLock < 0 || resourceAccessLock > 1)
        return panicMessage("Invalid resource access lock");

    int r = 0, w = 0;
    for (int i = 0; i < 3; i++) if (readerInCS[i]) r++;
    for (int i = 0; i < 2; i++) if (writerInCS[i]) w++;

    if (r != activeReaders)
        return panicMessage("Reader CS status does not match active reader count");
    if (w != activeWriters)
        return panicMessage("Writer CS status does not match active writer count");
}

// R1 PROCESS
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
            if (P(readerCountLock)) readerPC[0]++;
            break;

        case 2:
            read_count++;
            readerPC[0] = (read_count == 1) ? 3 : 4;
            break;

        case 3:
            if (P(resourceAccessLock)) readerPC[0]++;
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
            if (P(readerCountLock)) readerPC[0]++;
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

// R2 PROCESS
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
            if (P(readerCountLock)) readerPC[1]++;
            break;

        case 2:
            read_count++;
            readerPC[1] = (read_count == 1) ? 3 : 4;
            break;

        case 3:
            if (P(resourceAccessLock)) readerPC[1]++;
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
            if (P(readerCountLock)) readerPC[1]++;
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

// R3 PROCESS
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
            if (P(readerCountLock)) readerPC[2]++;
            break;

        case 2:
            read_count++;
            readerPC[2] = (read_count == 1) ? 3 : 4;
            break;

        case 3:
            if (P(resourceAccessLock)) readerPC[2]++;
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
            if (P(readerCountLock)) readerPC[2]++;
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

// W1 PROCESS
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

// W2 PROCESS
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

bool allFinished() {
    for (int i = 0; i < 3; i++) if (!readerCompleted[i]) return false;
    for (int i = 0; i < 2; i++) if (!writerCompleted[i]) return false;
    return true;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    while (!allFinished() && !panicDetected) {
        // IDs 0-2 = readers R1-R3, 3-4 = writers W1-W2
        int available[5];
        int count = 0;

        for (int i = 0; i < 3; i++) if (!readerCompleted[i]) available[count++] = i;
        for (int i = 0; i < 2; i++) if (!writerCompleted[i]) available[count++] = i + 3;

        int selected = available[rand() % count];

        switch (selected) {
            case 0: R1(); break;
            case 1: R2(); break;
            case 2: R3(); break;
            case 3: W1(); break;
            case 4: W2(); break;
        }

        if (panicDetected) {
            cout << "\nSimulation stopped because of a panic.\n";
            break;
        }
    }
    return 0;
}
