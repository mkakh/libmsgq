#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H
#include <mqueue.h>

/**************************************************
 * マクロ
 **************************************************/
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define MSG_QUE_SEND(a, b, c, d) mq_send(a->mqd, (void *)b, c, d)
#define MSG_QUE_CREATE(a, b) mq_open_wrapper(a, b)
#define MSG_QUE_READ(a, b, c, d) mq_receive_wrapper(a, b, c, d)
#define MSG_QUE_CLOSE(a) mq_close_wrapper(a)
#define MAX_QUEUE_NAME_SIZE 1000

/**************************************************
 * 構造体
 **************************************************/
typedef struct {
    mqd_t mqd;
    const char *name;
} MSG_QUEUE_T;

/**************************************************
 * 関数
 **************************************************/

/**************************************************
 * 関数名: mq_close_wrapper
 * 引数  : MSG_QUEUE_T *msgQue  メッセージキュー構造体
 * 内容  : メッセージキューを閉じて削除する
 * 作成日: 2018/09/01
 **************************************************/
void mq_close_wrapper(MSG_QUEUE_T *msgQue);

/**************************************************
 * 関数名: mq_open_wrapper
 * 引数  : const char *name  キューの名前
 *         int oflag  オープン時のフラグ(O_RDWR等)
 * 内容  : メッセージキューを新規作成する
 * 作成日: 2018/09/01
 **************************************************/
MSG_QUEUE_T *mq_open_wrapper(const char *name, int oflag);

/**************************************************
 * 関数名: mq_receive_wrapper
 * 引数  : MSG_QUEUE_T  メッセージキュー構造体
 *         ULONG *msg_ptr  メッセージの格納先（サイズはULONG*4）
 *         size_t msg_len  メッセージ格納先のサイズ
 *         unsigned *msg_prio プライオリティ
 * 内容  : メッセージを受け取る
 * 作成日: 2018/09/01
 **************************************************/
int mq_receive_wrapper(MSG_QUEUE_T *msgQueue, ULONG *msg_ptr, size_t msg_len, unsigned int *msg_prio);

#endif /* MSG_QUEUE_H */
