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

void Reader::init(const std::string& input_file, Writer* writer) {
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
	ret = pthread_cond_init(&file_available, NULL);
	if (ret != 0) {
		std::cerr << "Error initialising condition variable: " << strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}

	//initialise the writer
	Reader::writer = writer;
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
	int ret = pthread_create(this->thread, NULL, Reader::runner, this);
	if (ret != 0) {
		std::cerr << "Error creating thread: " << strerror(ret) << std::endl;
	}	
}

void Reader::join() {
	//join the thread
	int ret = pthread_join(*this->thread, NULL);
	if (ret != 0) {
		std::cerr << "Error joining thread: " << strerror(ret) << std::endl;
	}
}

void* Reader::runner(void* arg) { 
	Reader* reader = (Reader*)arg;
    while (true) {
		//lock the file mutex.
        int ret = pthread_mutex_lock(&file_mutex);
		if (ret != 0) {
			std::cerr << "Error locking mutex: " << strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}

		//read a line from the file
		std::string line;
		bool eofReached = !std::getline(input, line);

		//append line to writer queue. This has to be done within critical section to ensure order is maintained.
		(*reader->writer).append(line);

		//set writer flag to indicate end of file has been reached.
		if (eofReached) {
			(*reader->writer).setFinished();
		}

		//signal other reader threads
		ret = pthread_cond_signal(&file_available);
		if (ret != 0) {
			std::cerr << "Error signalling condition variable: " << strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}

        // Unlock the mutex
        ret = pthread_mutex_unlock(&file_mutex);
		if (ret != 0) {
			std::cerr << "Error unlocking mutex: " << strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}

		//if the line is empty, we have reached the end of the file.

        // append to the writer queue


    }
    return nullptr; 
}
