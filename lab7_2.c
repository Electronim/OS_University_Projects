#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>

int arrived, N; 
pthread_mutex_t mtx;
sem_t sem;

void barrier_point() {   
    sem_wait(&sem);
    sem_post(&sem);
}

void *tfun(void *arg) {
    int *tid = (int *)arg;

    printf("%d reached the barrier\n", *tid);

    pthread_mutex_lock(&mtx);
    arrived++;
    
    if (arrived == N) {
        sem_post(&sem);
    }
    sleep(rand() % 2);

    pthread_mutex_unlock(&mtx);

    barrier_point();

    printf("%d passed the barrier\n", *tid); fflush(stdin);

    return NULL;
}

int check(int nrThreads) {
    pthread_t thr[nrThreads + 1];
    int idx[nrThreads + 1];

    for (int i = 1; i <= nrThreads; ++i) idx[i] = i;

    for (int i = 1; i <= nrThreads; ++i) {
        pthread_create(&thr[i], NULL, tfun, &idx[i]);
    }

    for (int i = 1; i <= nrThreads; ++i) {
        pthread_join(thr[i], NULL);
    }

    return 1;
}

int main(int argc, char** argv) {
    if (argc < 2) return 0;
    srand(time(NULL));

    N = atoi(argv[1]);
    if (pthread_mutex_init(&mtx, NULL)) {
        perror(NULL);

        return errno;
    }

    if (sem_init(&sem, 0, 0)) {
        perror(NULL);

        return errno;
    }

    check(N);

    pthread_mutex_destroy(&mtx);

    return 0;
}