
#define UTF8_ERR ((size_t)-1)

extern const wchar_t alfaw[];
//wchar_t alfaw[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя0123456789!@#$%^&*()_+-=`~№;:?<>/.,{}[] ";
//extern int dl_alfaw = 0;
size_t utf8_to_wcs(const char *src, wchar_t *dst, size_t dst_len);

int wchar_array_to_int(const wchar_t *arr);

void gen_key(wchar_t key[10000][100], int dl, int kol_shif,char* sid); // генерации ключа

int shif_text(wchar_t text[10000],wchar_t Shif_text[100000], wchar_t key[1000][100], int dl, int kol_shif, char* sid); // шифрование текста

void rashif_text(wchar_t Shif_text[100000], wchar_t key[1000][100],wchar_t rashif_text[10000], int dl, int kol_shif); // расшифровка текста
