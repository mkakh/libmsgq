#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../../inc/lib.h"
#include "../../inc/msg_queue.h"
#define MAX_THREAD 2

typedef struct {
    ULONG num;
    long *data;
} DATA_T;

DATA_T *data;
int changed = 0;


/**************************************************
 * 関数名: read_data
 * 引数  : char *filename ファイル名
 * 内容  : ファイルからデータを読み込む
 * 作成日: 2018/09/02
 **************************************************/
static DATA_T *read_data(const char *filename) {
    FILE *fp;
    DATA_T *data = calloc(1, sizeof(DATA_T));
    const int buf_size = 200;
    char buf[buf_size];
    ULONG ct;

    if ((fp = fopen(filename, "r")) == NULL) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }

    /* 行数を数える */
    data->num = 0;
    while (fgets(buf, buf_size, fp) != NULL) {
        data->num++;
    }

    /* 読み込み位置を初期化 */
    rewind(fp);

    /* データ取得 */
    data->data = calloc(data->num, sizeof(long));
    ct = 0;
    while (fgets(buf, buf_size, fp) != NULL) {
        data->data[ct] = atol(buf);
        ct++;
    }
    fclose(fp);
    return data;
}

/**************************************************
 * 関数名: swap
 * 引数  : long *a  入れ替えたい数値
 *         long *b  入れ替えたい数値
 * 内容  : swapする
 * 作成日: 2018/09/02
 **************************************************/
static void swap(long *a, long *b) {
    long tmp = *b;
    *b = *a;
    *a = tmp;
}

/**************************************************
 * 関数名: show_data
 * 引数  : なし
 * 内容  : データを表示
 * 作成日: 2018/09/02
 **************************************************/
static void show_data() {
    for (ULONG i = 0; i < data->num; i++) {
        printf("%ld\n", data->data[i]);
    }
}

/**************************************************
 * 関数名: subthread
 * 引数  : void *vMsgQue  メッセージキュー構造体
 * 内容  : ソートのメイン処理
 * 作成日: 2018/09/02
 * 備考  : 現在ここにバグがあり，うまく動いていない
 **************************************************/
static void *subthread(void *vMsgQue) {
    MSG_QUEUE_T *msgQue = (MSG_QUEUE_T *)vMsgQue;
    ULONG msg[4] = {0};

    /* 初期化 */
    int init = 0;
    if (msgQue->name[0] == '1')
        init = 1;
    if (MSG_QUE_SEND(msgQue, msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }

    /* 終了メッセージが来るまでループ */
    while (msg[3] != 1) {
        msg[init] = 0;
        if ((MSG_QUE_READ(msgQue, msg, sizeof(msg), 0)) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
        for (ULONG i = init; i < data->num-1; i+=2) {
            if (data->data[i] > data->data[i+1]) {
                swap(&data->data[i], &data->data[i+1]);
                changed = 1;
            }
        }
        /* 処理が終わったことをmain関数へ伝える */
        MSG_QUE_SEND(msgQue, msg, sizeof(msg), 0);
    }
    return NULL;
}

/**************************************************
 * 関数名: main
 * 引数  : int argc       引数の数
 *         char *argv[]   引数の中身（データファイルのPATH）
 * 内容  : メイン関数
 * 作成日: 2018/09/02
 **************************************************/
int main(int argc, char *argv[]) {
    MSG_QUEUE_T *msgQueue[MAX_THREAD];
    ULONG msg[4] = {0};
    char *tmp[MAX_THREAD] = {"0", "1"};
    pthread_t tid[MAX_THREAD];

    /* 引数チェック */
    if (argc != 2) {
        printf("Usage: %s [filename]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* データの読み込み */
    data = read_data(argv[1]);

    /* スレッド＆メッセージキュー作成 */
    for (int i = 0; i < MAX_THREAD; i++) {
        msgQueue[i] = MSG_QUE_CREATE(tmp[i]);
        pthread_create(&tid[i], NULL, subthread, msgQueue[i]);
    }

    /* 初期化待ち */
    if (MSG_QUE_READ(msgQueue[0], msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }
    if (MSG_QUE_READ(msgQueue[1], msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }

    /* メイン処理 */
    do {
        changed = 0;
        if (MSG_QUE_SEND(msgQueue[0], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
        if (MSG_QUE_SEND(msgQueue[1], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
        if (MSG_QUE_READ(msgQueue[0], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
        if (MSG_QUE_READ(msgQueue[1], msg, sizeof(msg), 0) == -1) {
            PERROR(__FUNCTION__);
            exit(EXIT_FAILURE);
        }
    } while (changed);

    /* 終了メッセージ送信 */
    msg[3] = 1;
    if (MSG_QUE_SEND(msgQueue[0], msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }
    if (MSG_QUE_SEND(msgQueue[1], msg, sizeof(msg), 0) == -1) {
        PERROR(__FUNCTION__);
        exit(EXIT_FAILURE);
    }

    /* 結果表示 */
    show_data();

    /* 終了処理 */
    for (int i = 0; i < MAX_THREAD; i++) {
        pthread_join(tid[i], NULL);
        MSG_QUE_CLOSE(msgQueue[i]);
    }
    return EXIT_SUCCESS;
}
