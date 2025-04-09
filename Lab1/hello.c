#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* hello_thread(void* arg) {
   long int id = (long int) arg;
   printf("Hello thread %ld\n", id);
   pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int n_threads; //number of threads
    pthread_t *tid; //vector with thread ID's

    //the program needs 2 arguments: the program's name and the number of threads
    if(argc < 2 ) {
        printf("ERROR: Number of arguments insufficient\nType %s <number of threads>\n", argv[0]);
        return 1;
    }  

    n_threads = atoi(argv[1]);
    tid = (pthread_t*) malloc(n_threads * sizeof(pthread_t));

    if(tid == NULL) {
        printf("Memory Allocation Error\n");
        return 2;
    }

    //creating threads
    for(long int i=0; i < n_threads; i++) {
        int ret = pthread_create(&tid[i], NULL, hello_thread, (void*) i);
        if(ret) { //pthread_create returns 0 if thread was created, and different than 0 otherwise
            printf("ERROR %d in the creation of thread %ld", ret, i);
        }
    }

    //waiting threads to finish
    for(long int i=0; i < n_threads; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("End\n");

    return 0;
}