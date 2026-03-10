#include "test_vectors.h"
#include "des.h"
#include <stdio.h>
#include <string.h>

static const struct {
    uint8_t key[8];
    uint8_t plaintext[8];
} TEST_CASES[] = {
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}},
    {{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}, {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}},
    {{0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xE1}, {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77}},
    {{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11}, {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10}},
};

static const size_t NUM_TESTS = sizeof(TEST_CASES) / sizeof(TEST_CASES[0]);

static void print_hex(const char *label, const uint8_t *data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int des_run_tests(void) {
    int passed = 0;
    int failed = 0;

    printf("Running DES self-consistency tests (%zu cases)...\n\n", NUM_TESTS);

    for (size_t t = 0; t < NUM_TESTS; t++) {
        const uint8_t *key = TEST_CASES[t].key;
        const uint8_t *pt = TEST_CASES[t].plaintext;

        uint8_t encrypted[8];
        memcpy(encrypted, pt, 8);
        des_message msg_enc = {.data = encrypted, .data_len = 8};
        des_encrypt(&msg_enc, key);

        uint8_t decrypted[8];
        memcpy(decrypted, encrypted, 8);
        des_message msg_dec = {.data = decrypted, .data_len = 8};
        des_decrypt(&msg_dec, key);

        int roundtrip_ok = (memcmp(decrypted, pt, 8) == 0);

        if (roundtrip_ok) {
            printf("Test %zu: PASS\n", t + 1);
            passed++;
        } else {
            printf("Test %zu: FAIL\n", t + 1);
            print_hex("  Key", key, 8);
            print_hex("  Plaintext", pt, 8);
            print_hex("  Ciphertext", encrypted, 8);
            print_hex("  Decrypted", decrypted, 8);
            failed++;
        }

        printf("  Key:        ");
        for (int i = 0; i < 8; i++) printf("%02X", key[i]);
        printf("\n");

        printf("  Plaintext:  ");
        for (int i = 0; i < 8; i++) printf("%02X", pt[i]);
        printf("\n");

        printf("  Ciphertext: ");
        for (int i = 0; i < 8; i++) printf("%02X", encrypted[i]);
        printf("\n\n");
    }

    printf("Results: %d passed, %d failed\n", passed, failed);
    return (failed == 0) ? 0 : 1;
}

int main(void) {
    return des_run_tests();
}
