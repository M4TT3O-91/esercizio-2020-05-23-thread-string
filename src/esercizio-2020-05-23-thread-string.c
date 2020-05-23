/*
 prendere la stringa di caratteri:

 char * content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. Pretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. Blandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. Etiam non quam lacus suspendisse faucibus interdum posuere. Eget sit amet tellus cras adipiscing enim eu turpis egestas. Lectus magna fringilla urna porttitor rhoncus dolor purus non. Sit amet consectetur adipiscing elit duis tristique sollicitudin nibh. Nec tincidunt praesent semper feugiat nibh. Sapien pellentesque habitant morbi tristique senectus et netus et malesuada.";

 il processo crea tre threads; passa la stringa di caratteri content come parametro a ciascuno dei tre threads.

 - il primo thread conta il numero di vocali nella stringa e restituisce questo valore.

 - il secondo thread conta il numero di consonanti nella stringa e restituisce questo valore.

 - il terzo thread conta il numero di spazi e newline nella stringa e restituisce questo valore.

 - il thread principale aspetta il risultato dei tre threads e poi scrive su stdout i risultati.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

char *text =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut \nlabore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. \nPretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. \nBlandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. \nEtiam non quam lacus suspendisse faucibus interdum posuere. \nEget sit amet tellus cras adipiscing enim eu turpis egestas. \nLectus magna fringilla urna porttitor rhoncus dolor purus non. \nSit amet consectetur adipiscing elit duis tristique sollicitudin nibh. \nNec tincidunt praesent semper feugiat nibh. \nSapien pellentesque habitant morbi tristique senectus et netus et malesuada.";

void* vocal_thread_counter(void *arg);
void* consonant_thread_counter(void *arg);
void* char_thread_counter(void *arg);
int* prepare_return_value(int value);

int main(int argc, char *argv[]) {

	pthread_t t1;
	pthread_t t2;
	pthread_t t3;

	void *res1;
	void *res2;
	void *res3;
	int *ret_value1;
	int *ret_value2;
	int *ret_value3;

	int s;
	s = pthread_create(&t1, NULL, vocal_thread_counter, text);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	s = pthread_create(&t2, NULL, consonant_thread_counter, text);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	s = pthread_create(&t3, NULL, char_thread_counter, text);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t1, &res1); //<------------JOIN  T1
	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}
	ret_value1 = (int*) res1;

	s = pthread_join(t2, &res2);//<------------JOIN  T2
	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}
	ret_value2 = (int*) res2;

	s = pthread_join(t3, &res3);//<------------JOIN  T3
	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}
	ret_value3 = (int*) res3;

	printf("Testo inserito:\n %s\n", text);
	printf("------------------------------- \n");
	printf("Nel testo inserito ci sono: \n");
	printf("  - VOCALI: %d\n", *ret_value1);
	printf("  - CONSONANTI: %d\n", *ret_value2);
	printf("  - SPAZI/NEW LINE: %d\n", *ret_value3);

	printf("BYE!\n");

	exit(EXIT_SUCCESS);

}

void* vocal_thread_counter(void *arg) {

	char *str = (char*) arg;
	int counter = 0;

//	//// ATTENZIONE!!! fuori standard!!! funziona su Linux
//	pthread_t tid = pthread_self();
//	printf("Vocal thread ID: %lu\n", tid);
//	////

	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == 'a' || str[i] == 'e' || str[i] == 'i' || str[i] == 'o'
				|| str[i] == 'u' || str[i] == 'A' || str[i] == 'E'
				|| str[i] == 'I' || str[i] == 'O' || str[i] == 'U') {
			counter++;
		}

	}
	pthread_exit((void*) prepare_return_value(counter));
	return NULL;
}

void* consonant_thread_counter(void *arg) {

	char *str = (char*) arg;
	int counter = 0;
//	//// ATTENZIONE!!! fuori standard!!! funziona su Linux
//	pthread_t tid = pthread_self();
//	printf("Consonant thread ID: %lu\n", tid);
//	////

	for (int i = 0; str[i] < strlen(str); ++i) {

		if ((str[i] >= 'a' && str[i] <= 'z')
				|| (str[i] >= 'A' && str[i] <= 'Z')) {
			if (str[i] != 'a' && str[i] != 'e' && str[i] != 'i' && str[i] != 'o'
					&& str[i] != 'u' && str[i] != 'A' && str[i] != 'E'
					&& str[i] != 'I' && str[i] != 'O' && str[i] != 'U') {
				counter++;
			}
		}
	}
	pthread_exit((void*) prepare_return_value(counter));
	return NULL;
}

void* char_thread_counter(void *arg) {

	char *str = (char*) arg;
	int counter = 0;
//	//// ATTENZIONE!!! fuori standard!!! funziona su Linux
//	pthread_t tid = pthread_self();
//	printf("Consonant thread ID: %lu\n", tid);
//	////

	for (int i = 0; str[i] < strlen(str); ++i) {
		if (str[i] == ' ' || str[i] == '\n')
			counter++;
	}
	pthread_exit((void*) prepare_return_value(counter));
	return NULL;
}

int* prepare_return_value(int value) {

	int *result_value_ptr;

	result_value_ptr = malloc(sizeof(int));

	if (result_value_ptr == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	*result_value_ptr = value;

	return result_value_ptr;
}

