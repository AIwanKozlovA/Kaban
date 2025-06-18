#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AES.h"
/*
// Перевод текста в блоки по 128 бит
void text_to_blocks(const char *text, unsigned short blocks[][16]) {
    int i, j;
    int text_len = strlen(text);
    int block_count = (text_len + 15) / 16; // Округление вверх

    // Заполняем блоки 128-битными данными
    for (i = 0; i < block_count; i++) {
        for (j = 0; j < 16; j+=2) {
            if (i * 16 + j < text_len) {
                unsigned short ll;
                unsigned char low = text[i * 16 + j];
                unsigned char high = text[i * 16 + j+1];
                unsigned short s = (high << 8) | low;
                printf("s = %02X ", s);
                blocks[i][j] = s;//text[i * 16 + j];
            } else {
                blocks[i][j] = 0; // Заполнение нулями
            }
        }
    }
}
*/
unsigned short hex_pair_to_short(const char* s) {
    unsigned short val = 0;
    for(int i = 0; i < 4; i++) {
        val <<= 4;
        val |= (s[i] <= '9') ? (s[i] - '0') : (s[i] - 'a' + 10);
    }
    return val;
}
void text_to_block(char* text, unsigned short key[]){ // Перевод текста в блоки по 256 бит
    for(int i = 0; i < 16;i++){
        if(strlen(text) >= 4*i && strlen(text) - 4*i >= 4){
            key[i] = hex_pair_to_short(text+4*i);
        }else if(strlen(text) % 4 != 0 && strlen(text) - 4*i < 4 ){
            key[i] = hex_pair_to_short(text+ (4*i - strlen(text)));
        }else{
            key[i] = 0;
        }
    }
}


// Перевод блоков по 128 бит в текст
void blocks_to_text(const unsigned short blocks[][16], int block_count, char *text) {
    int i, j;
    int text_len = block_count * 16; // Длина текста

    // Создание текста из блоков
    for (i = 0; i < block_count; i++) {
        for (j = 0; j < 16; j++) {
            if (i * 16 + j < text_len) {
                text[i * 16 + j] = blocks[i][j];
            }
        }
    }
    text[text_len] = '\0'; // Добавляем нулевой символ
}

int schid_s_klav(char *string,size_t* buffer_size, size_t* string_length){

    // Выделяем память для строки
    if (string == NULL) {
        fprintf(stderr, "Ошибка: Не удалось выделить память.\n");
        return 1;
    }

    // Считываем строку с клавиатуры
    printf("Введите строку: ");
    while (1) {
        char c = getchar();
        if (c == '\n') {
            break;
        }
        string[*string_length] = c;
        (*string_length)++;
        // Если размер буфера недостаточен, увеличиваем его
        if (*string_length == *buffer_size) {
            *buffer_size *= 2;
            string = realloc(string, *buffer_size);
            if (string == NULL) {
                fprintf(stderr, "Ошибка: Не удалось перевыделить память.\n");
                return 1;
            }
        }
    }
    string[*string_length] = '\0'; // Добавляем нулевой символ в конец строки
    return 0;
}
