#include <stdio.h>
#include <pthread.h>
#include <sched.h>

/*
 Two threads will increment variable.
*/

struct thread_arg {
	int thread_num;
	int seed;
}; 

// global variable
int num = 0;
const int MAX = 1000;

// mutex
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


void *thread_body(void *thread_body_ptr) {
	struct thread_arg * thread_body = (struct thread_arg *)thread_body_ptr;

	int seed = thread_body->seed;

	// local value of num
	int num_priv = 0;

	do {
		pthread_mutex_lock(&mut);
		num += seed;
		num_priv = num;

		printf("T%d: %d -> %d\n", thread_body->thread_num, num-seed, num);

		pthread_mutex_unlock(&mut);
		sched_yield();
	} while(num_priv < MAX);
}

int main() {
	pthread_t t1, t2;
	int s1, s2;
	struct thread_arg ta1, ta2;

	// init args for both threads
	ta1.thread_num = 1;
	ta1.seed = 1;
	ta2.thread_num = 2;
	ta2.seed = 2;

	s1 = pthread_create(&t1, NULL, thread_body, (void *) &ta1);
	s2 = pthread_create(&t2, NULL, thread_body, (void *) &ta2);

	if (s1 != 0 || s2 != 0) {
		printf("Error while creating threads.");
	}

	// don't forget to join the threads
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}