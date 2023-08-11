#include "queue.h"

Queue::Queue() {
	int ret = pthread_mutex_init(&mtx, NULL);
		if (ret != 0) {
			std::cerr << "pthread_mutex_init failed: " << strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}
	ret = pthread_cond_init(&buffer_ready, NULL);
		if (ret != 0) {
			std::cerr << "pthread_cond_init failed: " << strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}
}

Queue::~Queue() {
	int ret = pthread_mutex_destroy(&mtx);
	if (ret != 0) {
		std::cerr << "pthread_mutex_destroy failed: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
	ret = pthread_cond_destroy(&buffer_ready);
	if (ret != 0) {
		std::cerr << "pthread_cond_destroy failed: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Queue::enqueue(DataBlock value) {
	int ret = pthread_mutex_lock(&mtx);
	if (ret != 0) {
		std::cerr << "pthread_mutex_lock failed: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
	q.push(value);
	ret = pthread_cond_signal(&buffer_ready);  // Notify one waiting thread, if there is one
	if (ret != 0) {
		std::cerr << "pthread_cond_signal failed: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
	ret = pthread_mutex_unlock(&mtx);
	if (ret != 0) {
		std::cerr << "pthread_mutex_unlock failed: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}

}

DataBlock Queue::dequeue() {
	int ret = pthread_mutex_lock(&mtx);
	if (ret != 0) {
		std::cerr << "pthread_mutex_lock failed: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Pulling from Queue" << std::endl;
	while (q.empty()) {
		int ret = pthread_cond_wait(&buffer_ready, &mtx);  // Unlock the mutex and wait for signal
		if (ret != 0) {
			std::cerr << "pthread_cond_wait failed: " << strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	DataBlock value = q.front();
	q.pop();
	ret = pthread_mutex_unlock(&mtx);
	if (ret != 0) {
		std::cerr << "pthread_mutex_unlock failed: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
	return value;
}

bool Queue::isEmpty() {
	int ret = pthread_mutex_lock(&mtx);
	if (ret != 0) {
		std::cerr << "pthread_mutex_lock failed: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
	bool empty = q.empty();
	ret = pthread_mutex_unlock(&mtx);
	if (ret != 0) {
		std::cerr << "pthread_mutex_unlock failed: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
	return empty;
}
