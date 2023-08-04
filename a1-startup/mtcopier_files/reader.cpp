/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"

/**
 * implement the functions needed for this class
 **/
std::ifstream Reader::in;

struct ThreadData {
	Reader* reader;
	Queue* queue;
};

void Reader::init(const std::string& name, unsigned int num_threads) {
	in.open(name);
	this->num_threads = num_threads;
	sequence = 0;
}

Reader::~Reader() {
	in.close();
	pthread_mutex_destroy(&sequence_mutex);
}

void Reader::run(int numThreads, Queue& queue) {
	std::vector<ThreadData> threadDatas(numThreads);
	std::vector<pthread_t> threads(numThreads);
	for (int i = 0; i < numThreads; ++i) {
		threadDatas[i].reader = this;
		threadDatas[i].queue = &queue;
		pthread_create(&threads[i], NULL, Reader::runner, &threadDatas[i]);
	}
	// Wait for all threads to finish.
	for (pthread_t thread : threads) {
		pthread_join(thread, NULL);
	}
}

void* Reader::runner(void* arg) { 
	ThreadData* threadData = (ThreadData*) arg;

	DataBlock block;

	pthread_mutex_lock(&threadData->reader->sequence_mutex);
	block.sequence_number = threadData->reader->sequence++;

	threadData->reader->in.read(block.buffer.data(), BLOCK_SIZE);
	block.actual_size = threadData->reader->in.gcount();

	pthread_mutex_unlock(&threadData->reader->sequence_mutex);

	threadData->queue->enqueue(block);
	
	return nullptr; 
}
