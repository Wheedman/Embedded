#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BILLION  1000000000L

sem_t semaphore;

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

void waste_msecs(unsigned int msecs) {
	int i;
	for (i = 0; i < msecs; i++) {
		oneSecMethod();
	}
}
void do_nothing() {
	;
}

void* function_thread1(void* arg) {
	struct timespec time;
	int i, err;

	if (clock_gettime(CLOCK_REALTIME, &time) == -1) {
		perror("clock gettime");
		return (void *) EXIT_FAILURE;
	}

	while (1) {

		time.tv_nsec += 4 * 1000 * 1000;

		if (time.tv_nsec >= BILLION) {
			time.tv_nsec = time.tv_nsec % BILLION;
			time.tv_sec += 1;
		}

		waste_msecs(2);

		err = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &time, NULL);
		if (err != 0) {
			printf("clock nanosleep: %s \n", strerror(err));
		}
		if (i == 3) {
			sem_post(&semaphore);
			i = 0;
		}
		++i;
	}

	return NULL;
}

void* function_thread2(void* arg) {
	while (1) {
		while (sem_wait(&semaphore) && (errno == EINTR))
			;
		waste_msecs(3);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t thread_one;
	pthread_t thread_two;
	pthread_attr_t attr;
	pthread_attr_t attr2;
	int err;

	struct sched_param param;

	param.sched_priority += 12;

	pthread_attr_init(&attr);
	pthread_attr_init(&attr2);

	pthread_attr_setschedparam(&attr, &param);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);

	err = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

	if (err != 0) {
		printf("pthread_attr_setinheritsched: %s ", strerror(err));
	}

	sem_init(&semaphore, 0, 1);

	err = pthread_create(&thread_one, &attr, &function_thread1, NULL);

	if (err != 0) {
		printf("pthread_create: %s ", strerror(err));
	}

	err = pthread_create(&thread_two, &attr2, &function_thread2, NULL);

	if (err != 0) {
		printf("pthread_create: %s ", strerror(err));
	}

	err = pthread_join(thread_one, NULL);

	if (err != 0) {
		printf("pthread_create: %s ", strerror(err));
	}

	err = pthread_join(thread_two, NULL);

	if (err != 0) {
		printf("pthread_create: %s ", strerror(err));
	}

	return EXIT_SUCCESS;
}
