#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

int n, m, nn, mm;
int **a, **b;

typedef struct {
    int i, j;
} my;

void *add(void *args) {
    my *realArgs = (my *) malloc(sizeof(my));
    realArgs = args;

    int *result = (int *) malloc(sizeof(int));

    for (int i = 1; i <= m; i++) {
        (*result) += a[realArgs->i][i] * b[i][realArgs->j];
    }

    return result;
}

int main() {
    scanf("%d %d", &n, &m);

    a = (int **) malloc((n + 1) * sizeof(int *));
    for (int i = 1; i <= n; i++) a[i] = (int *) malloc((m + 1) * sizeof(int));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    scanf("%d %d", &nn, &mm);

    b = (int **) malloc((nn + 1) * sizeof(int *));
    for (int i = 1; i <= nn; i++) b[i] = (int *) malloc((mm + 1) * sizeof(int));

    for (int i = 1; i <= nn; i++) {
        for (int j = 1; j <= mm; j++) {
            scanf("%d", &b[i][j]);
        }
    }

    if (m != nn) return 0 * printf("INVALID ARGUMENTS!");

    int **c = (int **) malloc((n + 1) * sizeof(int *));
    for (int i = 1; i <= nn; i++) c[i] = (int *) malloc((mm + 1) * sizeof(int));


    pthread_t tid[200];
    int cnt = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= mm; j++) {
            my *args = (my *) malloc(sizeof(my));
            args->i = i;
            args->j = j;

            pthread_create(&tid[cnt++], NULL, add, args);
        }
    }

    cnt = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= mm; j++) {
            void *result;
            
            pthread_join(tid[cnt++], &result);

            int *res = (int *) malloc(sizeof(int));
            res = (int *) result;
            c[i][j] = *res;
        }
    }

    for (int i = 1; i <= n; i++, printf("\n"))
        for (int j = 1; j <= mm; j++, printf(" "))
            printf("%d", c[i][j]);

    return 0;
}