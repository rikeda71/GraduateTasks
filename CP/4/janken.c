#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define genrnd(mn, mx) (rand() % ((mx)-(mn)+1) + (mn))


char rsp[3][10] = {"rock", "scissors", "paper"};
int judge_val[3] = {9, 2, 4};

void janken_cpu(int *janken) {
    srand((unsigned)time(NULL) * pthread_self());
    *janken = genrnd(0, 2);
}

int ctoi(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    return 0;
}

int janken_judge(int *janken, int n) {
    int i;
    int judge = 0;
    int shiftjudge;
    int result; // 0: next battle, 1: end battle

    // ビット演算でどの手が出されたかを調査
    for (i = 0; i < n; i++) {
        judge |= judge_val[janken[i]];
    }
    // 格納したビットと1つ右シフトしたビットを＆演算
    judge &= judge >> 1;
    if (judge == 7 || judge == 0) {
        result = 1;
        printf("引き分けです\n");
        printf("player:%s\n", rsp[janken[0]]);
        for (i = 1; i < n; i++) {
            printf("cpu%d:%s\n", i, rsp[janken[i]]);
        }
    }
    else {
        result = 0;
        judge = judge == 1 ? 9 : judge;
        if (judge_val[janken[0]] == judge) {
            printf("%sでplayerは勝ちです\n", rsp[janken[0]]);
        }
        else {
            printf("%sでplayerは負けです\n", rsp[janken[0]]);
        }
        for(i = 1; i < n; i++) {
            if (judge_val[janken[i]] == judge) {
                printf("%sでcpu%dは勝ちです\n", rsp[janken[i]], i);
            }
            else {
                printf("%sでcpu%dは負けです\n", rsp[janken[i]], i);
            }
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    int i, j;
    int flag = 1;
    int n;
    pthread_t *threads;
    int status;
    int true_janken;
    int *janken_array;
    char c;

    if (argc < 2) {
        exit(1);
    }
    // じゃんけんをする人数を用意
    n = atoi(argv[1]);
    if (n < 1) {
        fprintf(stderr, "argv[1] > 1\n");
        exit(1);
    }
    n++;
    threads = malloc(sizeof(pthread_t) * (n - 1));
    janken_array = malloc(sizeof(int) * n);

    while(flag) {
        while(1) {
            printf("jankenの手を決めろ\n0: rock, 1: scissor, 2: paper -> ");
            scanf("%c", &c);
            if(isdigit(c) > 0 && ctoi(c) < 3 && ctoi(c) > -1) break;
            else { printf("0,1,2のいずれかを入力せよ\n"); }
        }
        getchar();
        janken_array[0] = ctoi(c);

        for (i = 1; i < n; i++) {
            pthread_create(&threads[i - 1], NULL, (void *)janken_cpu, &(janken_array[i]));
        }
        for (i = 0; i < n - 1; i++) {
            pthread_join(threads[i], (void **)&status);
        }
        flag = janken_judge(janken_array, n);
    }
    pthread_exit(0);
    return 0;

}
