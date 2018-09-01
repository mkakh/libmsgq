#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../../inc/lib.h"
#include "../../inc/msg_queue.h"
#define MAX_THREAD 2
#define N 20

int data[N] = {100, 21, 450, 29, 30, 500, 20, 60, 80, 100, 60, 24, 540, 10, 102, -193, 4, 0, -123, 84};
int changed = 0;
static void swap(int *a, int *b) {
    int tmp = *b;
    *b = *a;
    *a = tmp;
}
static void show_data() {
    for (int i = 0; i < N; i++) {
        printf("%d\n", data[i]);
    }
}

static void *subthread(void *vMsgQue) {
    MSG_QUEUE_T *msgQue = (MSG_QUEUE_T *)vMsgQue;
    ULONG msg[4] = {0};

    /* 初期化 */
    int init = 0;
    if (msgQue->name[0] == '1')
        init = 1;
    if (MSG_QUE_SEND(msgQue, msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }

    /* 終了メッセージが来るまでループ */
    while (msg[3] != 1) {
        msg[init] = 0;
        if ((MSG_QUE_READ(msgQue, msg, sizeof(msg), 0)) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
        for (int i = init; i < N-1; i+=2) {
            if (data[i] > data[i+1]) {
                swap(&data[i], &data[i+1]);
                changed = 1;
            }
        }
        /* 処理が終わったことをmain関数へ伝える */
        MSG_QUE_SEND(msgQue, msg, sizeof(msg), 0);
    }
    return NULL;
}

int main(void) {
    MSG_QUEUE_T *msgQueue[MAX_THREAD];
    ULONG msg[4] = {0};
    char *tmp[MAX_THREAD] = {"0", "1"};
    pthread_t tid[MAX_THREAD];

    /* スレッド＆メッセージキュー作成 */
    for (int i = 0; i < MAX_THREAD; i++) {
        msgQueue[i] = MSG_QUE_CREATE(tmp[i]);
        pthread_create(&tid[i], NULL, subthread, msgQueue[i]);
    }

    /* 初期化待ち */
    if (MSG_QUE_READ(msgQueue[0], msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }
    if (MSG_QUE_READ(msgQueue[1], msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }

    /* メイン処理 */
    do {
        changed = 0;
        if (MSG_QUE_SEND(msgQueue[0], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
        if (MSG_QUE_SEND(msgQueue[1], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
        if (MSG_QUE_READ(msgQueue[0], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
        if (MSG_QUE_READ(msgQueue[1], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
    } while (changed);

    /* 終了メッセージ送信 */
    msg[3] = 1;
    if (MSG_QUE_SEND(msgQueue[0], msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }
    if (MSG_QUE_SEND(msgQueue[1], msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }

    /* 結果表示 */
    show_data();

    /* 終了処理 */
    for (int i = 0; i < MAX_THREAD; i++) {
        pthread_join(tid[i], NULL);
        MSG_QUE_CLOSE(msgQueue[i]);
    }
    return EXIT_SUCCESS;
}
