#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>

/*
 Threads will do some 'work' and then meet on barrier.
*/

//==============================================================
// HEADER
struct thread_arg {
	int thread_num;
	int seed;
}; 

void create_thread(pthread_t* t, struct thread_arg * ta, int thread_num, int seed);
//===============================================================


// GLOBALS
// mutex + cond for barrier
pthread_mutex_t barrier_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_barrier = PTHREAD_COND_INITIALIZER;

const int MAX = 1000;

int barrier_cnt = 0;

// another condition to be set when the barrier is unlocked
// because the spurious-wakeup-while might just lock the thread 
// when the pthread_cond_broadcast() is called
int barrier_open = 1;
const int BARRIER_MAX = 3;



// IMPLEMENTATION
void *thread_body(void *thread_body_ptr) {
	struct thread_arg * thread_body = (struct thread_arg *)thread_body_ptr;
	int seed = thread_body->seed;
	int thread_num = thread_body->thread_num;

	// each thread will increment the number by random values until it reached the MAX
	int num = 0;

	while(num < MAX) {
		printf("T%d is working...\n", thread_num);
		num += rand() % seed;
		printf("T%d is resting...\n", thread_num);
	}

	printf("T%d has done its work and is now entering the barrier.\n", thread_num);

	// barrier code
	pthread_mutex_lock(&barrier_lock);
	barrier_cnt++;

	if (barrier_cnt < BARRIER_MAX) {
		barrier_open = 0;
		// spur wakeup
		while (barrier_cnt < BARRIER_MAX && barrier_open == 0) {
			printf("T%d is #%d on the barrier.\n", thread_num, barrier_cnt);
			pthread_cond_wait(&cond_barrier, &barrier_lock);
			printf("T%d has woken up!\n", thread_num);
		}
	} else {
		printf("T%d was the last to arrive to the barrier and is waking up the others.\n", thread_num);
		barrier_cnt = 0;
		barrier_open = 1;
		pthread_cond_broadcast(&cond_barrier);
	}

	pthread_mutex_unlock(&barrier_lock);
}

int main() {
	pthread_t t1, t2, t3;
	struct thread_arg ta1, ta2, ta3;

	// init rand
	srand(time(NULL));

	// init threads
	create_thread(&t1, &ta1, 1, 10);
	create_thread(&t2, &ta2, 2, 13);
	create_thread(&t3, &ta3, 3, 9);

	// don't forget to join the threads
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	return 0;
}

void create_thread(pthread_t* t, struct thread_arg * ta, int thread_num, int seed) {
	int state;

	ta->thread_num = thread_num;
	ta->seed = seed;

	state = pthread_create(t, NULL, thread_body, (void *) ta);
	if (state != 0) {
		printf("Failed to create thread %d.\n", thread_num);
	}
}