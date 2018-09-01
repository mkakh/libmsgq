# メッセージキュー

メッセージキューの自作ライブラリ（予定）です．

mqueueを使っています．

構造体：MSG_QUEUE_T

|メンバ名|用途|
|:------|:-------|
|mqd_t mqd | mqueueの構造体|
|const char *name | メッセージキューの名前 |

|関数名                 | 用途                |
|:--------------------|:--------------------|
|MSG_QUE_CREATE(const char *name) |指定された名前のメッセージキューを作成．|
|MSG_QUE_SEND(MSG_QUEUE_T *msgQueue, const unsigned long *msg_ptr, size_t msg_len, unsigned int prio)| メッセージを送信する．|
|MSG_QUE_READ(MSG_QUEUE_T *msgQueue, unsigned long *msg_ptr, size_t msg_len, unsigned int *msg_prio) | メッセージを受け取ってmsg_ptrに格納する．|
|MSG_QUE_CLOSE(MSG_QUEUE_T *msgQueue) | メッセージキューを閉じて削除する．|
