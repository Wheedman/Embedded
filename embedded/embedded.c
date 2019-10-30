#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

void* function(void* arg) {
	sleep((int) arg);
	int id = pthread_self();
	printf("This is thread %d\n", id);
	return (void *) id;
}

int main(int argc, char *argv[]) {
	printf("Welcome to the QNX Momentics IDE\n");
	pthread_t thread_one;
	pthread_t thread_two;
	pthread_attr_t attr;
	pthread_attr_t attr1;
	pthread_t returnValue;
	pthread_t returnValue1;
	int err;

	pthread_attr_init(&attr);
	pthread_attr_init(&attr1);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
	err = pthread_create(&thread_one, &attr, &function, (void *) 2);

	if (err != 0) {
		printf("pthread_create: %d ", strerror(err));
	}

	err = pthread_create(&thread_two, &attr1, &function, (void *) 4);
	if (err != 0) {
		printf("pthread_create: %d  ", strerror(err));
	}

	printf("Joining Thread One\n");

	err = pthread_join(thread_one, (void **) &returnValue);
	if (err != 0) {
		printf("pthread_join: %d ", strerror(err));
	}
	printf("returnValue %d\n", returnValue);
	printf("Joining Thread Two\n");
	err = pthread_join(thread_two, (void **) &returnValue1);
	if (err != 0) {
		printf("pthread_join: %d ", strerror(err));
	}
	printf("returnValue %d\n", returnValue1);
	printf("Difference for first thread %d\n", returnValue - thread_one);
	printf("Difference for second thread %d\n", returnValue1 - thread_two);

	return EXIT_SUCCESS;
}

