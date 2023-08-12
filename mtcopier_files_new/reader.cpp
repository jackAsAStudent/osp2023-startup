/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"

/**
 * Global variable declerations.
 **/
std::ifstream Reader::input;
pthread_mutex_t Reader::file_mutex;

void Reader::init(const std::string& input_file) {
	input.open(input_file);
	if (!input.is_open()) {
		std::cerr << "Unable to open input file " << input_file << std::endl;
		exit(EXIT_FAILURE);
	}

	//initialise the mutex
	int ret = pthread_mutex_init(&file_mutex, NULL);
	if (ret != 0) {
		std::cerr << "Error initialising mutex: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}

	//initialise the condition variable
	ret = pthread_cond_init(&file_cond, NULL);
	if (ret != 0) {
		std::cerr << "Error initialising condition variable: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
}

Reader::~Reader() {
	input.close();
	int ret = pthread_mutex_destroy(&file_mutex);
	if (ret != 0) {
		std::cerr << "Error destroying mutex: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Reader::run() {
	//The threadDatas will be passed to the runner function. It contains a pointer to the reader and the queue.
	int ret = pthread_create(this->thread, NULL, Reader::runner(), )	
}

void Reader::join() {
	//join the threads
	for (int i = 0; i < this->num_threads; i++) {
		int ret = pthread_join(threads[i], NULL);
		if (ret != 0) {
			std::cerr << "Error joining thread: " << strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

void* Reader::runner(void* arg) { 
    ReaderThreadData* threadData = (ReaderThreadData*) arg;

    DataBlock block;

    while (true) {
        int ret = pthread_mutex_lock(&threadData->reader->sequence_mutex);
		if (ret != 0) {
			std::cerr << "Error locking mutex: " << strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}

        // Read data into the buffer
        threadData->reader->input.read(block.buffer.data(), BLOCK_SIZE);
        
        // Get the number of characters actually read
        block.actual_size = threadData->reader->input.gcount();

		//assign the sequence number to the block and increment the sequence number.
		block.sequence_number = threadData->reader->sequence++;

        // Unlock the mutex
        ret = pthread_mutex_unlock(&threadData->reader->sequence_mutex);
		if (ret != 0) {
			std::cerr << "Error unlocking mutex: " << strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}

        // If no data was read, set reading done and break out of the loop
        if (block.actual_size == 0) {
			threadData->reader->shared_state->setReadingDone();
            break;
        }

        // Enqueue the data block
        threadData->queue->enqueue(block);
    }
    return nullptr; 
}
