#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../inc/lib.h"
#include "../inc/msg_queue.h"

void *subthread(void *vId) {
    char *stId = (char *)vId;
    ULONG msg[4] = {0};
    MSG_QUE_READ(stId, msg, sizeof(msg), 0);
    printf("%s\n", (char *)msg[0]);
    return NULL;
}

int main(void) {
    mqd_t mqd;
    ULONG msg[4] = {0};
    pthread_t tid;

    if ((mqd = MSG_QUE_CREATE("hoge")) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }
    pthread_create(&tid, NULL, subthread, "hoge");

    char *str = "OK";
    msg[0] = (ULONG)str;

    if (MSG_QUE_SEND(mqd, msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }

    sleep(1);
    MSG_QUE_CLOSE(&mqd, "hoge");
    return EXIT_SUCCESS;
}
