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
void Writer::init(const std::string& name, unsigned int num_threads, Queue* queue) {
	this->sequence = 0;
	this->output.open(name);
	if (!output.is_open()) {
		std::cerr << "Unable to open output file " << name << std::endl;
		exit(EXIT_FAILURE);
	}
	this->num_threads = num_threads;
	this->queue = queue;
	pthread_mutex_init(&sequence_mutex, NULL);
	pthread_cond_init(&sequence_incremented, NULL);
}

void Writer::run() {}

void* Writer::runner(void* arg) { 
	//print out a message to show that the thread has started and which thread it is
    std::cout << "Thread " << pthread_self() << " started." << std::endl;

    ThreadData* threadData = (ThreadData*) arg;

    while (true) {

		//If the 
        //Dequeue the next block
		DataBlock block = threadData->queue->dequeue(block);
        
		//Lock the mutex
        pthread_mutex_lock(&threadData->writer->sequence_mutex);

		if (block.sequence_number == threadData->writer->sequence) {
			//write the block to the output file

			//Increment the sequence number

			//Signal the condition variable
			pthread_cond_broadcast(&threadData->writer->sequence_incremented)
		}
		else {
			pthread_cond_wait(&threadData->writer->sequence_incremented, &threadData->writer->sequence_mutex);
		}

        // Unlock the mutex
        pthread_mutex_unlock(&threadData->reader->sequence_mutex);

        // If no data was read, break out of the loop
        if (block.actual_size == 0) {
            break;
        }

        // Enqueue the data block
        threadData->queue->enqueue(block);

        // Increment sequence number for the next iteration
        block.sequence_number++;
    }

    return nullptr; 
	//Get the thread data from arg


	//Get the queue from the thread data

	//Get the output file from the thread data

	//Pull off of the queue

	//Lock the mutex if the sequence number of the block is the next in the sequence, else wait until the sequence number has increased.

	//Write the block to the output file

	//Unlock the mutex
	
	return nullptr; 
}
