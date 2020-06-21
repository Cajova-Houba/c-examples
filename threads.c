#include <stdio.h>
#include <pthread.h>
#include <sched.h>

/*
 Two threads will print some numbers.
*/

struct thread_arg {
	int thread_num;
	int seed;
}; 


void *thread_body(void *thread_body_ptr) {
	struct thread_arg * thread_body = (struct thread_arg *)thread_body_ptr;
	int i = 0;
	for (i = thread_body->seed; i < thread_body->seed+1000; i+=1) {
		printf("T%d: %d\n", thread_body->thread_num, i);

		// added so one can actually see the parallelism
		sched_yield();
	}
}

int main() {
	pthread_t t1, t2;
	int s1, s2;
	struct thread_arg ta1, ta2;

	// init args for both threads
	ta1.thread_num = 1;
	ta1.seed = 3;
	ta2.thread_num = 2;
	ta2.seed = 7;

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