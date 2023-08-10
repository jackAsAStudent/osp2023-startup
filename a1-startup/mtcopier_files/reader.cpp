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
SharedState* Reader::shared_state;
Queue* Reader::queue;
std::vector<ReaderThreadData> Reader::threadDatas;

void Reader::init(const std::string& input_file, unsigned int num_threads, Queue* queue, SharedState* shared_state) {
	input.open(input_file);
	if (!input.is_open()) {
		std::cerr << "Unable to open input file " << input_file << std::endl;
		exit(EXIT_FAILURE);
	}

	//set up shared data
	this->num_threads = num_threads;
	this->sequence = 0;
	this->queue = queue;
	this->shared_state = shared_state;

	this->threads.resize(this->num_threads);
	this->threadDatas.resize(this->num_threads);

	//initialise the mutex and the detached attribute.
	pthread_mutex_init(&sequence_mutex, NULL);
}

Reader::~Reader() {
	input.close();
	pthread_mutex_destroy(&sequence_mutex);
}

void Reader::run() {
	//The threadDatas will be passed to the runner function. It contains a pointer to the reader and the queue.
	for (int i = 0; i < num_threads; i++) {
		threadDatas[i].reader = this;
		threadDatas[i].queue = this->queue;
		int result = pthread_create(&threads[i], NULL, Reader::runner, &threadDatas[i]);
		if (result != 0) {
			std::cerr << "Error creating thread: " << strerror(result) << std::endl;
		}
	}
}

void Reader::join() {
	//join the threads
	for (int i = 0; i < this->num_threads; i++) {
		pthread_join(threads[i], NULL);
	}
}

void* Reader::runner(void* arg) { 
    ReaderThreadData* threadData = (ReaderThreadData*) arg;

    DataBlock block;

    while (true) {
        pthread_mutex_lock(&threadData->reader->sequence_mutex);

        // Read data into the buffer
        threadData->reader->input.read(block.buffer.data(), BLOCK_SIZE);
        
        // Get the number of characters actually read
        block.actual_size = threadData->reader->input.gcount();

		//assign the sequence number to the block and increment the sequence number.
		block.sequence_number = threadData->reader->sequence++;

        // Unlock the mutex
        pthread_mutex_unlock(&threadData->reader->sequence_mutex);

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
