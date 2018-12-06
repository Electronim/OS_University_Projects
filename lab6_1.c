#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

char *p;

void *rev(void *arg) {
    char *str = (char *)arg;
    char *strRev = (char *)malloc(strlen(str));

    int p = 0;
    while(p < strlen(str)) {
        strRev[p] = str[strlen(str) - p - 1];
        p++;
    }

    return strRev;
}

int main() {
    void *result;
    char *res = (char *) malloc(sizeof(100));
    p = (char *) malloc(sizeof(100));
    scanf("%s", p);
    
    pthread_t thr;
    if (pthread_create(&thr, NULL, rev, p)) {
        perror(NULL);
        return errno;
    }

    if(pthread_join(thr, &result)) {
        perror(NULL);
        return errno;
    }

    res = (char *) result;

    printf("%s", res);

    return 0;
}