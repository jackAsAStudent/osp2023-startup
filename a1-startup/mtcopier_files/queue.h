#ifndef QUEUE
#define QUEUE

#include <pthread.h>
#include <queue>
#include <iostream>

#define BLOCK_SIZE 2048

struct DataBlock {
    int sequence_number;
    std::vector<char> buffer = std::vector<char>(BLOCK_SIZE);
	size_t actual_size;
};

class Queue {
private:

    std::queue<DataBlock> q;
    pthread_mutex_t mtx;
    pthread_cond_t buffer_ready;

public:

    Queue();

    ~Queue();

    void enqueue(DataBlock value);

    DataBlock dequeue();

    bool isEmpty();
};

#endif //QUEUE