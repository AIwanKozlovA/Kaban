#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AES.h"

/*// Перевод текста в блоки по 256 бит
void dl_text_to_blocks(const char *text, unsigned short blocks[][16], int* block_count){
    int i, j, dl = 0;
    int text_len = strlen(text);
    *block_count = (text_len + 15) / 16; // Округление вверх
    //printf("%d\n", *block_count);
    // Заполняем блоки 128-битными данными
    for (i = 0; i < block_count; i++) {
        for (j = 0; j < 16; j+=2) {
            if (i * 16 + j < text_len) {
                unsigned short ll;
                unsigned char low = text[dl];
                unsigned char high = text[dl +1];
                dl++;
                unsigned short s = (high << 8) | low;
                printf("s = %02X ", s);
                blocks[i][j] = s;//text[i * 16 + j];
            } else {
                blocks[i][j] = 0; // Заполнение нулями
            }
        }
    }
}*/
void dl_text_to_blocks(const char *text, unsigned short blocks[][16], int* block_count) {
    int i, j, dl = 0;
    int text_len = strlen(text);
    *block_count = (text_len + 15) / 16; // Округление вверх

    // Обнуляем все блоки
    memset(blocks, 0, (*block_count) * 16 * sizeof(unsigned short));

    // Заполняем блоки 256-битными данными (16 unsigned short по 16 бит)
    for (i = 0; i < *block_count; i++) {
        for (j = 0; j < 16; j++) {
            if (dl < text_len) {
                // Берем два символа (16 бит) и формируем одно unsigned short
                unsigned char low = text[dl++];
                unsigned char high = (dl < text_len) ? text[dl++] : 0;
                blocks[i][j] = (high << 8) | low;
                //printf("block[%d][%d] = %04X ", i, j, blocks[i][j]);
            }
        }
    }
}

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


// Перевод блоков по 256 бит в текст
void blocks_to_text(const unsigned short blocks[][16], int block_count, char *text) {
    int i, j, kol = 0;
    int text_len = block_count * 16; // Длина текста
    // Создание текста из блоков
    for (i = 0; i < block_count; i++) {
        for (j = 0; j < 16; j++) {
            if (i * 16 + j < text_len) {
                unsigned char low = blocks[i][j] & 0xFF;
                unsigned char high = (blocks[i][j] >> 8) & 0xFF;
                text[kol] = low;
                text[kol + 1] = high;
                kol += 2;
            }
        }
    }
    text[text_len] = '\0'; // Добавляем нулевой символ
}
void Sfif_AES_text(char* original_text,unsigned short matrix_shif[COUNT_BLOCKS_FOR_TEXT][16], int* block_count, char* secret_key){ // шифрование текста
    unsigned short secret_key_block[16];
    text_to_block(secret_key, secret_key_block);
    unsigned short matrix[COUNT_BLOCKS_FOR_TEXT][16];
    dl_text_to_blocks(original_text, matrix, block_count);
    //print_all_block(matrix, *block_count);
    for(int i = 0; i < *block_count; i++){
        cryption(matrix[i], matrix_shif[i], secret_key_block);
    }
}

void Rasfif_AES_text(char*rashif_text, unsigned short matrix_shif[COUNT_BLOCKS_FOR_TEXT][16], int* block_count, char* secret_key){ // расшифровка текста
    unsigned short secret_key_block[16];
    text_to_block(secret_key, secret_key_block);
    unsigned short matrix_rash[COUNT_BLOCKS_FOR_TEXT][16];
    for(int i = 0; i < *block_count; i++){
        decryption(matrix_shif[i],matrix_rash[i], secret_key_block);
    }
    blocks_to_text(matrix_rash, *block_count, rashif_text);
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
