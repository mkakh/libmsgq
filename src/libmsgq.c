#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "../inc/mylib.h"
#include "../inc/libmsgq.h"

/**************************************************
 * 関数名: mq_close_wrapper
 * 引数  : MSG_QUEUE_T *msgQue  メッセージキュー構造体
 * 内容  : メッセージキューを閉じて削除する
 * 作成日: 2018/09/01
 **************************************************/
void mq_close_wrapper(MSG_QUEUE_T *msgQue) {
    mqd_t mqd = msgQue->mqd;
    const char *name = msgQue->name;
    char buf[MAX_QUEUE_NAME_SIZE+1] = {0};
    snprintf(buf, MAX_QUEUE_NAME_SIZE, "/que_%s", name);
    mq_close(mqd);
    mq_unlink(buf);
}

/**************************************************
 * 関数名: mq_open_wrapper
 * 引数  : const char *name  キューの名前
 * 内容  : メッセージキューを新規作成する
 * 作成日: 2018/09/01
 **************************************************/
MSG_QUEUE_T *mq_open_wrapper(const char *name) {
    MSG_QUEUE_T *tmp = calloc(1, sizeof(MSG_QUEUE_T));
    char buf[MAX_QUEUE_NAME_SIZE+1] = {0};
    snprintf(buf, MAX_QUEUE_NAME_SIZE, "/que_%s", name);
    mq_unlink(buf);
    if ((tmp->mqd = mq_open(buf, O_RDWR | O_CREAT , FILE_MODE, NULL)) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }
    tmp->name = name;
    return tmp;
}

/**************************************************
 * 関数名: mq_receive_wrapper
 * 引数  : MSG_QUEUE_T  メッセージキュー構造体
 *         ULONG *msg_ptr  メッセージの格納先（サイズはULONG*4）
 *         size_t msg_len  メッセージ格納先のサイズ
 *         unsigned *msg_prio プライオリティ
 * 内容  : メッセージを受け取る
 * 作成日: 2018/09/01
 **************************************************/
int mq_receive_wrapper(MSG_QUEUE_T *msgQueue, ULONG *msg_ptr, size_t msg_len, unsigned int *msg_prio) {
    ULONG *ulBuf;
    struct mq_attr attr;
    char buf[MAX_QUEUE_NAME_SIZE+1] = {0};
    mqd_t mqd;

    snprintf(buf, MAX_QUEUE_NAME_SIZE, "/que_%s", msgQueue->name);

    if ((mqd = mq_open(buf, O_RDONLY)) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }

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