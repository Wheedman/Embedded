#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>


#define MILLION  1E6

void waste_msecs(unsigned int msecs) {
	int i;
	for (i = 0; i < msecs; i++) {
		oneSecMethod();
	}
}

void oneSecMethod() {
	volatile int zero = 0, one = 1, sum;
	int i;
	for (i = 0; i < 50000; i++) {
		sum = zero + one;
	}
	for (i = 0; i < 50000; i++) {
		sum = zero + one;
	}
}

void* function(void* arg) {
	struct timespec start, stop;
	int s, ms;
	if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
		perror("clock gettime");
		return EXIT_FAILURE;
	}

	waste_msecs(1000);

	if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
		perror("clock gettime");
		return EXIT_FAILURE;
	}

	s = (stop.tv_sec - start.tv_sec) * 1000;
	ms = (stop.tv_nsec - start.tv_nsec) / MILLION;

	printf("start: %ld, n %ld \n", start.tv_sec, start.tv_nsec);
	printf("stop: %ld, n %ld \n", stop.tv_sec, stop.tv_nsec);
	printf("Waited miliseconds: %d\n", s + ms);
}

int main(int argc, char *argv[]) {
	pthread_t thread_one;
	pthread_attr_t attr;
	struct sched_param param;
	int err;

	param.sched_priority = sched_get_priority_max(SCHED_FIFO);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setschedparam(&attr, &param);

	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	err = pthread_create(&thread_one, &attr, &function, NULL);

	if (err != 0) {
		printf("pthread_create: %d ", strerror(err));
	}

	pthread_attr_getschedparam(&attr, &param);

	printf("Thread Priority: %d\n", param.sched_priority);

	//function(NULL);

	return EXIT_SUCCESS;
}

