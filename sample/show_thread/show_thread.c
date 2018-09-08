#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../../inc/mylib.h"
#include "include/libmsgq.h"
#define MAX_THREAD 8

static void *subthread(void *vMsgQue) {
    msgq_t *msgQue = (msgq_t *)vMsgQue;
    ULONG msg[4] = {0};
    msgq_receive(msgQue, msg, sizeof(msg), 0);
    printf("Thread: %lu\n", msg[0]);
    return NULL;
}

int main(void) {
    msgq_t *msgQueue[MAX_THREAD];
    ULONG msg[4] = {0};
    char *tmp[MAX_THREAD];
    pthread_t tid[MAX_THREAD];

    for (int i = 0; i < MAX_THREAD; i++) {
        tmp[i] = char_to_string('0' + i);
    }

    for (int i = 0; i < MAX_THREAD; i++) {
        msgQueue[i] = msgq_open(tmp[i], 0);
        pthread_create(&tid[i], NULL, subthread, msgQueue[i]);
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        msg[0] = i;
        if (msgq_send(msgQueue[i], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        pthread_join(tid[i], NULL);
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        msgq_close(msgQueue[i]);
    }
    return EXIT_SUCCESS;
}
