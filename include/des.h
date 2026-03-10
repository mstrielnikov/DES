#ifndef DES_H
#define DES_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *data;
    size_t data_len;
} des_message;

void des_encrypt(des_message *msg, const uint8_t *key);
void des_decrypt(des_message *msg, const uint8_t *key);

void des_message_read(des_message *msg, const char *file_path);
void des_message_write(const des_message *msg, const char *file_path);
void des_message_free(des_message *msg);

#endif
