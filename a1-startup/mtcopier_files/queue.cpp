#include "queue.h"

Queue::Queue() {
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&buffer_ready, NULL);
}

Queue::~Queue() {
	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&buffer_ready);
}

void Queue::enqueue(DataBlock value) {
	pthread_mutex_lock(&mtx);
	q.push(value);
	//for testing.
	std::cout << std::string(value.buffer.data(), value.buffer.size()) << std::endl;
	pthread_cond_signal(&buffer_ready);  // Notify one waiting thread, if there is one
	pthread_mutex_unlock(&mtx);
}

DataBlock Queue::dequeue() {
	pthread_mutex_lock(&mtx);
	while (q.empty()) {
		pthread_cond_wait(&buffer_ready, &mtx);  // Unlock the mutex and wait for signal
	}
	DataBlock value = q.front();
	q.pop();
	pthread_mutex_unlock(&mtx);
	return value;
}
