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


int main(int argc,char* argv[]){
    char* sid;
    if(argc > 1) sid = argv[1];
    else sid = NULL;
    int dl_alfaw = 0;
    //for(int i = 0; alfaw[i] != '\0';i++) dl_alfaw += 1;
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if(argc == 1){
        printf("Это моя реализация моего алгоритма шифрования Шиф++ или Shif++ - эти названия равнозначныю\n");
        printf("Для генерации ключа введите флаг -g, имя или путь к файлу куда сохранить ключ, длинну шифротекста на шифр каждой буквы, количество вариантов шифротекста для каждой буквы, и сид сгенерированный в кабанчике или сгенерированный вами самостоятельно\n");
        printf("Пример: Shif -g key.txt 10 10 34324324324234234234234234234234234234234234\n");
        printf("Для шифрования текста введите сам текст ,имя файла с ключом или укажите путь к нему, флаг -t0 и по желанию сид\n");
        printf("Пример: Shif 'Привет мир' key.txt -t0 879879789789797\n");
        printf("Для расшифровки текста введите шифротекст ,имя файла с ключом или укажите путь к нему, и флаг -t1\n");
        printf("Пример: Shif '5?zы,fзУБгtУйХа^z3ц>№[)?_J`#ТO:AR$]ъEу;Qм' key.txt -t1\n");
        return 0;

    }
    if(argv[1][0] == '-' && argv[1][1] == 'g' && argc == 6){ // генерация ключа и запись его в файл
        for(int i = 0; alfaw[i] != '\0';i++) dl_alfaw += 1;
        char* sid = argv[5];
        int dl = atoi(argv[3]); // длинна шифротекста на шифр каждой буквы
        int kol_shif = atoi(argv[4]); // количество вариантов шифротекста для каждой буквы
        if(dl < 2 || kol_shif < 1){
            printf("Длина шифротекста должна быть больше одного 1. А количество вариантов шифротекста для каждой буквы больше нуля!!!\n");
            return -1;
        }
        wchar_t key[10000][100];
        gen_key(key, dl, kol_shif, sid);
        FILE *f = fopen(argv[2], "w");
        if (!f) { perror("fopen"); return 1; }
        fwprintf(f, L"%d\n%d\n", (dl), kol_shif);
        for (int i = 0; i < dl_alfaw *kol_shif; ++i) {
            /* fputws не добавляет '\n' автоматически */
            if (fputws(key[i], f) == WEOF || fputwc(L'\n', f) == WEOF) {
                perror("write");
                fclose(f);
                return 1;
            }
        }
        fclose(f);
        setlocale(LC_ALL, ""); // Установка локали для работы с широкими символами
        printf("Ключ сгенерирован и записан в файл\n");
        return 0;
    }else if(argv[3][0] == '-' && argv[3][1] == 't' && argv[3][2] == '0' && argc < 6){ // шифрование текста по прачтённому из файла ключу
        wchar_t Shif_text[100000];
        static wchar_t text[10000];
        const char *utf8 = argv[1];
        char* sid = argv[4];
        size_t need = utf8_to_wcs(utf8, text, sizeof text / sizeof *text);
        if (need == UTF8_ERR) {
            fputs("Ошибка: неверный UTF-8\n", stderr);
            return 1;
        }
        if (need >= sizeof text / sizeof *text) {
            fputs("Внимание: буфер усечен\n", stderr);
        }
        for(int i = 0; alfaw[i] != '\0';i++) dl_alfaw += 1;
        setlocale(LC_CTYPE, "");
        //printf("%ls\n", text);




        setlocale(LC_ALL, ""); // Установка локали для работы с широкими символами

        FILE *f1 = fopen(argv[2], "r, ccs=UTF-8"); // Открытие файла в UTF-8
        if (!f1) {
            perror("fopen");
            return 1;
        }

        wchar_t buf[256];
        int dl_l = 0;       // Длина ключа
        int kol_shif_l = 0;  // Количество вариантов шифротекста
        wchar_t key1[10000][100]; // Массив для хранения ключей

        // Чтение первой строки (длина ключа)
        if (!fgetws(buf, 256, f1)) {
            perror("Ошибка чтения длины ключа");
            fclose(f1);
            return 1;
        }
        dl_l = wchar_array_to_int(buf);

        // Чтение второй строки (количество вариантов)
        if (!fgetws(buf, 256, f1)) {
            perror("Ошибка чтения количества вариантов");
            fclose(f1);
            return 1;
        }
        kol_shif_l = wchar_array_to_int(buf);
        //printf(" dl_alfaw %d \n", dl_alfaw);
        // Чтение самих ключей
        for (int i = 0; i < kol_shif_l *dl_alfaw; ++i) {
            if (!fgetws(buf, 256, f1)) {
                fprintf(stderr, "Файл закончился раньше ожидаемого\n");
                break;
            }

            // Удаление символа новой строки (если есть)
            size_t len = wcslen(buf);
            if (len > 0 && buf[len - 1] == L'\n') {
                buf[len - 1] = L'\0';
            }

            wcscpy(key1[i], buf); // Копирование ключа в массив
            //wprintf(L"Ключ %d: %ls\n", i, key1[i]);
        }

        fclose(f1);


        if(shif_text(text, Shif_text, key1, dl_l , kol_shif_l, sid) != 0){
            printf("Ошибка при шифровани!!! Скорее всего вы использовали не существующий символ в раскладке!\n");
            return -1;
        }
        printf("%ls\n", Shif_text);

        return 0;
    }else if(argv[3][0] == '-' && argv[3][1] == 't' && argv[3][2] == '1' && argc < 5){ // расшифровка текста по прачтённому из файла ключу
        wchar_t Shif_text[100000];
        const char *utf8 = argv[1];
        char* sid = argv[4];
        size_t need = utf8_to_wcs(utf8, Shif_text, sizeof Shif_text / sizeof *Shif_text);
        if (need == UTF8_ERR) {
            fputs("Ошибка: неверный UTF-8\n", stderr);
            return 1;
        }
        if (need >= sizeof Shif_text / sizeof *Shif_text) {
            fputs("Внимание: буфер усечен\n", stderr);
        }
        for(int i = 0; alfaw[i] != '\0';i++) dl_alfaw += 1;
        setlocale(LC_CTYPE, "");


        setlocale(LC_ALL, ""); // Установка локали для работы с широкими символами

        FILE *f1 = fopen(argv[2], "r, ccs=UTF-8"); // Открытие файла в UTF-8
        if (!f1) {
            perror("fopen");
            return 1;
        }

        wchar_t buf[256];
        int dl_l = 0;       // Длина ключа
        int kol_shif_l = 0;  // Количество вариантов шифротекста
        wchar_t key1[10000][100]; // Массив для хранения ключей

        // Чтение первой строки (длина ключа)
        if (!fgetws(buf, 256, f1)) {
            perror("Ошибка чтения длины ключа");
            fclose(f1);
            return 1;
        }
        dl_l = wchar_array_to_int(buf);

        // Чтение второй строки (количество вариантов)
        if (!fgetws(buf, 256, f1)) {
            perror("Ошибка чтения количества вариантов");
            fclose(f1);
            return 1;
        }
        kol_shif_l = wchar_array_to_int(buf);
        //printf(" dl_alfaw %d \n", dl_alfaw);
        // Чтение самих ключей
        for (int i = 0; i < kol_shif_l *dl_alfaw; ++i) {
            if (!fgetws(buf, 256, f1)) {
                fprintf(stderr, "Файл закончился раньше ожидаемого\n");
                break;
            }

            // Удаление символа новой строки (если есть)
            size_t len = wcslen(buf);
            if (len > 0 && buf[len - 1] == L'\n') {
                buf[len - 1] = L'\0';
            }

            wcscpy(key1[i], buf); // Копирование ключа в массив
            //wprintf(L"Ключ %d: %ls\n", i, key1[i]);
        }

        fclose(f1);

        wchar_t Rashif_text[10000];
        rashif_text(Shif_text, key1,Rashif_text, dl_l , kol_shif_l);

        printf("%ls\n", Rashif_text);


        return 0;

    }else{
        printf("Ошибка при вводе аргументов\n");
        return -1;
    }


    return 0;
    }
