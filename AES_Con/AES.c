#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "AES.h"

// Матрица S-box для SubBytes
unsigned short sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};


// Обратный S-box для InvSubBytes
unsigned short inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};


// SubBytes преобразование
void SubBytes(unsigned short state[16]) {
    for (int i = 0; i < 16; i++) {
        unsigned char low = state[i] & 0xFF;
        unsigned char high = (state[i] >> 8) & 0xFF;
        //printf("%02x ", (unsigned char) high);
        //printf("%02x ", (unsigned char) state[i]);
        unsigned short s = (sbox[high] << 8) | sbox[low];
        state[i] = s;
    }
}

// InvSubBytes преобразование
void InvSubBytes(unsigned short state[16]) {
    for (int i = 0; i < 16; i++) {
        unsigned char low = state[i] & 0xFF;
        unsigned char high = (state[i] >> 8) & 0xFF;
        //printf("%02x ", (unsigned char) high);
        //printf("%02x ", (unsigned char) state[i]);
        unsigned short s = (inv_sbox[high] << 8) | inv_sbox[low];
        state[i] = s;
    }
}


// ShiftRows
void ShiftRows(unsigned short matrix[16]){
    unsigned short lok;
    lok = matrix[4];
    matrix[4] = matrix[5];
    matrix[5] = matrix[6];
    matrix[6] = matrix[7];
    matrix[7] = lok;


    lok = matrix[8];
    matrix[8] = matrix[10];
    matrix[10] = lok;
    lok =  matrix[9];
    matrix[9] = matrix[11];
    matrix[11] = lok;


    lok = matrix[12];
    matrix[12] = matrix[15];
    matrix[15] = lok;

    lok = matrix[15];
    matrix[15] = matrix[13];
    matrix[13] = lok;

    lok = matrix[14];
    matrix[14] = matrix[15];
    matrix[15] = lok;
}
// реализация InvShiftRows
void InvShiftRows(unsigned short matrix[16]){
    unsigned short lok;
    lok = matrix[4];
    matrix[4] = matrix[7];
    matrix[7] = lok;
    lok = matrix[5];
    matrix[5] = matrix[7];
    matrix[7] = lok;
    lok = matrix[6];
    matrix[6] = matrix[7];
    matrix[7] = lok;

    lok = matrix[8];
    matrix[8] = matrix[10];
    matrix[10] = lok;
    lok = matrix[9];
    matrix[9] = matrix[11];
    matrix[11] = lok;

    lok = matrix[13];
    matrix[13] = matrix[14];
    matrix[14] = lok;

    lok = matrix[14];
    matrix[14] = matrix[15];
    matrix[15] = lok;

    lok = matrix[12];
    matrix[12] = matrix[15];
    matrix[15] = lok;
}
// Вывести весь блок
void print_all_block(unsigned short blocks[16], int block_count){
    //for (int i = 0; i < block_count; i++) {
        for (int j = 0; j < 16; j++) {
            printf("%02hhx ", blocks[j]);
        }
        printf("\n");
    //}
}




unsigned short galois_multiplication(unsigned short a, unsigned short b){
    unsigned short p = 0;
    unsigned short counter;
    unsigned short hi_bit_set;
    for (counter = 0; counter < 8; counter++){
        if ((b & 1) == 1)
            p ^= a;
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set == 0x80)
            a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

void mixColumn(unsigned short column[4]){
    unsigned short cpy[4];
    int i;
    for (i = 0; i < 4; i++){
        cpy[i] = column[i];
    }
    column[0] = galois_multiplication(cpy[0], 2) ^
    galois_multiplication(cpy[3], 1) ^
    galois_multiplication(cpy[2], 1) ^
    galois_multiplication(cpy[1], 3);

    column[1] = galois_multiplication(cpy[1], 2) ^
    galois_multiplication(cpy[0], 1) ^
    galois_multiplication(cpy[3], 1) ^
    galois_multiplication(cpy[2], 3);

    column[2] = galois_multiplication(cpy[2], 2) ^
    galois_multiplication(cpy[1], 1) ^
    galois_multiplication(cpy[0], 1) ^
    galois_multiplication(cpy[3], 3);

    column[3] = galois_multiplication(cpy[3], 2) ^
    galois_multiplication(cpy[2], 1) ^
    galois_multiplication(cpy[1], 1) ^
    galois_multiplication(cpy[0], 3);
}

void mixColumns(unsigned short state[16]){
    int i, j;
    unsigned short column[4];
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            column[j] = state[(j * 4) + i];
        }
        mixColumn(column);
        for (j = 0; j < 4; j++){
            state[(j * 4) + i] = column[j];
        }
    }
}


void invMixColumn(unsigned short column[4]){
    unsigned short cpy[4];
    int i;
    for (i = 0; i < 4; i++){
        cpy[i] = column[i];
    }
    column[0] = galois_multiplication(cpy[0], 14) ^
    galois_multiplication(cpy[3], 9) ^
    galois_multiplication(cpy[2], 13) ^
    galois_multiplication(cpy[1], 11);
    column[1] = galois_multiplication(cpy[1], 14) ^
    galois_multiplication(cpy[0], 9) ^
    galois_multiplication(cpy[3], 13) ^
    galois_multiplication(cpy[2], 11);
    column[2] = galois_multiplication(cpy[2], 14) ^
    galois_multiplication(cpy[1], 9) ^
    galois_multiplication(cpy[0], 13) ^
    galois_multiplication(cpy[3], 11);
    column[3] = galois_multiplication(cpy[3], 14) ^
    galois_multiplication(cpy[2], 9) ^
    galois_multiplication(cpy[1], 13) ^
    galois_multiplication(cpy[0], 11);
}

void invMixColumns(unsigned short state[16]){
    int i, j;
    unsigned short column[4];
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            column[j] = state[(j * 4) + i];
        }

        invMixColumn(column);
        for (j = 0; j < 4; j++){
            state[(j * 4) + i] = column[j];
        }
    }
}
unsigned short raz(unsigned short x){ // разрезает unsigned short на два unsigned char, прогоняет их черех sbox и объединяет в unsigned short
    unsigned char low = x & 0xFF;
    unsigned char high = (x >> 8) & 0xFF;
    unsigned short s = (sbox[high] << 8) | sbox[low];
    return s;
}

void KeyExpansion(unsigned short key[16]){
    // RotWord
    key[0] = key[7] ^ raz(key[7]) ^ 0x1;
    key[4] = key[11] ^ raz(key[11]);
    key[8] = key[15] ^ raz(key[15]);
    key[12] = key[3] ^ raz(key[3]);

    key[1] ^= key[0];
    key[5] ^= key[4];
    key[9] ^= key[8];
    key[13] ^= key[12];

    key[2] ^= key[1];
    key[6] ^= key[5];
    key[10] ^= key[9];
    key[14] ^= key[13];

    key[3] ^= key[2];
    key[7] ^= key[6];
    key[11] ^= key[10];
    key[15] ^= key[14];
    //key[4] = lock;
    //SubBytes(key);
}


// шифрование
void cryption(unsigned short blocks[16], unsigned short encryption_blocks[16], unsigned short secret_key[16]) {
    //KeyExpansion(secret_key);
    //for(int i = 0; i < block_count; i++){
        KeyExpansion(secret_key);
        for(int a = 0; a < 16; a++){
            encryption_blocks[a] = blocks[a];
            //printf("%02X ", encryption_blocks[a]);

        }
        //этап SubBytes
         //printf("SubBytes\n");
        SubBytes(encryption_blocks);
         //printf("ShiftRows \n");
        ShiftRows(encryption_blocks);
        //printf("mixColumns \n");
        mixColumns(encryption_blocks);
        //printf("Ща xor будет \n");
        // этап xor
        for(int j = 0; j < 16; j++){
            encryption_blocks[j] = encryption_blocks[j] ^ secret_key[j];
            //printf("seh = %02X", secret_key[j]);
        }
        //printf("\n");
    //}
}

// расшифровка
void decryption(unsigned short encryption_blocks[16], unsigned short decryption_blocks[16], unsigned short secret_key[16]) {
    // printf("222\n");
    //KeyExpansion(secret_key);
    //for(int i = 0; i < block_count; i++){
        KeyExpansion(secret_key);
        // этап xor
        for(int j = 0; j < 16; j++){
            decryption_blocks[j] = encryption_blocks[j] ^ secret_key[j];
            //printf("se = %02X", secret_key[j]);
        }
        invMixColumns(decryption_blocks);
        InvShiftRows(decryption_blocks);
        // printf("InvSubBytes\n");
        InvSubBytes(decryption_blocks);
         //printf("\n");

        // InvSubBytes это обратная SubBytes

    //}
}

