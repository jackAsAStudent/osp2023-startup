/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "writer.h"

std::ofstream Writer::output;
pthread_mutex_t Writer::sequence_mutex;
unsigned int Writer::sequence;

struct ThreadData {
	Writer* writer;
	Queue* queue;
};

/**
 * implement these functions requred for the Writer class
 **/
void Writer::init(const std::string& name, unsigned int num_threads, Queue* queue, SharedState* shared_state) {
	this->sequence = 0;
	this->output.open(name);
	if (!output.is_open()) {
		std::cerr << "Unable to open output file " << name << std::endl;
		exit(EXIT_FAILURE);
	}
	this->num_threads = num_threads;
	this->queue = queue;
	this->shared_state = shared_state;

	pthread_mutex_init(&sequence_mutex, NULL);
	pthread_cond_init(&sequence_incremented, NULL);
	pthread_attr_init(&(this->detached_attr));
	pthread_attr_setdetachstate(&(this->detached_attr), PTHREAD_CREATE_DETACHED);
}

void Writer::run() {
	std::vector<ThreadData> threadDatas(this->num_threads);
	std::vector<pthread_t> threads(this->num_threads);
	for (int i = 0; i < num_threads; i++) {
		threadDatas[i].writer = this;
		threadDatas[i].queue = this->queue;
		int result = pthread_create(&threads[i], &(this->detached_attr), Writer::runner, &threadDatas[i]);
		if (result != 0) {
		std::cerr << "Error creating thread: " << strerror(result) << std::endl;
		}
	}
}

void* Writer::runner(void* arg) { 
	//print out a message to show that the thread has started and which thread it is
    std::cout << "Thread " << pthread_self() << " started." << std::endl;

    ThreadData* threadData = (ThreadData*) arg;

    while (true) {

		//lock the mutex
		pthread_mutex_lock(&(threadData->writer->sequence_mutex));

		//wait until there is data in the queue. 
		while (threadData->queue->isEmpty() && !threadData->writer->shared_state->readingDone()){
			pthread_cond_wait(&(threadData->writer->sequence_incremented), &(threadData->writer->sequence_mutex));
		}

		//queue is empty and reading is finished, so break out of the loop
		if (threadData->queue->isEmpty() && threadData->writer->shared_state->readingDone()) {
			pthread_mutex_unlock(&(threadData->writer->sequence_mutex));
			break;
		}

        //Dequeue the next block
		DataBlock block = threadData->queue->dequeue();
        
		if (block.sequence_number == threadData->writer->sequence) {
			//write the block to the output file
			threadData->writer->output.write(&(block.buffer[0]), block.actual_size);

			//Increment the sequence number
			threadData->writer->sequence++;

			//Signal the condition variable
			pthread_cond_broadcast(&threadData->writer->sequence_incremented);
		}
		else {
			pthread_cond_wait(&threadData->writer->sequence_incremented, &threadData->writer->sequence_mutex);
		}

        // Unlock the mutex
        pthread_mutex_unlock(&threadData->writer->sequence_mutex);
	}

    return nullptr; 
}
