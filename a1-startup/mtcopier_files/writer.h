/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#ifndef WRITER
#define WRITER
#include <pthread.h>

#include <deque>
#include <fstream>
#include <iostream>
#include <string>

#include "queue.h"
#include "shared_state.h"

class Writer {
   public:
    /**
     * Please note that methods and data need to be static as there's a
     * variety of information that needs to be coordinated between writers
     * such as locks and other shared data. Also, as the pthread library
     * is a c library, it does not know about classes so ruuner in particular
     * needs to be static. You can pass in instances into the function as
     * pointers though.
     **/
    void init(const std::string& output_file, unsigned int num_threads, Queue* queue, SharedState* shared_state);
    static void* runner(void*);
    void run();
    void join();

   private:
    std::vector<pthread_t> threads;
    static std::ofstream output;
    static pthread_mutex_t sequence_mutex;
    static pthread_cond_t sequence_incremented;
    static unsigned int sequence;
    unsigned int num_threads;
    Queue* queue;
    SharedState* shared_state;
    pthread_attr_t detached_attr;
};
#endif
