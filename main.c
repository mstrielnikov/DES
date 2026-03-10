#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "des.h"

#define OPEN_RESOURCE "resources/open.txt"
#define DECR_RESOURCE "resources/decrypted.txt"
#define ENCR_RESOURCE "resources/encrypted.txt"
#define KEY_RESOURCE "resources/key.txt"

int main(void) {
    des_message msg = {0};
    des_message_read(&msg, OPEN_RESOURCE);

    printf("Original: %.*s\n", (int)msg.data_len, msg.data);

    des_encrypt(&msg, (const uint8_t *)"encrypti");
    des_message_write(&msg, ENCR_RESOURCE);
    printf("Encrypted (%zu bytes)\n", msg.data_len);

    des_decrypt(&msg, (const uint8_t *)"encrypti");
    des_message_write(&msg, DECR_RESOURCE);
    printf("Decrypted: %.*s\n", (int)msg.data_len, msg.data);

    des_message_free(&msg);
    return 0;
}
