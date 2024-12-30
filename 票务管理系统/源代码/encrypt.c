#include "head.h"

#ifndef stv_encrypt_rot13
#define stv_encrypt_rot13

char* encrypt(const char str[]) {
    const int len = strlen(str);
    char* encrypted = (char*)malloc((len + 1) * sizeof(char)); // 动态分配内存
    if (encrypted == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            encrypted[i] = ((str[i] - 'a' + 13) % 26) + 'a';
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            encrypted[i] = ((str[i] - 'A' + 13) % 26) + 'A';
        } else {
            encrypted[i] = str[i]; // 非字母字符保持不变
        }
    }
    encrypted[len] = '\0'; // 添加字符串结束符
    return encrypted;
}

char* decrypt(const char str[]) {
    const int len = strlen(str);
    char* decrypted = (char*)malloc((len + 1) * sizeof(char)); // 动态分配内存
    if (decrypted == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            decrypted[i] = ((str[i] - 'a' - 13 + 26) % 26) + 'a';
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            decrypted[i] = ((str[i] - 'A' - 13 + 26) % 26) + 'A';
        } else {
            decrypted[i] = str[i]; // 非字母字符保持不变
        }
    }
    decrypted[len] = '\0'; // 添加字符串结束符
    return decrypted;
}

// 别看了，问就是网上找的 rot13 算法
//就是凯撒加密的变体
#endif

#ifndef stv_num_alpha
#define stv_num_alpha

int alpha_num_check(const char *str) {
    while (*str) {
        // 如果字符不是英文字符或数字，返回0表示不合法
        if (!isalnum(*str)) {
            return 0;
        }
        str++;
    }
    return 1;  // 字符串合法，只包含英文和数字
}

#endif // stv_num_alpha
