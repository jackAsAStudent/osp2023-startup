#include "shared_state.h"

SharedState::SharedState() {
	pthread_mutex_init(&state_mutex, NULL);
	pthread_cond_init(&readingDoneCond, NULL);
}

SharedState::~SharedState() {
	pthread_mutex_destroy(&state_mutex);
	pthread_cond_destroy(&readingDoneCond);
}

void SharedState::setReadingDone() {
	pthread_mutex_lock(&state_mutex);
	isReadingDone = true;
	pthread_mutex_unlock(&state_mutex);
	
	pthread_cond_signal(&readingDoneCond);  // Signal any waiting threads
}

bool SharedState::readingDone() {
	pthread_mutex_lock(&state_mutex);
	bool done = isReadingDone;
	pthread_mutex_unlock(&state_mutex);
	return done;
}

