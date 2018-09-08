# libmsgq

メッセージキューの静的ラッパーライブラリです．

構造体：msgq_t

|メンバ名|用途|
|:------|:-------|
|mqd_t mqd | mqueueの構造体|
|const char *name | メッセージキューの名前 |

|関数名                 | 用途                |
|:--------------------|:--------------------|
|msgq_open(const char *name, int oflag) |指定された名前のメッセージキューを開く．（存在しない場合は新規作成）|
|msgq_send(msgq_t *msgQueue, const unsigned long *msg_ptr, size_t msg_len, unsigned int prio)| メッセージを送信する．|
|msgq_receive(msgq_t *msgQueue, unsigned long *msg_ptr, size_t msg_len, unsigned int *msg_prio) | メッセージを受け取ってmsg_ptrに格納する．|
|msgq_close(msgq_t *msgQueue) | メッセージキューを閉じて削除する．|
