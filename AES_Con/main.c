#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AES.h"
#ifdef _WIN32
#include <windows.h>
#endif
int main(int argc, char* argv[]) {
    #ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    #endif



    if(argc == 1){
        printf("Моя реализация AES на Си\n");
        printf("Чтобы зашивровать или расшифровать файл, передайте три аргумента:\n 1) имя входного файла\n 2) как хотите назвать выходной файл\n 3) ключ шифрования\n 4) выбраннай режим (0 режим шифрования, 1 режим расшифровки)\n");
        printf("Пример для Linux ./AES оригинальный_файл.txt зашифрованный_файл.txt \"ключ шифрования\" 0\n");
        printf("© ИVан КоZлоV\n");
        return 0;
    }
    unsigned short secret_key[16];
    text_to_block(argv[3], secret_key); // преобразуем текст в блоки

    if(argc < 5){
        printf("Вы ввели слишком мало элементов\n");
        return 1;
    }
    if(argv[4][0] != '0' && argv[4][0] != '1'){
        printf("Вы ввели не верный третий элемент введите 0 или 1\n");
        return 1;
    }

    FILE *fp,* f_obr;
    unsigned short buffer[16];
    size_t bytesRead;


    // Открываем файл для чтения
    fp = fopen(argv[1], "rb");


    if (fp == NULL) {
        printf("Ошибка открытия файла!\n");
        return 1;
    }

    f_obr = fopen(argv[2], "wb");
    if (f_obr == NULL) {
        printf("Ошибка создания файла!\n");
        return 1;
    }

    unsigned short matrix[16];
    unsigned short matrix_obr[16];
    unsigned short matrix_add_nul[16];
    for(int i = 0; i< 16;i++) matrix_add_nul[i] = 0;//{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
    for(int i = 0; i< 16;i++) matrix[i] = 0;
    int add_byte = 0;
    long file_size;
    if(argv[4][0] == '0'){
        FILE *fplok;
        unsigned short buffer[16];

        // Открываем файл для чтения
        fplok = fopen(argv[1], "rb+");
        // Получаем размер файла
        fseek(fplok, 0, SEEK_END);
        file_size = ftell(fplok);
        //printf("размер %d\n",file_size);
        fclose(fplok);
    }
    if(argv[4][0] == '1'){
        FILE *fplok;
        unsigned short buffer[16];

        // Открываем файл для чтения
        fplok = fopen(argv[1], "rb+");
        // Получаем размер файла
        fseek(fplok, 0, SEEK_END);
        file_size = ftell(fplok);
        // Перемещаемся на 16 байт перед концом файла
        fseek(fplok, -16*sizeof(unsigned short), SEEK_END);
        // Читаем 16 байт в буфер
        fread(matrix_add_nul, sizeof(unsigned short), 16, fplok);
        //printf("\n");
        //printf("Остаток %02X\n размер %d\n", matrix_add_nul[15],file_size);
        int n = 16 + (16-matrix_add_nul[15]);
        add_byte = matrix_add_nul[15];
        // Перемещаемся на n байт перед концом файла
        fseek(fplok, -n, SEEK_END);
        char temp_buffer[1];
        fclose(fplok);
    }
    int schet = 0;
    unsigned short secret_key_for_sh[16], secret_key_for_rsh[16];
    for(int i = 0; i < 16; i++) {
        secret_key_for_sh[i] = secret_key[i];
        secret_key_for_rsh[i] = secret_key[i];
    }

    // Читаем файл по 16
    while ((bytesRead = fread(buffer, sizeof(unsigned short), 16, fp)) > 0) {
        // чтение
        schet += bytesRead * sizeof(unsigned short);
        for (int i = 0; i < 16; i++) {
            matrix[i] = buffer[i];
        }
        //}
        if(bytesRead < 16 ){ // добавляем нули
            if(file_size % 2 ==  0) add_byte = bytesRead;
            else add_byte = bytesRead+1;
            for(int j = bytesRead+1; j < 16; j++){
                matrix[j] = 0;
            }
        }
        // дальше будет шифрование или расшифровка
        if(argv[4][0] == '0')
            cryption(matrix, matrix_obr, secret_key);//printf("шифрование");
        else if(argv[4][0] == '1')
            decryption(matrix, matrix_obr, secret_key);//printf("расшифровка");

        if(argv[4][0] != '1' || schet < file_size - (16 * sizeof(unsigned short)) - add_byte ){
            fwrite(matrix_obr,sizeof(unsigned short),16,f_obr);
        }else if(schet < file_size - (16)+ add_byte){ // при расшифровке последние добавленные байты не записываются
            unsigned char Star_posl_bait = (matrix_obr[add_byte-1] >> 8) & 0xFF;
            unsigned char low = matrix_obr[add_byte-1] & 0xFF;
            if (add_byte % 2 == 0 && (Star_posl_bait == 0 && low != 0)){
                fwrite(matrix_obr,sizeof(unsigned short),add_byte-1,f_obr);
                unsigned char last[1];
                last[0] = matrix_obr[add_byte-1];
                fwrite(last,sizeof(unsigned char),1,f_obr);
            }else if(add_byte != 0 && (Star_posl_bait != 0 && low != 0)){ // на случай, если добавлено не чётное количество unsigned short
                fwrite(matrix_obr,sizeof(unsigned short),add_byte,f_obr);
            }else if(add_byte == 0){ // на случай, если длинна файла кратна 32 битам или 16ти unsigned short(ам)
                fwrite(matrix_obr,sizeof(unsigned short),16,f_obr);
            }
            else{ // unsigned short весит 2 байта, если файл весит не чётное количество, то при расшифровке последний элемент массива переводитя в unsigned char
                fwrite(matrix_obr,sizeof(unsigned short),add_byte-1,f_obr);
                unsigned char last[1];
                last[0] = matrix_obr[add_byte-1];
                fwrite(last,sizeof(unsigned char),1,f_obr);
            }
        }
        for(int i = 0; i< 16;i++) buffer[i] = 0;
    }
    //добавляем нулевую строку, с нулями, где в конце будет число добавленных бит
    if(argv[4][0] == '0'){
        // на случай, если размер файла на один байт больше числа кратного 32ум байтам (16 unsigned shortам)в этом случае останется байт, котрый не поместится и пропадет, если его не записать в нижнем  if блоке кода
        if(schet+1 == file_size && file_size % 16 == 1){
            unsigned char b[2];
            fseek(fp, -sizeof(unsigned char), SEEK_END); // Перемещаемся на байт перед концом файла
            bytesRead = fread(b, sizeof(unsigned char), 2, fp);
            buffer[0] = b[0];
            cryption(buffer, matrix_obr, secret_key);
            fwrite(matrix_obr,sizeof(unsigned short),16,f_obr);
            add_byte = 1;
        }
        matrix_add_nul[15] = add_byte;
        cryption(matrix_add_nul, matrix_obr, secret_key);
        fwrite(matrix_add_nul,sizeof(unsigned short),16,f_obr);
    }
    printf("Файл успешно обработан\n");
    // Закрываем файл
    fclose(fp);
    fclose(f_obr);

    return 0;

}
