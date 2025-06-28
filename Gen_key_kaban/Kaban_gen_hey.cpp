#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4146) // Отключаем предупреждение C4146
#include <gmpxx.h>
#pragma warning(pop)
#else
#include <gmpxx.h>
#endif
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdint.h>
#include <openssl/sha.h>
#include <cstdio>
#include <limits>
#include "EntropyCollector.hpp"
#define SIZE 256
using namespace std;

string mpz_to_string(const mpz_class& num) {
    return num.get_str(10);  // 10 is the base for decimal representation
}


// Версия для массива unsigned char
string sha256(const unsigned char* data, size_t size) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(data, size, hash); // Уже unsigned char, преобразование не нужно

    string output;
    char buffer[3];

    for (const auto &byte : hash) {
        snprintf(buffer, sizeof(buffer), "%02x", byte);
        output += buffer;
    }

    return output;
}

mpz_class find_nearest_prime(const mpz_class& num) { // поиск ближайшего простого числа
    if (num < 2) {
        return 2; // Наименьшее простое число
    }

    mpz_class next = num;
    mpz_class prev = num;

    // Проверить, является ли само число простым
    if (mpz_probab_prime_p(num.get_mpz_t(), 25) > 0) {
        return num;
    }

    // Найти следующее простое число
    mpz_nextprime(next.get_mpz_t(), num.get_mpz_t());

    // Найти предыдущее простое число
    if (num > 2) {
        mpz_prevprime(prev.get_mpz_t(), num.get_mpz_t());
    } else {
        prev = 2; // Наименьшее простое число
    }

    // Определить, какое из двух чисел ближе
    mpz_class diff_next = next - num;
    mpz_class diff_prev = num - prev;

    if (diff_next < diff_prev) {
        return next;
    } else {
        return prev;
    }
}

mpz_class user_dop_sid("6787687676384920090"); // дополнительный пользовательский сид
int long_key = 2048; // длинна ключа
int mause_x = 3312,mause_y = 1231; // координаты мыши передаются из Qt



int gen_rand_thead_data(string* itog_thead) { // генерируем рандомный поток данных и спользуем его для генерацции случайных чисел и ключей для
    mpz_class sid;
    EntropyCollector ec;
    auto entropy = ec.collect();
    int* entropia = NULL;
    entropia  = (int*)malloc(sizeof(int));
    // выбираем из массива с энтропией числа не равные нулю
    int kol_entropi = 0, capacity_en = 1; // kol_entropi - количество энтропии не равное нулю
    for(int i = 0; i < entropy.size();i++) {
        if(entropy[i] != 0){
            if (kol_entropi == capacity_en) {
                capacity_en *= 2; // Удваиваем размер при необходимости
                entropia = (int*)realloc(entropia, capacity_en * sizeof(int));
                if (entropia == NULL) {
                    std::cerr << "Ошибка перевыделения памяти!" << std::endl;
                    return 1;
                }
            }
            entropia[kol_entropi] = entropy[i];
            kol_entropi++;
        }
    }

    // добавляем в массив entropia координаты мыши и пользовательский доп сид
    if (kol_entropi-2 - user_dop_sid.get_str().size() <= capacity_en) {
        capacity_en += 2 + user_dop_sid.get_str().size(); // Увеличиваем размер при необходимости
        entropia = (int*)realloc(entropia, capacity_en * sizeof(int));
        if (entropia == NULL) {
            std::cerr << "Ошибка перевыделения памяти!" << std::endl;
            return 1;
        }
    }
    entropia[kol_entropi] = mause_x;
    kol_entropi++;
    entropia[kol_entropi] = mause_y;
    kol_entropi++;
    while (user_dop_sid > 0){
        mpz_class lok = user_dop_sid % mpz_class(INT_MAX);
        int lo = lok.get_si();
        user_dop_sid /= INT_MAX;
        entropia[kol_entropi] = lo;
        kol_entropi++;
    }

    // динамический массив int преобразуем в динамический массив unsigned char
    unsigned char* thead_data  = NULL;
    int long_thead_data = 0, capacity = 1;
    thead_data  = (unsigned char*)malloc(capacity * sizeof(unsigned char));
    for(int i = 0; i < kol_entropi; i++){
        long a = entropia[i];
        while (a!=0){
            if (long_thead_data == capacity) {
                capacity *= 2; // Удваиваем размер при необходимости
                thead_data = (unsigned char*)realloc(thead_data, capacity * sizeof(unsigned char));
                if (thead_data == NULL) {
                    std::cerr << "Ошибка перевыделения памяти!" << std::endl;
                    return 1;
                }
            }
            thead_data[long_thead_data]=a%256;
            a=a/256;
            ++long_thead_data;
        }
    }


    // хэшируем энтропию из массива unsigned char и переводим в число и записываем в переменную сид
   *itog_thead = sha256(thead_data, long_thead_data);

    // чистим память
    free(thead_data);
    free(entropia);
    return 0;
}

mpz_class gen_rand_chislo(){ // моя функция для генерации простых случайлых чисел
    mpz_class sid;
    string rand_thead;
    gen_rand_thead_data(&rand_thead);

    // хэшируем энтропию из массива unsigned char и переводим в число и записываем в переменную сид
    sid.set_str(rand_thead, 16); // Основание 16


    gmp_randclass r(gmp_randinit_default); // передаём сгенерированный сид
    r.seed(sid);

    mpz_class number;
    number = r.get_z_bits(long_key); // генерируем случайное число указвнной длины в битах
    number = find_nearest_prime(number); // ищем ближайшее простое число

    return number;
}

mpz_class findD( mpz_class e, mpz_class phi){ // ищем d
    mpz_class eprev, dprev, d("1"), etemp, dtemp;

    eprev = phi, dprev = phi;
    while (e != 1)
    {
        etemp = e;
        dtemp = d;
        e = eprev - eprev / etemp * e;
        d = dprev - eprev / etemp * d;
        eprev = etemp;
        dprev = dtemp;
        while (d < 0)
            d += phi;
    }

    return d;
}

void gen_key(mpz_class *p,mpz_class *q, mpz_class *n, mpz_class *d, mpz_class* e){ // генерация ключей
    //cout<<"p="<<*p<<" q="<<*q<<endl;
    string df, ff;
    do{
        *p=gen_rand_chislo();
        *q=gen_rand_chislo();
        //cout<<"p="<<*p<<" q="<<*q<<endl;
        df = mpz_to_string(*p);
        ff= mpz_to_string(*q);
        //cout<<df.length() << ff.length()<<endl;
    }while(df.length() != ff.length());
    *n = *q * *p;
    mpz_class a = (*p-1)*(*q-1);
    *d = findD(*e, a);
    cout<<"Сгенерированный ключ"<<endl;
    cout<<"p ="<< *p<<", q = "<<*q<<endl;
    cout<<"Открытый ключ e ="<< *e<<", n = "<<*n<<endl;
    cout<<"Закрытый ключ d = "<< *d<<", n = "<<*n<<endl;
}


int main(int argc, char* argv[]){
    if(argc == 1){
        cout<<"Мой генератор ключей"<<endl;
        cout<<"Вы можете сгенерировать ключ для RSA, AES и других алгоритмов шифрования работающих по схожему принципу."<<endl;
        cout<<"Пример RSA -g 2048(длина ключа) 123(сид не обязательно) name_file(не обязательно)  флаг g, длину ключа в битах и имя файла или путь куда записать ключ, сид - дополнительный пользовательский сид"<<endl;
        cout<<"Пример AES -g -s 123(сид не обязательно) name_file(не обязательно) флаг g, флаг s, имя файла или путь куда записать ключ, сид - дополнительный пользовательский сид "<<endl;
        return 0;
    }
    if(argv[1][0] == '-' && argv[1][1] == 'g' && argc >= 4 && argv[2][0] != '-' && argv[2][1] != 's' ){ // генерируем закрытый и открытый RSA ключ и сохраняем в файл
        mpz_class p,q, n, d;
        mpz_class e = 65537;

        string sid, dl_key;
        dl_key = (string) argv[2];
        sid = (string) argv[3];
        user_dop_sid = sid;//atoi(sid.c_str());
        long_key = atoi(dl_key.c_str());
        if(argc >= 6) {
            string mause_x_st,mause_y_st;
            mause_x_st = (string) argv[5];
            mause_y_st = (string) argv[6];
            mause_x = atoi(mause_x_st.c_str());
            mause_y = atoi(mause_y_st.c_str());
        }
        gen_key(&p,&q, &n, &d, &e);
        if(argc >= 4){ // записываем сгенерированный ключ в файл
            std::ofstream file(argv[4], std::ios::binary); // Открываем  файл
            if (!file.is_open()) {
                std::cerr << "Не удалось открыть файл для записи." << std::endl;
            }
            file<<p;
            file<<'\n';
            file<<q;
            file<<'\n';
            file<<e;
            file<<'\n';
            file<<n;
            file<<'\n';
            file<<d;
            file<<'\n';
            file.close();
        }
        return 0;
    }else if(argv[1][0] == '-' && argv[1][1] == 'g' && argv[2][0] == '-' && argv[2][1] == 's'){
        string key_hex, sid;
        sid = (string) argv[3];
        user_dop_sid = sid;//atoi(sid.c_str());
        if(argc >= 6) {
            string mause_x_st,mause_y_st;
            mause_x_st = (string) argv[5];
            mause_y_st = (string) argv[6];
            mause_x = atoi(mause_x_st.c_str());
            mause_y = atoi(mause_y_st.c_str());
        }
        cout<<"Сгенерированный ключ"<<endl;
        gen_rand_thead_data(&key_hex);
        //key.resize(long_key / 8);
        if(argc >= 5){ // записываем сгенерированный ключ в файл
            std::ofstream file(argv[4], std::ios::binary); // Открываем  файл
            if (!file.is_open()) {
                std::cerr << "Не удалось открыть файл для записи." << std::endl;
            }
            file<<key_hex;
            file.close();
        }
        cout<<key_hex<<endl;
        return 0;
    }else{
        cout<<"Вы не ввели нужные аргументы!!!!"<<endl;
        return -1;
    }

    return 0;
}
