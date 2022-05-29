#include <iostream>
#include "SyncQueue.h"

SyncQueue::SyncQueue(int nSize) {
    this->pushPos = 0;
    this->popPos = 0;
    this->size = nSize;
    this->queue = new int[nSize];
    full = CreateSemaphore(NULL, 0, size, NULL);
    empty = CreateSemaphore(NULL, size, size, NULL);
    InitializeCriticalSection(&cs);
}

SyncQueue::~SyncQueue() {
    DeleteCriticalSection(&cs);
    CloseHandle(empty);
    CloseHandle(full);
    delete[]queue;
}

void SyncQueue::Insert(int nElement) {
    WaitForSingleObject(empty, INFINITE);
    EnterCriticalSection(&cs);
    queue[pushPos++] = nElement;
    if (pushPos == size) {
        pushPos = 0;
    }
    LeaveCriticalSection(&cs);
    ReleaseSemaphore(full, 1, NULL);
}

int SyncQueue::Remove() {
    WaitForSingleObject(full, INFINITE);
    EnterCriticalSection(&cs);
    int element = queue[popPos++];
    if (popPos == size) {
        popPos = 0;
    }
    LeaveCriticalSection(&cs);
    ReleaseSemaphore(empty, 1, NULL);
    return element;
}
