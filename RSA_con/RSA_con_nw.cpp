#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4146) // Отключаем предупреждение C4146
#include <gmpxx.h>
#pragma warning(pop)
#else
#include <gmpxx.h>
#endif
#include <limits>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#include <clocale>
#endif

using namespace std;
const size_t BLOCK_SIZE = 256;
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

mpz_class text_to_mpz(const std::string& text) { // текст в mpz_class
    mpz_class number;

    // Импорт сырых байтов текста
    mpz_import(number.get_mpz_t(),       // Куда записываем
               text.size(),               // Количество байтов
               1,                         // Порядок байтов (1 - big-endian)
    sizeof(char),              // Размер элемента
               0,                         // Endian (0 - native)
    0,                         // Nails (биты, которые не используются)
    text.data()                // Указатель на данные
    );

    return number;
}

std::string mpz_to_text(const mpz_class& number) { // mpz_class в текс
    size_t count;

    // Получаем размер буфера
    mpz_export(nullptr, &count, 1, sizeof(char), 0, 0, number.get_mpz_t());

    // Выделяем память
    std::string result(count, '\0');

    // Экспортируем данные
    mpz_export(result.data(), &count, 1, sizeof(char), 0, 0, number.get_mpz_t());

    return result;
}


string Shif_RSA_text(string original_text, string e_text, string n_text){ // шифрование текста RSA
    mpz_class e = mpz_class(e_text);
    mpz_class n = mpz_class(n_text);
    mpz_class original_chislo = text_to_mpz(original_text);
    mpz_class Shif = fast_pow_mod(original_chislo, e, n);
    return (string) Shif.get_str(16);
}

string RaShif_RSA_text(string Shif_text, string d_text, string n_text){ // расшифровка текста RSA
    mpz_class d = mpz_class(d_text);
    mpz_class n = mpz_class(n_text);
    mpz_class Shif_chislo = mpz_class(Shif_text, 16);
    mpz_class rashif_chislo = fast_pow_mod(Shif_chislo, d, n);

    return (string) mpz_to_text(rashif_chislo);
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

int main(int argc, char* argv[]) {

    #ifdef _WIN32 // добавляем поддержку кириллицы
    setlocale(LC_ALL, "");
    //setlocale(LC_ALL, "ru_RU.UTF-8");
    #endif

    /*string original_text = "Привет мир!!!! ZOV 123456789";
    string e_text = "65537";
    string n_text ="565124466205892178378244081259920283519672538433465851692730129084388920931545502478637437813018034886857292679924691261318536558571997602557681379569114108191818759907249584547657854886132993014454089367192084727820076163810279297649980516376639806965656824412382234542468973693638647451710243854051438925394529930444970465254786935038573448763526453699408427881623278295491149591519686544605990496212883254749199621553643239516117422228658997487051874306402655796830262691136705356120280384642975858727426496683893320100768633072938287716739694316035900892100755762020617627332803403180003269822425361777294513553996204858135737580939695995883603471106797024568786616477843736139695354103763444924145017786953410366493579492891987038351388249926669148731104872477535153809933401511845706937394515466122926520194748382539230596809034696138114319944991621918749062553144740093198801926196476400342042521610366750917143253408884184204496908904784689835455974256993576907816442892671823024609865904314112116607190831865964545663540415770269565832726334905050955985011369324815286542227664433362597405968864756322782918536003099752875670431917642638550115234783880966588963491130006121253884123248306562293061947925480402155682861234319";
    string d_text = "335692745615383409436883624264899165927962096544163230028807908895055627994340088980169300609743993441038717122075594409312764212966841122840083252309773291910027989123536724696588496432811349589586000260384025183545715627159225674924298358218145287168668388458031957378859531957418748879183969257644117328617560312376561335007230348979227983587348899743930452987344465325594251393836480113241546149771389369476575694143511776772852758706710633263209018825217135208669944101285562957013023412334269957127404573231975326174877136358537735032312682907720487995017813171421680031616736141199589961307155032027558103249388834112641923367796220780366160363869820826729181736028836893877661381274748569040756188961816707421547462510059962921685415966788126262224059874070596573778245006737728772300021339420660552181280120649125667913257130673791624131198265690235337018472583883600288216302697624864045098693775416749083815573893719248930181666920163657770364662864591710745989276105238469698098096625217842296229055485595830367259922228800950648691961212068935954952215468974886911431934040406359634857675004846470288453212211929079874476997093874332647655232013521367372452571596154146965427612465024305577468679519989366006329219589633";
    string Shif = Shif_RSA_text(original_text, e_text,  n_text);
    cout<< Shif <<endl;

    string Rashif = RaShif_RSA_text(Shif, d_text, n_text);

    cout<< Rashif <<endl;

    return 0;*/

    //mpz_class p("20152078181582730435322550289494993053609300602428097955111050652216847072776237629332875343116539756547599356296253220613613514473094923176970198175879628348445443222424983795227343487809789484363088305899226151219363869029521428984107435922446867562793699262603283874745795243736914517130998623794648737109793655257497423746905119914311809412255579986894029909939588842080377267766326835879089094593191965422708123854724740459288069067726118996488927718937975440867435767546961033280205490363909089237709661235241079172887088452847573392275804277369928412129227636536922112963803366261027623128011842053072577345153"),q("28042986986938519775862752481931448334809929942459637605139993115007560598847547155484160014076234743366066175080198497971601958279327350460936319126605752646054025420925204256307975430359615116885981500175866299400804323941840497271165754428018553078259338300102612019344484374819265351153925345599092993918595321725388493507564500840836204142021661263077584107597027693850964143177589273331142853134731614941229848840078535537192132897128184543021666106078607928622977031816371290041682830738650496183247633326066813222592299828170320395283053574564824337035786079639303920427829733030992330199432519004310821668623"), n("565124466205892178378244081259920283519672538433465851692730129084388920931545502478637437813018034886857292679924691261318536558571997602557681379569114108191818759907249584547657854886132993014454089367192084727820076163810279297649980516376639806965656824412382234542468973693638647451710243854051438925394529930444970465254786935038573448763526453699408427881623278295491149591519686544605990496212883254749199621553643239516117422228658997487051874306402655796830262691136705356120280384642975858727426496683893320100768633072938287716739694316035900892100755762020617627332803403180003269822425361777294513553996204858135737580939695995883603471106797024568786616477843736139695354103763444924145017786953410366493579492891987038351388249926669148731104872477535153809933401511845706937394515466122926520194748382539230596809034696138114319944991621918749062553144740093198801926196476400342042521610366750917143253408884184204496908904784689835455974256993576907816442892671823024609865904314112116607190831865964545663540415770269565832726334905050955985011369324815286542227664433362597405968864756322782918536003099752875670431917642638550115234783880966588963491130006121253884123248306562293061947925480402155682861234319"),d("335692745615383409436883624264899165927962096544163230028807908895055627994340088980169300609743993441038717122075594409312764212966841122840083252309773291910027989123536724696588496432811349589586000260384025183545715627159225674924298358218145287168668388458031957378859531957418748879183969257644117328617560312376561335007230348979227983587348899743930452987344465325594251393836480113241546149771389369476575694143511776772852758706710633263209018825217135208669944101285562957013023412334269957127404573231975326174877136358537735032312682907720487995017813171421680031616736141199589961307155032027558103249388834112641923367796220780366160363869820826729181736028836893877661381274748569040756188961816707421547462510059962921685415966788126262224059874070596573778245006737728772300021339420660552181280120649125667913257130673791624131198265690235337018472583883600288216302697624864045098693775416749083815573893719248930181666920163657770364662864591710745989276105238469698098096625217842296229055485595830367259922228800950648691961212068935954952215468974886911431934040406359634857675004846470288453212211929079874476997093874332647655232013521367372452571596154146965427612465024305577468679519989366006329219589633");//, m;
    mpz_class p,q, n, d;
    mpz_class e = 65537; // открытый
    if(argc == 1){
        cout<<"Моя реализация RSA"<<endl;
        cout<<"Для шифрования файла по вашему ключу передайте четыре элемента - имя файла, имя зашифрованного файла, e и n,  и флаг 0"<<endl;
        cout<<"Пример RSA file_name shif_file_name 65537 543534534534 0"<<endl;

        cout<<"Для расшифрования по вашему ключу передайте три элемента - имя файла d и n и флаг 1"<<endl;
        cout<<"Пример RSA shif_file_name file_name 883730063718 543534534534 1"<<endl;


        cout<<"Для шифрования текста по вашему ключу передайте три элемента - текст, e и n,  и флаг -t0"<<endl;
        cout<<"Пример RSA 'Привет мир' 65537 543534534534 -t0"<<endl;


        cout<<"Для расшифрования текста по вашему ключу передайте три элемента - зашифрованный текст, d и n и флаг -t1"<<endl;
        cout<<"Пример RSA '67b64d75f0f6d1aee6017' 883730063718 543534534534 -t1"<<endl;
        return 0;
    }else if( argc == 6 && argv[5][0] == '0'){
        e = mpz_class(argv[3]);
        n = mpz_class(argv[4]);
    }
    else if(argc == 6 && argv[5][0] == '1' && !(argv[1][0] == '-' && argv[1][1] == 'g')){
        d = mpz_class(argv[3]);
        n = mpz_class(argv[4]);
    }else if(argc == 5 && argv[4][0] == '-' && (argv[4][1] == 't' && argv[4][2] == '0')){
        string original_text = argv[1];
        string e_text = argv[2];
        string n_text = argv[3];
        string Shif = Shif_RSA_text(original_text, e_text,  n_text);
        cout<< Shif <<endl;
        return 0;
    }else if(argc == 5 && argv[4][0] == '-' && (argv[4][1] == 't' && argv[4][2] == '1')){
        string Shif = argv[1];
        string n_text = argv[3];
        string d_text = argv[2];
        string Rashif = RaShif_RSA_text(Shif, d_text, n_text);

        cout<< Rashif <<endl;
        return 0;
    }else{
        cout<<"Вы не ввели нужные аргументы!!!!" <<endl;
        return -1;
    }

    if(argv[5][0] == '0'){ // шифрование
        std::ifstream input(argv[1], std::ios::binary);
        std::ofstream Shif(argv[2], std::ios::binary);

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
        cout<<"Шифрование файла закончена"<<endl;
        return 0;
    }

    if(argv[5][0] == '1'){ // расшифровка
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


    }else{ // расшифровка файлов размером больше 120 байтов
    while (file1) {
        std::vector<unsigned char> buffer(BLOCK_SIZE);
        cout<< "Идёт расшифровка! Она может занять больше времени"<<endl;
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
        //cout<<"1111"<<endl;
        // Запись в выходной файл
        file2.write(reinterpret_cast<char*>(bytes.get()), expected_bytes);
    }


    }
    //cout<< "Rrrr"<<endl;
    file1.close();
    file2.close();
    std::cout << "Файл успешно обработан!" << std::endl;
    }
    return 0;
}
