#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *A_rows;
    int *B_columns;
    int size;
} Matrix_elem;

int multi_mat_elem(Matrix_elem m) {
    int i;
    int sum = 0;
    for (i = 0; i < m.size; i++){
        sum += m.A_rows[i] * m.B_columns[i];
    }
    return sum;
}

int main(int argc, char *argv[]) {

    int i, j;
    int n;
    int threadnum;
    Matrix_elem *m;
    pthread_t *threads;

    if (argc < 3) {
        fprintf(stderr, "argv[1] = size of matrixes, argv[2] = the number of thread");
        exit(1);
    }

    n = atoi(argv[1]);
    threadnum = atoi(argv[2]);

    printf("%d %d\n", n, threadnum);

    // ensure memories
    m = malloc(sizeof(Matrix_elem) * n);

    if (m == NULL) {
        fprintf(stderr, "don't ensure memories");
        exit(2);
    }

    for (i = 0; i < n; i++) {
        m[i].A_rows = malloc(sizeof(int) * n);
        m[i].B_columns = malloc(sizeof(int) * n);
        for (j = 0; j < n; j++) {
            m[i].A_rows[j] = rand() % 10 + 1;
            m[i].B_columns[j] = rand() % 10 + 1;
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("i: %d, j: %d\n", i, j);
            printf("A:%d B:%d\n", m[i].A_rows[j], m[i].B_columns[j]);
        }
    }

    threads = (pthread_t *)malloc(threadnum);

    /*
    for (i = 0; i < threadnum; i++) {
        pthread_create(&threads[i], NULL, (int (*))multi_mat_elem, info)
    }
    */

    // decolonize memories
    printf("aaa");
    for (i = 0; i < n; i++) {
        free(m[i].A_rows);
        free(m[i].B_columns);
    }
    free(m);
    free(threads);
}
