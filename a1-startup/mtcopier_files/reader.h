/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#ifndef READER
#define READER

#include <pthread.h>
#include <fstream>
#include <iostream>
#include <string>
#include "queue.h"


class Reader {
   public:
    /* this class has a bunch of static (which means shared in a class)
     * because we need to share the state between threads. For example, we
     * are reading from the same file, should have the same locks for all
     * readers, etc.
     **/

    /** initialize the shared data for the class - for example, open the
     * file. There will be other things which you will need to figure out
     * as you complete the assignment.
     **/
    void init(const std::string& input_file, unsigned int num_threads, Queue* queue);

    ~Reader();

    /**
     * the method that implements the thread. It has to be static as the first
     * implicit argument to all instance methods is 'this' and c functions don't
     * know about classes
     **/
    static void* runner(void*);

    /**
     * does the setup for and launches the threads
     **/
    void run();
    /**
     * there may be other functions you need so declare them.
     **/

    void assertFileOpen();

   private:
    static std::ifstream input;
    /**
     * There may be other private instance data you need so declare those here.
     **/
    static pthread_mutex_t sequence_mutex;
    static unsigned int sequence;
    unsigned int num_threads;
    Queue* queue;
};

#endif // READER
