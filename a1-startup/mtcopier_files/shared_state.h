#include <pthread.h>

class SharedState {
private:
    bool isReadingDone = false;
    pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t readingDoneCond = PTHREAD_COND_INITIALIZER;

public:
    void setReadingDone();
    bool readingDone(); 
};
