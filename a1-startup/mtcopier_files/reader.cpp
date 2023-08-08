/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"

/**
 * Global variable declerations.
 **/
std::ifstream Reader::input;
pthread_mutex_t Reader::sequence_mutex;
unsigned int Reader::sequence;

struct ThreadData {
	Reader* reader;
	Queue* queue;
};

void Reader::init(const std::string& input_file, unsigned int num_threads, Queue* queue, SharedState* shared_state) {
	input.open(input_file);
	this->num_threads = num_threads;
	this->sequence = 0;
	this->queue = queue;
	pthread_mutex_init(&sequence_mutex, NULL);
	this->shared_state = shared_state;
}

Reader::~Reader() {
	input.close();
	pthread_mutex_destroy(&sequence_mutex);
}

void Reader::run() {
	std::vector<ThreadData> threadDatas(this->num_threads);
	std::vector<pthread_t> threads(this->num_threads);
	for (int i = 0; i < num_threads; i++) {
		threadDatas[i].reader = this;
		threadDatas[i].queue = this->queue;
		int result = pthread_create(&threads[i], NULL, Reader::runner, &threadDatas[i]);
		if (result != 0) {
		std::cerr << "Error creating thread: " << strerror(result) << std::endl;
		}
	}
	// Wait for all threads to finish.
	for (pthread_t thread : threads) {
		pthread_join(thread, NULL);
	}
}

void* Reader::runner(void* arg) { 
    ThreadData* threadData = (ThreadData*) arg;

    DataBlock block;

    while (true) {
        pthread_mutex_lock(&threadData->reader->sequence_mutex);

        // Read data into the buffer
        threadData->reader->input.read(block.buffer.data(), BLOCK_SIZE);
        
        // Get the number of characters actually read
        block.actual_size = threadData->reader->input.gcount();

        // Unlock the mutex
        pthread_mutex_unlock(&threadData->reader->sequence_mutex);

        // If no data was read, set reading done and break out of the loop
        if (block.actual_size == 0) {
			threadData->reader->shared_state->setReadingDone();
            break;
        }

		//assign the sequence number to the block and increment the sequence number.
		block.sequence_number = threadData->reader->sequence++;

        // Enqueue the data block
        threadData->queue->enqueue(block);
    }

    return nullptr; 
}


void Reader::assertFileOpen() {
	if (!input.is_open()) {
		std::cerr << "Error: File not open." << std::endl;
		exit(1);
	}
}