#include "des.h"
#include "permutations.h"
#include "key.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void round_feistel_cipher(uint32_t *N1, uint32_t *N2, uint64_t key48b);
static void swap(uint32_t *N1, uint32_t *N2);
static uint32_t magic_fn(uint32_t block32b, uint64_t key48b);
static void feistel_round(uint32_t *N1, uint32_t *N2, uint64_t key48b);

static void swap(uint32_t *N1, uint32_t *N2) {
    uint32_t temp = *N1;
    *N1 = *N2;
    *N2 = temp;
}

static void round_feistel_cipher(uint32_t *N1, uint32_t *N2, uint64_t key48b) {
    feistel_round(N1, N2, key48b);
}

static void feistel_round(uint32_t *N1, uint32_t *N2, uint64_t key48b) {
    uint32_t temp = *N2;
    *N2 = magic_fn(*N2, key48b) ^ *N1;
    *N1 = temp;
}

static uint32_t magic_fn(uint32_t block32b, uint64_t key48b) {
    uint64_t block48b = des_expansion_permutation(block32b);
    block48b ^= key48b;
    block32b = des_substitutions(block48b);
    return des_permutation(block32b);
}

static void feistel_encrypt(uint32_t *N1, uint32_t *N2, uint64_t *keys48b) {
    for (int8_t round = 0; round < 16; ++round) {
        round_feistel_cipher(N1, N2, keys48b[round]);
    }
    swap(N1, N2);
}

static void feistel_decrypt(uint32_t *N1, uint32_t *N2, uint64_t *keys48b) {
    for (int8_t round = 15; round >= 0; --round) {
        round_feistel_cipher(N1, N2, keys48b[round]);
    }
    swap(N1, N2);
}

void des_message_read(des_message *msg, const char *file_path) {
    FILE *fp = fopen(file_path, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file %s\n", file_path);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    size_t actual_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    size_t aligned_len = actual_len % 8 == 0 ? actual_len : actual_len + (8 - (actual_len % 8));
    
    msg->data = (uint8_t *)malloc(aligned_len);
    if (!msg->data) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(fp);
        exit(1);
    }

    memset(msg->data, 0, aligned_len);
    if (fread(msg->data, 1, actual_len, fp) != actual_len) {
        fprintf(stderr, "Error: Failed to read file\n");
        free(msg->data);
        msg->data = NULL;
        fclose(fp);
        exit(1);
    }
    msg->data_len = aligned_len;

    fclose(fp);
}

void des_message_write(const des_message *msg, const char *file_path) {
    FILE *fp = fopen(file_path, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file %s\n", file_path);
        exit(1);
    }

    fwrite(msg->data, 1, msg->data_len, fp);
    fclose(fp);
}

void des_message_free(des_message *msg) {
    if (msg->data) {
        free(msg->data);
        msg->data = NULL;
    }
    msg->data_len = 0;
}

void des_encrypt(des_message *msg, const uint8_t *key) {
    uint8_t key_data[8];
    memcpy(key_data, key, 8);

    uint64_t keys48b[16] = {0};
    des_key_expansion(des_join_8bits_to_64bits(key_data), keys48b);

    uint32_t N1, N2;

    for (size_t i = 0; i < msg->data_len; i += 8) {
        des_split_64bits_to_32bits(
            des_initial_permutation(
                des_join_8bits_to_64bits(msg->data + i)
            ),
            &N1, &N2
        );
        feistel_encrypt(&N1, &N2, keys48b);
        des_split_64bits_to_8bits(
            des_final_permutation(
                des_join_32bits_to_64bits(N1, N2)
            ),
            (msg->data + i)
        );
    }
}

void des_decrypt(des_message *msg, const uint8_t *key) {
    uint8_t key_data[8];
    memcpy(key_data, key, 8);

    uint64_t keys48b[16] = {0};
    des_key_expansion(des_join_8bits_to_64bits(key_data), keys48b);

    uint32_t N1, N2;

    for (size_t i = 0; i < msg->data_len; i += 8) {
        des_split_64bits_to_32bits(
            des_initial_permutation(
                des_join_8bits_to_64bits(msg->data + i)
            ),
            &N1, &N2
        );
        feistel_decrypt(&N1, &N2, keys48b);
        des_split_64bits_to_8bits(
            des_final_permutation(
                des_join_32bits_to_64bits(N1, N2)
            ),
            (msg->data + i)
        );
    }
}
