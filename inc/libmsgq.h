#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H
#include <mqueue.h>

/**************************************************
 * マクロ
 **************************************************/
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define msgq_send(a, b, c, d) mq_send(a->mqd, (void *)b, c, d)
#define MAX_QUEUE_NAME_SIZE 1000

/**************************************************
 * メッセージキュー構造体
 **************************************************/
typedef struct {
    mqd_t mqd;
    const char *name;
} msgq_t;

/**************************************************
 * 関数
 **************************************************/

/**************************************************
 * 関数名: msgq_close
 * 引数  : MSG_QUEUE_T *msgQue  メッセージキュー構造体
 * 内容  : メッセージキューを閉じて削除する
 * 作成日: 2018/09/01
 **************************************************/
void msgq_close(MSG_QUEUE_T *msgQue);

/**************************************************
 * 関数名: msgq_open
 * 引数  : const char *name  キューの名前
 *         int oflag  オープン時のフラグ（0の場合，O_RDWRになる）
 * 内容  : メッセージキューを開く（存在しない場合は新規作成）
 * 作成日: 2018/09/08
 **************************************************/
MSG_QUEUE_T *msgq_open(const char *name, int oflag);

/**************************************************
 * 関数名: msgq_receive
 * 引数  : MSG_QUEUE_T  メッセージキュー構造体
 *         ULONG *msg_ptr  メッセージの格納先（サイズはULONG*4）
 *         size_t msg_len  メッセージ格納先のサイズ
 *         unsigned *msg_prio プライオリティ
 * 内容  : メッセージを受け取る
 * 作成日: 2018/09/01
 **************************************************/
int msgq_receive(MSG_QUEUE_T *msgQueue, ULONG *msg_ptr, size_t msg_len, unsigned int *msg_prio);

#endif /* MSG_QUEUE_H */
