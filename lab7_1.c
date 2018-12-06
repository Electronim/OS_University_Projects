#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define MAX_RESOURCES 100

int avResources = MAX_RESOURCES;

pthread_mutex_t mtx;

int decCount(int count) {
    pthread_mutex_trylock(&mtx);
    if (avResources < count) {
        pthread_mutex_unlock(&mtx);
        return -1;
    }

    avResources -= count;
    printf("Got %d resources %d remaining\n", count, avResources);
    sleep(rand() % 3);

    pthread_mutex_unlock(&mtx);
    
    return 0;
}

int incCount(int count) {
    pthread_mutex_trylock(&mtx);

    avResources += count;
    printf("Released %d resources %d remaining\n", count, avResources);

    pthread_mutex_unlock(&mtx);
    
    return 0;
}

void *nrResources(void *arg) {
    int randomResources = rand() % MAX_RESOURCES + 1;
    
    int nrUsed = decCount(randomResources);
    if (nrUsed != -1) incCount(randomResources);

    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) return 0;
    
    srand(time(NULL));
    printf("Available Resources: %d\n", avResources);

    if (pthread_mutex_init(&mtx, NULL)) {
        perror(NULL);
        return errno;
    }

    int nrThreads = atoi(argv[1]);
    pthread_t thr[nrThreads + 1];

    for (int i = 1; i <= nrThreads; ++i) {
        if (pthread_create(&thr[i], NULL, nrResources, NULL)) {
            perror(NULL);
            return errno;
        }
    }

    for (int i = 1; i <= nrThreads; ++i) {
        if (pthread_join(thr[i], NULL)) {
            perror(NULL);
            return errno;
        }
    }

    pthread_mutex_destroy(&mtx);
    return 0;
}