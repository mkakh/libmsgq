#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "../inc/mylib.h"
#include "../inc/libmsgq.h"

/**************************************************
 * 関数名: msgq_close
 * 引数  : msgq_t *msgQue  メッセージキュー構造体
 * 内容  : メッセージキューを閉じて削除する
 * 作成日: 2018/09/01
 **************************************************/
void msgq_close(msgq_t *msgQue) {
    mqd_t mqd = msgQue->mqd;
    const char *name = msgQue->name;
    char buf[MAX_QUEUE_NAME_SIZE+1] = {0};
    snprintf(buf, MAX_QUEUE_NAME_SIZE, "/%s", name);
    mq_close(mqd);
    mq_unlink(buf);
}

/**************************************************
 * 関数名: msgq_open
 * 引数  : const char *name  キューの名前
 *         int oflag  オープン時のフラグ（0の場合，O_RDWRになる）
 * 内容  : メッセージキューを開く（存在しない場合は新規作成）
 * 作成日: 2018/09/08
 **************************************************/
msgq_t *msgq_open(const char *name, int oflag) {
    msgq_t *tmp = calloc(1, sizeof(msgq_t));
    char buf[MAX_QUEUE_NAME_SIZE+1] = {0};
    snprintf(buf, MAX_QUEUE_NAME_SIZE, "/%s", name);
    if (oflag == 0)
        oflag = O_RDWR;
    if ((tmp->mqd = mq_open(buf, oflag | O_CREAT , FILE_MODE, NULL)) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }
    tmp->name = name;
    return tmp;
}

/**************************************************
 * 関数名: msgq_receive
 * 引数  : msgq_t  メッセージキュー構造体
 *         ULONG *msg_ptr  メッセージの格納先（サイズはULONG*4）
 *         size_t msg_len  メッセージ格納先のサイズ
 *         unsigned *msg_prio プライオリティ
 * 内容  : メッセージを受け取る
 * 作成日: 2018/09/01
 **************************************************/
int msgq_receive(msgq_t *msgQueue, ULONG *msg_ptr, size_t msg_len, unsigned int *msg_prio) {
    ULONG *ulBuf;
    struct mq_attr attr;
    char buf[MAX_QUEUE_NAME_SIZE+1] = {0};
    mqd_t mqd;

    snprintf(buf, MAX_QUEUE_NAME_SIZE, "/%s", msgQueue->name);

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
