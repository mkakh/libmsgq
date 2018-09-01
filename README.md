# メッセージキュー

メッセージキューの自作ライブラリ（予定）です．

mqueueを使っています．

|名前                 | 用途                |
|:--------------------|:--------------------|
|MSG_QUE_CREATE(const char *name) |指定された名前のメッセージキューを作成．|
|MSG_QUE_SEND(mqd_t mqd, const unsigned long *msg_ptr, size_t msg_len, unsigned int prio)| メッセージを送信する．|
|MSG_QUE_READ(const char *name, unsigned long *msg_ptr, size_t msg_len, unsigned int *msg_prio) | メッセージを受け取ってmsg_ptrに格納する．|
|MSG_QUE_CLOSE(mqd_t *mqd, const char *name) | メッセージキューを閉じて削除する．|
