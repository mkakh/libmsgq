#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H
#include <mqueue.h>

/**************************************************
 * マクロ
 **************************************************/
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define MSG_QUE_SEND(a, b, c, d) mq_send(a, (void *)b, c, d)
#define MSG_QUE_CREATE(a) mq_open_wrapper(a)
#define MSG_QUE_READ(a, b, c, d) mq_receive_wrapper(a, b, c, d)
#define MSG_QUE_CLOSE(a, b) mq_close_wrapper(a, b)
#define MAX_QUEUE_NAME_SIZE 1000

/**************************************************
 * 関数
 **************************************************/

/**************************************************
 * 関数名: mq_close_wrapper
 * 引数  : mqd_t *mqd        msgQueueのID
 *         const char *name  msgQueueの名前
 * 内容  : メッセージキューを閉じて削除する
 * 作成日: 2018/09/01
 **************************************************/
void mq_close_wrapper(mqd_t *mqd, const char *name);

/**************************************************
 * 関数名: mq_open_wrapper
 * 引数  : const char *name  キューの名前
 * 内容  : メッセージキューを新規作成する
 * 作成日: 2018/09/01
 **************************************************/
mqd_t mq_open_wrapper(const char *name);

/**************************************************
 * 関数名: mq_receive_wrapper
 * 引数  : const char *name  キューの名前
 *         ULONG *msg_ptr  メッセージの格納先（サイズはULONG*4）
 *         size_t msg_len  メッセージ格納先のサイズ
 *         unsigned *msg_prio プライオリティ
 * 内容  : メッセージを受け取る
 * 作成日: 2018/09/01
 **************************************************/
int mq_receive_wrapper(const char *name, ULONG *msg_ptr, size_t msg_len, unsigned int *msg_prio);

#endif /* MSG_QUEUE_H */
