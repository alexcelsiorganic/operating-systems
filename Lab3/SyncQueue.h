#include <Windows.h>

class SyncQueue {
private:
    int* queue;
    int size;
    int pushPos;
    int popPos;
    HANDLE full;
    HANDLE empty;
    CRITICAL_SECTION cs;
public:
    SyncQueue(int nSize);

    ~SyncQueue();

    void Insert(int nElement);

    int Remove();
};
