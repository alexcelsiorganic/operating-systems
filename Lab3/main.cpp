#include <iostream>
#include "SyncQueue.h"
#include <ctime>

HANDLE start;

struct Producer {
    SyncQueue* queue;
    int id;
    int count;
};

struct Consumer {
    SyncQueue* queue;
    int count;
};

DWORD WINAPI producer(LPVOID par) {
    WaitForSingleObject(start, INFINITE);
    Producer* data = (Producer*)par;
    for (int i = 0; i < data->count; i++) {
        data->queue->Insert(data->id);
        std::cout << data->id << " was produced" << std::endl;
        Sleep(7);
    }
    std::cout << std::endl;
    return 0;
}

DWORD WINAPI consumer(LPVOID par) {
    WaitForSingleObject(start, INFINITE);
    Consumer* data = (Consumer*)par;
    for (int i = 0; i < data->count; i++) {
        int number = data->queue->Remove();
        std::cout << number << " was removed" << std::endl;
        Sleep(7);
    }
    std::cout << std::endl;
    return 0;
}


void main() {
    std::cout << "Enter queue size:\n";
    int queue_size;
    std::cin >> queue_size;
    start = CreateEvent(NULL, TRUE, FALSE, NULL);
    SyncQueue* queue = new SyncQueue(queue_size);
    int num_of_prod_threads = 0;
    std::cout << "Enter num of producer threads:\n";
    std::cin >> num_of_prod_threads;
    Producer* prod_threads = new Producer[num_of_prod_threads];
    int num_of_cons_threads = 0;
    std::cout << "Enter num of consumer threads:\n";
    std::cin >> num_of_cons_threads;
    Consumer* cons_threads = new Consumer[num_of_prod_threads];
    int count = 0;
    for (int i = 0; i < num_of_prod_threads; i++) {
        std::cout << "Num of prod_numbers" << std::endl;
        std::cin >> count;
        std::cout << std::endl;
        prod_threads[i].count = count;
        prod_threads[i].id = i + 1;
        prod_threads[i].queue = *&queue;
    }
    for (int i = 0; i < num_of_cons_threads; i++) {
        std::cout << "Num of cons_numbers" << std::endl;
        std::cin >> count;
        std::cout << std::endl;
        cons_threads[i].count = count;
        cons_threads[i].queue = *&queue;
    }   
    HANDLE* prodThreadHandls = new HANDLE[num_of_prod_threads];
    DWORD* prodThreadID = new DWORD[num_of_prod_threads];
    for (int i = 0; i < num_of_prod_threads; i++) {
        prodThreadHandls[i] = CreateThread(NULL, 0, producer, (void*)&prod_threads[i], 0, &prodThreadID[i]);
    }
    HANDLE* consThreadHandls = new HANDLE[num_of_cons_threads];
    DWORD* consThreadID = new DWORD[num_of_cons_threads];
    for (int i = 0; i < num_of_cons_threads; i++) {
        consThreadHandls[i] = CreateThread(NULL, 0, consumer, (void*)&cons_threads[i], 0, &consThreadID[i]);
    }
    HANDLE* handles = new HANDLE[num_of_prod_threads + num_of_cons_threads];
    for (int i = 0; i < num_of_prod_threads; i++) {
        handles[i] = prodThreadHandls[i];
    }
    for (int i = 0; i < num_of_cons_threads; i++) {
        handles[i + num_of_cons_threads] = consThreadHandls[i];
    }
    SetEvent(start);
    WaitForMultipleObjects(num_of_prod_threads + num_of_cons_threads, handles, TRUE, INFINITE);
    system("pause");
}