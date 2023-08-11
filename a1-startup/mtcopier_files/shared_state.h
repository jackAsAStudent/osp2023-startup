#ifndef SHARED_STATE
#define SHARED_STATE

#include <pthread.h>
#include <iostream>

class SharedState {
private:
    bool isReadingDone = false;
    pthread_mutex_t state_mutex; 
    pthread_cond_t readingDoneCond; 

public:
    SharedState();
    ~SharedState();
    void setReadingDone();
    bool readingDone(); 
};

#endif //SHARED_STATE
