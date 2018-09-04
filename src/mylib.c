#include <stdio.h>
#include <stdlib.h>

/**************************************************
 * 関数名: char_to_string
 * 引数  : char ch    変換したい文字
 * 内容  : char型変数をchar *型に変換する
 * 作成日: 2018/09/01
 **************************************************/
char *char_to_string(char ch) {
    /* メモリ領域の解放をしていないためメモリリークが起こる可能性がある */
    char *str = calloc(1, 2);
    str[0] = ch;
    return str;
}
