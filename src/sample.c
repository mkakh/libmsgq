#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../inc/lib.h"
#include "../inc/msg_queue.h"
#define MAX_THREAD 8

void *subthread(void *vId) {
    char *stId = (char *)vId;
    ULONG msg[4] = {0};
    MSG_QUE_READ(stId, msg, sizeof(msg), 0);
    printf("Thread: %s\n", (char *)msg[0]);
    return NULL;
}

int main(void) {
    mqd_t mqd[MAX_THREAD];
    char *msgQueId[MAX_THREAD];
    ULONG msg[4] = {0};
    pthread_t tid[MAX_THREAD];

    for (int i = 0; i < MAX_THREAD; i++) {
        msgQueId[i] = char_to_string('0' + i);
    }

    for (int i = 0; i < MAX_THREAD; i++) {
        if ((mqd[i] = MSG_QUE_CREATE(msgQueId[i])) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
        pthread_create(&tid[i], NULL, subthread, msgQueId[i]);
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        char *str = msgQueId[i];
        msg[0] = (ULONG)str;
        if (MSG_QUE_SEND(mqd[i], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        MSG_QUE_CLOSE(&mqd[i], msgQueId[i]);
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        pthread_join(tid[i], NULL);
    }
    return EXIT_SUCCESS;
}
