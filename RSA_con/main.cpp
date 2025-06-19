#include <gmpxx.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdint.h>
#include <openssl/sha.h>
#include <cstdio>
#include <limits>
#include <memory>
#include "EntropyCollector.hpp"
#define SIZE 256
using namespace std;
const size_t BLOCK_SIZE = 256;
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
mpz_class gen_rand_chislo(){ // моя функция для генерации простых случайлых чисел
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
    sid.set_str(sha256(thead_data, long_thead_data), 16); // Основание 16


    gmp_randclass r(gmp_randinit_default); // передаём сгенерированный сид
    r.seed(sid);

    mpz_class number;
    number = r.get_z_bits(long_key); // генерируем случайное число указвнной длины в битах
    number = find_nearest_prime(number); // ищем ближайшее простое число

    // чистим память
    free(thead_data);
    free(entropia);

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


// Функция быстрого возведения в степень по модулю
//она шифрует и расшифровывает данные
mpz_class fast_pow_mod(mpz_class base, mpz_class exponent, mpz_class modulus) {
    mpz_class result = 1;
    base %= modulus; // Уменьшаем основание по модулю
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus; // Умножаем результат на основание, если показатель нечетный
        }
        exponent /= 2; // Делим показатель на 2
        base = (base * base) % modulus; // Квадрируем основание
    }
    return result;
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

size_t get_mpz_allocated_bytes(const mpz_class& num) {
    // Количество лимбов * размер одного лимба
    //cout<<"Либов " << num.get_mpz_t()<<endl;
    size_t s = mpz_size(num.get_mpz_t()) * sizeof(mp_limb_t);
    if (s == BLOCK_SIZE) return mpz_size(num.get_mpz_t()) * sizeof(mp_limb_t);

    // Получаем точное количество значащих битов
    size_t bits = mpz_sizeinbase(num.get_mpz_t(), 2);

    // Конвертируем биты в байты (округление вверх)
    return (bits + 7) / 8;

}
size_t bytes_read;
void zap_mpz(mpz_class number, std::ofstream& output){

    size_t bits = get_mpz_allocated_bytes(number);
    // Обратное преобразование с сохранением ведущих нулей
    size_t expected_bytes = bits;//bytes_read;
    //cout<< expected_bytes<<endl;
    //unsigned char* bytes = new unsigned char[expected_bytes];
    auto bytes = std::make_unique<unsigned char[]>(expected_bytes);
    size_t count;
    mpz_export(
        bytes.get(),              // Целевой буфер
               &count,             // Количество экспортированных элементов
               1,                  // Порядок: старший байт первый
               1,                  // Размер элемента: 1 байт
               0,                  // Nails (0 бит)
    0,                  // Nails
    number.get_mpz_t()
    );

    // Если count < expected_bytes, дополняем нулями
    if (count < expected_bytes) {
        std::memmove(bytes.get() + (expected_bytes - count), bytes.get(), count);
        std::memset(bytes.get(), 0, expected_bytes - count);
    }

    // Запись в выходной файл
    output.write(reinterpret_cast<char*>(bytes.get()), expected_bytes);
    //delete[] bytes;
}


int main(int argc, char* argv[]){
    //mpz_class ass;
    //ass = gen_rand_chislo();
    //cout<<"Рандомное число "<<ass<<endl;

    //mpz_class p("29344569467557721779772350994811203059814482418635669581215677153020781865560979647041884596431959586832341205073939851631977048948860631577769544211805453570928520893978890569671936770279329526335505252496672087044506479264229024367271437862616061874251043934854751374334806453275319277691945127651378104328268495732261456572660844748195692594761723521678663302089772582215880159248682410013967781968417083443896847546830322750068256275322770203786378764763335217867358379222856162431931859832785211442038823379137645156109741181503459323154587731971005816484524848271927063975141901052981331171320570021972523005037"),q("29737197237262935868643682174589843424444447737759835099391127736545088655244576510674294748071616813130082821967783605658656336716778072223402467405026859136483510265384775942321216203206664843127218607577867650053691745178431088188827948321520042717770165010744385318296912864008491601340972830419188483340521460077248903959068068582026228997705482823333467482226163916939943428195728870131176322181197657500663464779759953090327531242941982857366244152955133196552433602273568616134012305726333036212472563564662175112925686015719289356261199957452500799278759289354242530121754923592513889470205970906861103731173"), n("872625250099327785110388242195441135087465766645738081210159582846561485824784887565407585613599540805950696949703720341937856905670057992290001651536280652999736052776348502661823418668969702484837014183374500574334149258658182361505538493048877434027287322965915006183027097561400654137506412410456646309938976851245634661887990215763071427400870551674441544713439289268463263062043810535590160316372761471569823294683402890926295062334263672777179664993653588083179154827662561993601703203869804717338900619102858862545020080155142181318263493749084365401984121757102133044398424636094613126352781620644764057849228739174064730657216815474084602623127116091370255170098345348619045500685312868423899114493416692127119805499827985684030128425519419128471477132472356732058866411461184434224234299532326778212728132466115652423426800419554388046242166009196215381478253259690115143885797172082698793082884804235987762122441795461911713001616969134464747199022176449673194243084326494428743616715986769454018475475723097826029755389252596560104747850388948431775380759800696159744299992370769653155387343397688521497650606811566542392596613355627500248751800539385159674476320596377543013241729055185523905066449063606443938972918401"),d("346016942709175445193763206279398336474327065288658246737086181537659540902523534387632130359043155269912274923050346835008301988306571815091326623410795815028215218937378405155451198268924663296663861445951953040652174142617913316576047558155868866107193122387586146233094666895465443933524255600813843594555505919302383507918934368937164306939079039723571607221388632531540272169817545880775462656843904242819857453941095731060341940962822681301563513041336585341373219654644963891049749929947428402116148288579367277430420019576600696795973471658108479236177447428198012289619327418377263397997005294348161856223604557533073866383245861059722955633797975632825191435757065040143704886538673596958379174141550652889232630000355472427579523896435299020338239760374635901638559057950522854121153099403765742248423610684730616222586041247606214410374289536173686683376451657883298556219063897050747503586241344106447261247267132979646275234354366809994703883067059128528456727533154925750586660665447461157276748437570839448435371842838032337598268414144993770827878093186718056435137353975861626040686294792946683587250690934886723701528552842304616039708577788765385901005421408870999878173718920599419369770709682556747196890782865");//, m;
    mpz_class p,q, n, d;
    mpz_class e = 65537; // открытый

    //cout<<argc<<endl;
    if(argc == 1){
        cout<<"Моя реализация RSA"<<endl;
        cout<<"Для шифрования и генерации ключа передайте один элемент -  флаг g"<<endl;
        cout<<"Пример RSA -g 2048(длина ключа) 123(сид не обязательно) name_file(не обязательно)  флаг g, длину ключа в битах и имя файла или путь куда записать ключ сид - дополнительный пользовательский сид"<<endl;

        cout<<"Для шифрования по вашему ключу передайте четыре элемента - имя файла, имя зашифрованного файла, e и n,  и флаг 0"<<endl;
        cout<<"Пример RSA file_name shif_file_name 65537 543534534534 0"<<endl;

        cout<<"Для расшифрования по вашему ключу передайте три элемента - имя файла d и n и флаг 1"<<endl;
        cout<<"Пример RSA shif_file_name file_name 883730063718 543534534534 1"<<endl;
        return 0;
    }else if( argc == 6 && argv[5][0] == '0'){
        e = mpz_class(argv[3]);
        n = mpz_class(argv[4]);
    }
    else if(argc == 6 && argv[5][0] == '1' && !(argv[1][0] == '-' && argv[1][1] == 'g')){
        d = mpz_class(argv[3]);
        n = mpz_class(argv[4]);

    }else if(argv[1][0] == '-' && argv[1][1] == 'g'){ // генерируем закрытый и открытый ключ и сохраняем в файл
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
    }else{
        cout<<"Вы не ввели нужные аргументы!!!!"<<endl;
        return -1;
    }


    //gen_key(&p,&q, &n, &d, &e);
    mpz_class c, r;
    if(argv[5][0] == '0'){ // шифрование файла


        std::ifstream input(argv[1], std::ios::binary);
        std::ofstream Shif(argv[2], std::ios::binary);
        //std::ofstream output("output.bin", std::ios::binary);

        if (!input.is_open()) {
            std::cerr << "Ошибка открытия файлов!" << std::endl;
            return 1;
        }

        std::vector<unsigned char> buffer(BLOCK_SIZE);
        int kol = 0;
        while (input) { // шифрование
            kol++;
            // Чтение блока данных из файла
            input.read(reinterpret_cast<char*>(buffer.data()), BLOCK_SIZE);
            bytes_read = input.gcount();

            if (bytes_read == 0) break;

            // Преобразование байт в mpz_class
            mpz_class number, lo;
            mpz_import(
                number.get_mpz_t(),
                       bytes_read,          // Количество байт (не лимбов!)
            1,                   // Порядок: старший байт первый
            1,                   // Размер элемента: 1 байт
            0,                   // Endian: native
            0,                   // Nails (0 бит)
            buffer.data()
            );

            int ras = 0;
            lo = number;
            size_t bits = get_mpz_allocated_bytes(lo);
            if(bits != bytes_read) cout<<"bytes_read "<<bytes_read<<" bits"<<bits<<" lo"<< lo<<endl;
            mpz_class sh = fast_pow_mod(number, e, n);
            size_t rr = get_mpz_allocated_bytes(sh);
            Shif.write(reinterpret_cast<const char*>(&rr), sizeof(rr)); // Записываем размер
            zap_mpz(sh,Shif);

        }
        Shif.close();
        input.close();
        //output.close();
       /* FILE *fp, *fplok;//, * f_obr1;
        //unsigned char buffer[16];
        size_t bytesRead;
        //printf("\n");
        // Открываем исходный файл для чтения, чтобы узнать размер
        fplok = fopen(argv[1], "rb+");
        // Получаем размер файла
        fseek(fplok, 0, SEEK_END);
        long file_size = ftell(fplok);
        fclose(fplok);


        // Открываем исходный файл
        fp = fopen(argv[1], "rb");


        if (fp == NULL) {
            printf("Ошибка открытия файла!\n");
            return 1;
        }

        std::ofstream file(argv[2], std::ios::binary); // Открываем  файл
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл для записи." << std::endl;
        }
        long schet = 0;
        unsigned char buffer[SIZE], ob_buffer[SIZE];
        mpz_class big_number;//, temp;
        //vector<unsigned char> arr(SIZE);
        //vector<unsigned char> ob_arr(SIZE);
        // Читаем файл по 16 байт
        //cout<<"размер "<< file_size << "остаток " << file_size % SIZE<<endl;
        //mpz_class c, r; //lokk = 0;
        //mpz_t temp1;
        while ((bytesRead = fread(buffer, 1, SIZE, fp)) > 0) {
            int lock_ras = SIZE;
            if(schet == file_size - (file_size % SIZE))
                lock_ras = file_size % SIZE;


            mpz_class big_number = 0;
            //cout<<lock_ras<<endl;
            for (int i = 0; i < lock_ras; ++i) {
                //cout<<big_number<<" buff ="<<buffer[i]<<endl;
                big_number = (big_number << 8) | buffer[i];
            }



            //cout<<big_number<<endl;
            c = fast_pow_mod(big_number, e,n);
            mpz_t temp1;
            mpz_init_set(temp1, c.get_mpz_t());

            // Полезные байты
            size_t size = (mpz_sizeinbase(temp1, 2) + 7) / 8; // Получаем размер в байтах (округление вверх)

            // Записываем байты
            file.write(reinterpret_cast<const char*>(&size), sizeof(size)); // Записываем размер
            char* buffer11 = new char[size];
            mpz_export(buffer11, nullptr, 1, size > 0 ? 1 : 0, 0, 0, temp1); // Экспортируем в байты
            file.write(buffer11, size); // Записываем байты в файл

            delete[] buffer11; // Освобождаем память
            mpz_clear(temp1); // Освобождаем память

            schet += lock_ras;
            //big_number = 0;
            c = 0;
            r = 0;
        }
        fclose(fp);
        file.close();
        cout<<"Шифрование файла закончена"<<endl;*/


    }
    //cout<<"РРР"<<endl;

    if(argv[5][0] == '1'){ // расшифровка файла



        std::vector<unsigned char> buffer(BLOCK_SIZE);

        std::ifstream file1(argv[1], std::ios::binary);
        std::ofstream file2(argv[2], std::ios::binary);
        int kol1 = 0;
        file1.seekg(0, std::ios::end);
        size_t size44 = file1.tellg();
        //cout<<"file раз " << size44<<endl;
        file1.seekg(0, std::ios::beg); // Возвращаем указатель в начало

        if(size44 <= 520){ // расшифровка файла размером до 120 байт

            while (true) {
                size_t size;
                if (!file1.read(reinterpret_cast<char*>(&size), sizeof(size))) break; // Читаем размер

                std::vector<char> buffer(size);
                if (!file1.read(buffer.data(), size)) break; // Читаем байты

                mpz_t temp;
                mpz_init(temp);
                mpz_import(temp, size, 1, sizeof(char), 0, 0, buffer.data()); // Импортируем байты в mpz_t

                mpz_class numm = mpz_class(temp);

                mpz_class r = fast_pow_mod(numm, d,n);
                vector<unsigned char> buff;
                int j = 0;
                while (r > 0) {
                    //printf("%02hhx ", (r).get_ui() & 0xFF);
                    buff.push_back((r).get_ui() & 0xFF);
                    r >>= 8;
                    j++;
                }
                //printf("\n");
                unsigned char lok;
                for(int j = 0; j<buff.size()/2; j++){
                    lok = buff[j];
                    buff[j] = buff[buff.size() - j - 1];
                    buff[buff.size() - j - 1] = lok;
                }
                char* buff_for_zap = new char[buff.size()];
                for(int j = 0; j<buff.size(); j++){
                    //printf("%02hhx ", buff[j]);
                    buff_for_zap[j] = buff[j];
                }
                file2.write(buff_for_zap, buff.size());
                delete[] buff_for_zap; // Освобождаем память
                //printf("\n");

                mpz_clear(temp); // Освобождаем память
            }
            cout<<"Расшифровка файла закончена"<<endl;


        }else{ // расшифровка 120 байтов
            while (file1) {
                cout<< "000"<<endl;
                kol1++;
                size_t size;
                if (!file1.read(reinterpret_cast<char*>(&size), sizeof(size))) break; // Читаем размер
                //cout<< "111 "<<size<<endl;
                // Чтение блока данных из файла
                file1.read(reinterpret_cast<char*>(buffer.data()), size);
                bytes_read = file1.gcount();

                if (bytes_read == 0) break;
                // Преобразование байт в mpz_class
                mpz_class number;
                mpz_import(
                    number.get_mpz_t(),
                           bytes_read,          // Количество байт (не лимбов!)
                1,                   // Порядок: старший байт первый
                1,                   // Размер элемента: 1 байт
                0,                   // Endian: native
                0,                   // Nails (0 бит)
                buffer.data()
                );
                //cout<< "1,5   "<<endl;
                //cout<< d<<endl;
                //if (d > 0 ) cout<< "1,6   "<<endl;
                mpz_class rsh = fast_pow_mod(number, d, n);
                //cout<< "222"<<endl;
                //zap_mpz(rsh,file2);


                size_t bits = get_mpz_allocated_bytes(rsh);
                //if(bits != bytes_read) cout<<"bytes_read "<<bytes_read<<" bits"<<bits<<" bytes_read"<< bytes_read<<endl;
                //cout<< "444"<<endl;
                // Обратное преобразование с сохранением ведущих нулей
                size_t expected_bytes = bits;//bytes_read;
                //cout<< expected_bytes<<endl;
                //unsigned char* bytes = new unsigned char[expected_bytes];
                auto bytes = std::make_unique<unsigned char[]>(256);
                //cout<< "555 "<< expected_bytes<<endl;
                size_t count;
                mpz_export(
                    bytes.get(),              // Целевой буфер
                           &count,             // Количество экспортированных элементов
                           1,                  // Порядок: старший байт первый
                           1,                  // Размер элемента: 1 байт
                           0,                  // Nails (0 бит)
                0,                  // Nails
                rsh.get_mpz_t()
                );

                // Если count < expected_bytes, дополняем нулями
                if (count < expected_bytes) {
                    std::memmove(bytes.get() + (expected_bytes - count), bytes.get(), count);
                    std::memset(bytes.get(), 0, expected_bytes - count);
                }

                // Запись в выходной файл
                file2.write(reinterpret_cast<char*>(bytes.get()), expected_bytes);
            }


        }
        //cout<< "Rrrr"<<endl;
        file1.close();
        file2.close();
        std::cout << "Файл успешно обработан!" << std::endl;















        /*std::vector<mpz_class> numbers;
        std::ifstream file1(argv[1], std::ios::binary);

        std::ofstream file2(argv[2], std::ios::binary);
        if (!file2.is_open()){
            std::cerr << "Не удалось открыть файл для записи." << std::endl;
        }
        if (file1.is_open()) {
            while (true) {
                size_t size;
                if (!file1.read(reinterpret_cast<char*>(&size), sizeof(size))) break; // Читаем размер

                std::vector<char> buffer(size);
                if (!file1.read(buffer.data(), size)) break; // Читаем байты

                mpz_t temp;
                mpz_init(temp);
                mpz_import(temp, size, 1, sizeof(char), 0, 0, buffer.data()); // Импортируем байты в mpz_t
                //numbers.push_back(mpz_class(temp)); // Конвертируем в mpz_class

                mpz_class numm = mpz_class(temp);
                //cout<<numm<<endl;
                r = fast_pow_mod(numm, d,n);

                //cout<<"Расшифрованные данные"<<endl;
                //cout<<r<<endl;
                vector<unsigned char> buff;
                int j = 0;
                while (r > 0) {
                    //printf("%02hhx ", (r).get_ui() & 0xFF);
                    buff.push_back((r).get_ui() & 0xFF);
                    r >>= 8;
                    j++;
                }
                //printf("\n");
                unsigned char lok;
                for(int j = 0; j<buff.size()/2; j++){
                    lok = buff[j];
                    buff[j] = buff[buff.size() - j - 1];
                    buff[buff.size() - j - 1] = lok;
                }
                char* buff_for_zap = new char[buff.size()];
                for(int j = 0; j<buff.size(); j++){
                    //printf("%02hhx ", buff[j]);
                    buff_for_zap[j] = buff[j];
                }
                file2.write(buff_for_zap, buff.size());
                delete[] buff_for_zap; // Освобождаем память
                //printf("\n");




                mpz_clear(temp); // Освобождаем память
            }
            file1.close();
            cout<<"Расшифровка файла закончена"<<endl;
        } else {
            std::cerr << "Не удалось открыть файл для чтения." << std::endl;
        }

        file1.close();
        file2.close();*/

    }
    //fclose(f_obr);
    //cout<<"РРР"<<endl;
    return 0;
}

