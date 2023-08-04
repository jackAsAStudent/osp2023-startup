#ifndef QUEUE
#define QUEUE

#include <pthread.h>
#include <queue>

struct DataBlock {
    int sequence_number;
    std::vector<char> buffer;
	size_t actual_size;
};

class Queue {
private:

    std::queue<DataBlock> q;
    pthread_mutex_t mtx;
    pthread_cond_t cond;

public:

    Queue();

    ~Queue();

    void enqueue(DataBlock value);

    DataBlock dequeue();
};

#endif //QUEUE