/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include "writer.h"
#include <cstdlib>
#include <pthread.h>

void cleanup() {
    /**
     * perform any cleanup you need to do here for global pointers
     **/
}

int main(int argc, char** argv) {
    atexit(cleanup);
    /**
     * check command line arguments
     **/
    /**
     * process command line arguments
     **/
    unsigned int num_threads = std::stoi(argv[1]);
    char* input_file = argv[2];
    char* output_file = argv[3];
    /**
     * initiliaze the reader and writer classes
     **/

    Queue queue = Queue();
    Reader reader = Reader();
    Writer writer = Writer();
    SharedState shared_state = SharedState();
    
    reader.init(input_file, num_threads, &queue, &shared_state);
    writer.init(output_file, num_threads, &queue, &shared_state);

    reader.run();
    writer.run();

    /**
     *
     * have loop here waiting for the threads to bomplete. Please see
     * section on avoiding busy waiting on the assignment specification to see
     * what need to be done here
      **/
    return EXIT_SUCCESS;
}
