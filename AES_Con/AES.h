#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

void print_all_block(unsigned short blocks[16], int block_count);
void cryption(unsigned short blocks[16], unsigned short encryption_blocks[16], unsigned short secret_key[16]);
void decryption(unsigned short encryption_blocks[16], unsigned short decryption_blocks[16], unsigned short secret_key[16]);



// служебные функции
void text_to_block(char* text, unsigned short key[]);
void blocks_to_text(const unsigned short blocks[][16], int block_count, char *text);
int schid_s_klav(char *string,size_t* buffer_size, size_t* string_length);
// int subtract(int a, int b);

#endif
