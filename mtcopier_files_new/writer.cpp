/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "writer.h"

std::ofstream Writer::output;
pthread_mutex_t Writer::sequence_mutex;
unsigned int Writer::sequence;
pthread_cond_t Writer::sequence_incremented;
Queue* Writer::queue;
SharedState* Writer::shared_state;
std::vector<WriterThreadData> Writer::threadDatas;

/**
 * implement these functions requred for the Writer class
 **/
void Writer::init(const std::string& name, unsigned int num_threads, Queue* queue, SharedState* shared_state) {
	this->sequence = 0;
	this->output.open(name);
	if (!this->output.is_open()) {
		std::cerr << "Unable to open output file " << name << std::endl;
		exit(EXIT_FAILURE);
	}
	this->num_threads = num_threads;
	this->queue = queue;
	this->shared_state = shared_state;

	threads.resize(this->num_threads);
	threadDatas.resize(this->num_threads);

	pthread_mutex_init(&sequence_mutex, NULL);
	pthread_cond_init(&sequence_incremented, NULL);
}

void Writer::run() {
	for (int i = 0; i < num_threads; i++) {
		threadDatas[i].writer = this;
		threadDatas[i].queue = this->queue;
		std::cout << "Making Writer Thread " << i << std::endl;
		int result = pthread_create(&threads[i], NULL, Writer::runner, &threadDatas[i]);
		if (result != 0) {
		std::cerr << "Error creating thread: " << strerror(result) << std::endl;
		}
	}
}

void Writer::join() {
	//join the threads
	for (int i = 0; i < this->num_threads; i++) {
		std::cout << "Joining writer thread " << i << std::endl;
		pthread_join(threads[i], NULL);
	}
}

void* Writer::runner(void* arg) { 

    WriterThreadData* threadData = (WriterThreadData*) arg;

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

Writer::~Writer() {
	output.close();
	pthread_mutex_destroy(&sequence_mutex);
	pthread_cond_destroy(&sequence_incremented);
}