#include "Queue.h"

Queue::Queue() {
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond, NULL);
}

Queue::~Queue() {
	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&cond);
}

void Queue::enqueue(DataBlock value) {
	pthread_mutex_lock(&mtx);
	q.push(value);
	pthread_cond_signal(&cond);  // Notify one waiting thread, if there is one
	pthread_mutex_unlock(&mtx);
}

DataBlock Queue::dequeue() {
	pthread_mutex_lock(&mtx);
	while (q.empty()) {
		pthread_cond_wait(&cond, &mtx);  // Unlock the mutex and wait for signal
	}
	DataBlock value = q.front();
	q.pop();
	pthread_mutex_unlock(&mtx);
	return value;
}
