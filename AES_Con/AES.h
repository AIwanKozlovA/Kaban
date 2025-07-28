#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#define COUNT_BLOCKS_FOR_TEXT 500

void print_all_block(unsigned short blocks[][16], int block_count);
void cryption(unsigned short blocks[16], unsigned short encryption_blocks[16], unsigned short secret_key[16]);
void decryption(unsigned short encryption_blocks[16], unsigned short decryption_blocks[16], unsigned short secret_key[16]);

void Sfif_AES_text(char* original_text,unsigned short matrix_shif[COUNT_BLOCKS_FOR_TEXT][16], int* block_count, char* secret_key);
void Rasfif_AES_text(char*rashif_text, unsigned short matrix_shif[COUNT_BLOCKS_FOR_TEXT][16], int* block_count, char* secret_key);


// служебные функции
void dl_text_to_blocks(const char *text, unsigned short blocks[][16], int* block_count);
void text_to_block(char* text, unsigned short key[]);
void blocks_to_text(const unsigned short blocks[][16], int block_count, char *text);
int schid_s_klav(char *string,size_t* buffer_size, size_t* string_length);
// int subtract(int a, int b);

#endif
