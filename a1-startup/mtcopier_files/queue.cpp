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
	pthread_cond_signal(&buffer_ready);  // Notify one waiting thread, if there is one
	pthread_mutex_unlock(&mtx);
}

DataBlock Queue::dequeue() {
	pthread_mutex_lock(&mtx);
	std::cout << "Pulling from Queue" << std::endl;
	while (q.empty()) {
		pthread_cond_wait(&buffer_ready, &mtx);  // Unlock the mutex and wait for signal
	}
	DataBlock value = q.front();
	q.pop();
	pthread_mutex_unlock(&mtx);
	return value;
}

bool Queue::isEmpty() {
	pthread_mutex_lock(&mtx);
	bool empty = q.empty();
	pthread_mutex_unlock(&mtx);
	return empty;
}
