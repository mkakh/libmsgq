#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../../inc/lib.h"
//#include "../../inc/msg_queue.h"
#include "include/msg_queue.h"
#define MAX_THREAD 8

static void *subthread(void *vMsgQue) {
    MSG_QUEUE_T *msgQue = (MSG_QUEUE_T *)vMsgQue;
    ULONG msg[4] = {0};
    MSG_QUE_READ(msgQue, msg, sizeof(msg), 0);
    printf("Thread: %lu\n", msg[0]);
    return NULL;
}

int main(void) {
    MSG_QUEUE_T *msgQueue[MAX_THREAD];
    ULONG msg[4] = {0};
    char *tmp[MAX_THREAD];
    pthread_t tid[MAX_THREAD];

    for (int i = 0; i < MAX_THREAD; i++) {
        tmp[i] = char_to_string('0' + i);
    }

    for (int i = 0; i < MAX_THREAD; i++) {
        msgQueue[i] = MSG_QUE_CREATE(tmp[i]);
        pthread_create(&tid[i], NULL, subthread, msgQueue[i]);
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        msg[0] = i;
        if (MSG_QUE_SEND(msgQueue[i], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        pthread_join(tid[i], NULL);
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        MSG_QUE_CLOSE(msgQueue[i]);
    }
    return EXIT_SUCCESS;
}
