#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>

#define MakerNum 6
#define ProductMaxNum 100

int semid;
int *a, *b, *c;
/*
struct ringbuf {
    int bufsize;
    int wptr, rptr;
    int n_item;
    int buf[1];
}
*/

/*
struct ringbuf *a;
struct ringbuf *b;
struct ringbuf *c;
*/

int *x, *y, *z;
int prod_time[MakerNum];
int prod_after_time[MakerNum];


void err_msg(char *msg) {
    perror(msg);
    exit(1);
}

void P(int *v) {
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = SEM_UNDO;
    semop(*v, &sops, 1);
}

void V(int *v) {
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = 1;
    sops.sem_flg = SEM_UNDO;
    semop(*v, &sops, 1);
}

void A(int prod_num, int prod_time, int prod_after_time) {
    if (*a >= ProductMaxNum) return;
    P(a);
    usleep(prod_num * prod_time);
    *a += prod_num;
    if (*a > ProductMaxNum) { *a = ProductMaxNum; }
    usleep(prod_after_time);
    V(a);
    printf("a:%d\n", *a);
}

void B(int prod_num, int prod_time, int prod_after_time) {
    if (*b >= ProductMaxNum) return;
    P(b);
    usleep(prod_num * prod_time);
    *b += prod_num;
    if (*b > ProductMaxNum) { *b = ProductMaxNum; }
    usleep(prod_after_time);
    V(b);
    printf("b:%d\n", *b);
}

void C(int prod_num, int prod_time, int prod_after_time) {
    if (*c >= ProductMaxNum) return;
    P(c);
    usleep(prod_num * prod_time);
    *c += prod_num;
    if (*c > ProductMaxNum) { *c = ProductMaxNum; }
    usleep(prod_after_time);
    V(c);
    printf("c:%d\n", *c);
}

void X(int prod_time, int prod_after_time) {
    while(*a < 5 || *b < 5) {
        P(a);
        P(b);
        usleep(1);
        V(a);
        V(b);
    }
    P(a);
    P(b);
    usleep(prod_time);
    (*x)++;
    *a -= 5;
    *b -= 5;
    usleep(prod_after_time);
    V(a);
    V(b);
    printf("x:%d\n", *x);
}

void Y(int prod_time, int prod_after_time) {
    while(*a < 2 || *b < 4 || *c < 5) {
        P(a);
        P(b);
        P(c);
        usleep(1);
        V(a);
        V(b);
        V(c);
    };
    P(a);
    P(b);
    P(c);
    usleep(prod_time);
    (*y)++;
    *a -= 2;
    *b -= 4;
    *c -= 5;
    usleep(prod_after_time);
    V(a);
    V(b);
    V(c);
    printf("y:%d\n", *y);
}

void Z(int prod_time, int prod_after_time) {
    while(*a < 4 || *b < 2 || *c < 6) {
        P(a);
        P(b);
        P(c);
        usleep(1);
        V(a);
        V(b);
        V(c);

    };
    P(a);
    P(b);
    P(c);
    usleep(prod_time);
    (*z)++;
    *a -= 4;
    *b -= 2;
    *c -= 6;
    usleep(prod_after_time);
    V(a);
    V(b);
    V(c);
    printf("z:%d\n", *z);
}

int main(int argc, char *argv[]) {
    int max_stock_num;
    int i;
    int shmid[MakerNum];
    int pid[MakerNum];
    int prod_num;
    int status;

    if (argc < MakerNum * 2 + 1) {
        err_msg("argv < 13");
    }

    max_stock_num = atoi(argv[1]);
    for (i = 0; i < MakerNum; i++) {
        prod_time[i] = atoi(argv[i + 2]);
        prod_after_time[i] = atoi(argv[i + 8]);
    }

    // 共有メモリの確保
    for (i = 0; i < MakerNum; i++) {
        shmid[i] = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if (shmid[i] == -1) {
            err_msg("shmget");
            if (shmctl(shmid[i], IPC_RMID, NULL)) { // release shared memory area
                perror("shmctl");
            }
            exit(1);
        }
    }
    a = (int *)shmat(shmid[0], NULL, 0);
    if (a == (int *) - 1) {
        shmctl(shmid[0], IPC_RMID, NULL);
        err_msg("shmat a");
    }
    b = (int *)shmat(shmid[1], NULL, 0);
    if (b == (int *) - 1) {
        shmctl(shmid[1], IPC_RMID, NULL);
        err_msg("shmat b");
    }
    c = (int *)shmat(shmid[2], NULL, 0);
    if (c == (int *) - 1) {
        shmctl(shmid[2], IPC_RMID, NULL);
        err_msg("shmat c");
    }
    x = (int *)shmat(shmid[3], NULL, 0);
    if (x == (int *) - 1) {
        shmctl(shmid[3], IPC_RMID, NULL);
        err_msg("shmat x");
    }
    y = (int *)shmat(shmid[4], NULL, 0);
    if (y == (int *) - 1) {
        shmctl(shmid[4], IPC_RMID, NULL);
        err_msg("shmat y");
    }
    z = (int *)shmat(shmid[5], NULL, 0);
    if (z == (int *) - 1) {
        shmctl(shmid[5], IPC_RMID, NULL);
        err_msg("shmat z");
    }

    srand(10);
    // 実行
    for(i = 0; i < MakerNum && (pid[i] = fork()) > 0; i++);
    if (i == MakerNum) { for(i = 0; i < MakerNum; i++) { waitpid(pid[i], &status, 0); } }
    else {
        while(*x < ProductMaxNum || *y < ProductMaxNum || *z < ProductMaxNum){
            prod_num = rand() % 10 + 1;
            switch(i) {
                // A
                case 0:
                    A(prod_num, prod_time[0], prod_after_time[0]);
                    break;
                // B
                case 1:
                    B(prod_num, prod_time[1], prod_after_time[1]);
                    break;
                // C
                case 2:
                    C(prod_num, prod_time[2], prod_after_time[2]);
                    break;
                // X
                case 3:
                    X(prod_time[3], prod_after_time[3]);
                    break;
                // Y
                case 4:
                    Y(prod_time[4], prod_after_time[4]);
                    break;
                // Z
                case 5:
                    Z(prod_time[5], prod_after_time[5]);
                    break;
                default:
                    break;
            }
        }
        if (pid[i] == 0) { wait(&status); }
    }

    // 共有メモリの開放
    for (i = 0; i < MakerNum; i++) { shmctl(shmid[i], IPC_RMID, NULL); }
    printf("x:%d y:%d z:%d\n", *x, *y, *z);

}
