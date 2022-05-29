#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <windows.h>


HANDLE full, empty, mutex;
int messagesNumber;

struct Message {
    char name[20];
    char text[20];

    Message() {
        strcpy_s(name, "");
        strcpy_s(text, "");
    }

    Message(const char name1[], const char text1[]) {
        strcpy_s(name, name1);
        strcpy_s(text, text1);
    }
};


void write(char* filename, char* message, char* authorName) {
    WaitForSingleObject(empty, INFINITE);
    WaitForSingleObject(mutex, INFINITE);

    std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out);

    int messageWritePos;
    char p[10];
    f.read(p, sizeof(p));
    messageWritePos = atoi(p);
    std::cout << "Message write pos: " << messageWritePos << std::endl;

    Message* m = new Message(authorName, message);
    int pos = sizeof(p) + sizeof(Message) * messageWritePos;
    f.seekp(pos, std::ios::beg);
    f.write((char*)m, sizeof(Message));

    messageWritePos++;
    if (messageWritePos == messagesNumber) {
        messageWritePos = 0;
    }
    _itoa(messageWritePos, p, 10);
    f.seekp(0, std::ios::beg);
    f.write(p, sizeof(p));

    f.close();

    ReleaseMutex(mutex);
    ReleaseSemaphore(full, 1, NULL);
}


int main(int argc, char* argv[]) {
    full = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, "Full");
    empty = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, "Empty");
    mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "Mutex");
    char* filename = argv[1];
    messagesNumber = atoi(argv[2]);
    std::cout << "Enter name:\n";
    char name[20];
    std::cin >> name;

    bool doCycle = true;
    while (doCycle) {
        std::cout << "Enter:\n1) Write\n2) Exit\n";
        int answer;
        std::cin >> answer;

        if (answer == 1) {
            std::cout << "Enter message:\n";
            char message[30];
            std::cin >> message;
            write(filename, message, name);
        }
        else if (answer == 2) {
            doCycle = false;
        }
    }
    return 0;
}