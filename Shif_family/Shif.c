#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include <stddef.h>
#include <stdint.h>
#include "Shif.h"


//static wchar_t alfaw[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя0123456789!@#$%^&*()_+-=`~№;:?<>/.,{}[] ";
//dl_alfaw = 0;
const wchar_t alfaw[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя0123456789!@#$%^&*()_+-=`~№;:?<>/.,{}[] ";

size_t utf8_to_wcs(const char *src, wchar_t *dst, size_t dst_len) {
    const unsigned char *s = (const unsigned char *)src;
    size_t needed = 0;
    size_t out = 0;

    while (*s) {
        uint32_t cp;
        unsigned char c = *s;

        if (c <= 0x7F) {
            cp = c;
            s += 1;
        } else if ((c & 0xE0) == 0xC0) {
            if ((s[1] & 0xC0) != 0x80) return UTF8_ERR;
            uint32_t c1 = s[1] & 0x3F;
            cp = ((c & 0x1F) << 6) | c1;
            if (cp < 0x80) return UTF8_ERR; /* overlong */
                s += 2;
        } else if ((c & 0xF0) == 0xE0) {
            if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80) return UTF8_ERR;
            uint32_t c1 = s[1] & 0x3F;
            uint32_t c2 = s[2] & 0x3F;
            cp = ((c & 0x0F) << 12) | (c1 << 6) | c2;
            if (cp < 0x800) return UTF8_ERR; /* overlong */
                s += 3;
        } else if ((c & 0xF8) == 0xF0) {
            if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80 || (s[3] & 0xC0) != 0x80) return UTF8_ERR;
            uint32_t c1 = s[1] & 0x3F;
            uint32_t c2 = s[2] & 0x3F;
            uint32_t c3 = s[3] & 0x3F;
            cp = ((c & 0x07) << 18) | (c1 << 12) | (c2 << 6) | c3;
            if (cp < 0x10000 || cp > 0x10FFFF) return UTF8_ERR; /* overlong or out of range */
                s += 4;
        } else {
            return UTF8_ERR; /* invalid leading byte */
        }

        /* Reject surrogate code points encoded in UTF-8 */
        if (cp >= 0xD800 && cp <= 0xDFFF) return UTF8_ERR;

        if (sizeof(wchar_t) == 2) {
            if (cp <= 0xFFFF) {
                /* BMP */
                needed += 1;
                if (out + 1 < dst_len) dst[out] = (wchar_t)cp;
                out += (out + 1 < dst_len) ? 1 : 0;
            } else {
                /* surrogate pair */
                needed += 2;
                uint32_t v = cp - 0x10000;
                wchar_t high = (wchar_t)((v >> 10) + 0xD800);
                wchar_t low  = (wchar_t)((v & 0x3FF) + 0xDC00);
                if (out + 1 < dst_len) {
                    dst[out++] = high;
                    if (out < dst_len) dst[out++] = low;
                    else out--; /* in case only one slot left, don't write low */
                }
            }
        } else {
            /* wchar_t is 32-bit (likely UTF-32) */
            needed += 1;
            if (out + 1 < dst_len) dst[out++] = (wchar_t)cp;
        }
    }

    /* null-terminate if space */
    if (dst_len > 0) {
        if (out < dst_len) dst[out] = L'\0';
        else dst[dst_len - 1] = L'\0';
    }

    return needed;
}

int wchar_array_to_int(const wchar_t *arr) {
    return (int)wcstol(arr, NULL, 10);
}


void gen_key(wchar_t key[10000][100], int dl, int kol_shif,char* sid){ // генерации ключа
    int dl_alfaw = 0;
    for(int i = 0; alfaw[i] != '\0';i++) dl_alfaw += 1;
    int dl_sid = 0;
    if(sid != NULL) for(int i = 0; sid[i] != '\0';i++) dl_sid += 1;
    int chast_smen_sid = 0;
    if(sid != NULL) chast_smen_sid = dl_alfaw * kol_shif / (dl_sid/9);
    int Shet_vkin_sid = 0;
    for(int i = 0; i < dl_alfaw * kol_shif;i++){
        //printf("i = %d\n", i);
        if(sid != NULL && i % chast_smen_sid == 0 && Shet_vkin_sid*9 < dl_sid){
            char chunk[9] = {0}; // +1 для нулевого терминатора
            strncpy(chunk, sid + Shet_vkin_sid*9, 9);
            //printf("chunk %s i = %d", chunk, i);
            Shet_vkin_sid++;
            unsigned int lok_sid = atoi(chunk) ^ time(NULL);
            //printf("lok_sid = %d ", lok_sid);
            srand(lok_sid); // time(NULL)
        }
        for(int j = 0; j < dl; j++){
            key[i][j] =  alfaw[ rand() % dl_alfaw];
            printf("%lc", key[i][j]);
        }
        //printf("%s ", key[i]);
        printf(" ");
    }
    printf("\n");
}

static int dl_text = 0;
int shif_text(wchar_t text[10000],wchar_t Shif_text[100000], wchar_t key[1000][100], int dl, int kol_shif, char* sid){ // шифрование текста
    int dl_alfaw = 0;
    for(int i = 0; alfaw[i] != '\0';i++) dl_alfaw += 1;
    for(int i = 0; text[i] != '\0';i++) dl_text += 1;
    int lok = 0;
    int dl_sid = 0;
    if(sid != NULL) for(int i = 0; sid[i] != '\0';i++) dl_sid += 1;
    int chast_smen_sid = 0;
    if(sid != NULL){
        chast_smen_sid = dl_text * kol_shif / (dl_sid/9);
        if(dl_sid > dl_text) chast_smen_sid = 1;
    }
    int Shet_vkin_sid = 0;
    for(int i = 0; i < dl_text; i++){
        if(sid != NULL && Shet_vkin_sid*9 < dl_sid &&(chast_smen_sid == 1 || i % chast_smen_sid == 0)){
            //printf("тут\n");// i % chast_smen_sid == 0  изза этого ломается
            char chunk[9] = {0}; // +1 для нулевого терминатора
            strncpy(chunk, sid + Shet_vkin_sid*9, 9);
            //printf("chunk %s i = %d", chunk, i);
            Shet_vkin_sid++;
            unsigned int lok_sid = atoi(chunk) ^ time(NULL);
            srand(lok_sid); // time(NULL)
        }


        int id_simvola = -1;
        for(int a = 0; a < dl_alfaw; a++){
            if(alfaw[a] == text[i]){
                id_simvola = a + dl_alfaw * (0 + rand() % (kol_shif -1));
                break;
            }
        }
        if(id_simvola == -1) return -1;
        for(int j = 0; j < dl; j++){
            Shif_text[lok] = key[id_simvola][j];
            //printf("%lc", Shif_text[lok], id);
            lok++;
        }
    }
    return 0;
}

void rashif_text(wchar_t Shif_text[100000], wchar_t key[1000][100],wchar_t rashif_text[10000], int dl, int kol_shif){ // расшифровка текста
    int dl_alfaw = 0;
    for(int i = 0; alfaw[i] != '\0';i++) dl_alfaw += 1;
    int dl_text = 0, shet_sh = 0, shet_rash = 0;
    for(int i = 0; Shif_text[i] != '\0';i++) dl_text += 1;
    int kol_blocks = dl_text / dl;
    for(int i = 0; i < dl_text / dl;i++){
        wchar_t sim[100];
        for(int j = 0; j< dl; j++){
            sim[j] = Shif_text[shet_sh];
            //printf("%lc", Shif_text[shet_sh]);
            shet_sh++;
        }
        for(int a = 0; a < dl_alfaw * kol_shif; a++){
            //printf("%ls %ls", key[a], sim);
            bool sx = 1;
            for(int k = 0; k < dl; k++){
                if(key[a][k] != sim[k])
                    sx = 0;
            }
            if(sx){
                if(a < dl_alfaw){
                    rashif_text[shet_rash] = alfaw[a];
                    //printf("%lc", alfaw[a]);
                    shet_rash++;
                    break;
                }else{
                    rashif_text[shet_rash] = alfaw[a % dl_alfaw];
                    shet_rash++;
                }
            }
        }
    }

}
