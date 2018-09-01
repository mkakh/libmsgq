#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "../inc/lib.h"
#include "../inc/msg_queue.h"

/**************************************************
 * 関数名: mq_close_wrapper
 * 引数  : mqd_t *mqd        msgQueueのID
 *         const char *name  msgQueueの名前
 * 内容  : メッセージキューを閉じて削除する
 * 作成日: 2018/09/01
 **************************************************/
void mq_close_wrapper(mqd_t *mqd, const char *name) {
    char buf[MAX_QUEUE_NAME_SIZE+1] = {0};
    snprintf(buf, MAX_QUEUE_NAME_SIZE, "/que_%s", name);
    mq_close(*mqd);
    mq_unlink(buf);
}

/**************************************************
 * 関数名: mq_open_wrapper
 * 引数  : const char *name  キューの名前
 * 内容  : メッセージキューを新規作成する
 * 作成日: 2018/09/01
 **************************************************/
mqd_t mq_open_wrapper(const char *name) {
    char buf[MAX_QUEUE_NAME_SIZE+1] = {0};
    mqd_t mqd;
    snprintf(buf, MAX_QUEUE_NAME_SIZE, "/que_%s", name);
    mq_unlink(name);

    mqd = mq_open(buf, O_RDWR | O_CREAT , FILE_MODE, NULL);
    return mqd;
}

/**************************************************
 * 関数名: mq_receive_wrapper
 * 引数  : const char *name  キューの名前
 *         ULONG *msg_ptr  メッセージの格納先（サイズはULONG*4）
 *         size_t msg_len  メッセージ格納先のサイズ
 *         unsigned *msg_prio プライオリティ
 * 内容  : メッセージを受け取る
 * 作成日: 2018/09/01
 **************************************************/
int mq_receive_wrapper(const char *name, ULONG *msg_ptr, size_t msg_len, unsigned *msg_prio) {
    mqd_t mqd;
    ULONG *ulBuf;

    char buf[MAX_QUEUE_NAME_SIZE+1] = {0};
    snprintf(buf, MAX_QUEUE_NAME_SIZE, "/que_%s", name);
    if ((mqd = mq_open(buf, O_RDONLY)) == -1) {
        PERROR(__FUNCTION__);
        return -1;
    }
    struct mq_attr attr;
    mq_getattr(mqd, &attr);
    ulBuf = calloc(1, attr.mq_msgsize);

    if (mq_receive(mqd, (char *)ulBuf, attr.mq_msgsize, 0) == -1) {
        PERROR(__FUNCTION__);
        return -1;
    }
    mq_close(mqd);
    memcpy(msg_ptr, ulBuf, msg_len);
    return 0;
}
