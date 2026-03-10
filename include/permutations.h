#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

#include <stdint.h>
#include <stddef.h>

uint32_t des_substitutions(uint64_t block48b);
void des_substitution_6bits_to_4bits(uint8_t *blocks6b, uint8_t *blocks4b);
uint8_t des_extreme_bits(uint8_t block6b);
uint8_t des_middle_bits(uint8_t block6b);

uint32_t des_permutation(uint32_t block32b);
uint64_t des_expansion_permutation(uint32_t block32b);
uint64_t des_initial_permutation(uint64_t block64b);
uint64_t des_final_permutation(uint64_t block64b);

void des_split_64bits_to_32bits(uint64_t block64b, uint32_t *block32b_1, uint32_t *block32b_2);
void des_split_64bits_to_8bits(uint64_t block64b, uint8_t *blocks8b);
void des_split_48bits_to_6bits(uint64_t block48b, uint8_t *blocks6b);

uint64_t des_join_32bits_to_64bits(uint32_t block32b_1, uint32_t block32b_2);
uint64_t des_join_28bits_to_56bits(uint32_t block28b_1, uint32_t block28b_2);
uint64_t des_join_8bits_to_64bits(uint8_t *blocks8b);
uint32_t des_join_4bits_to_32bits(uint8_t *blocks4b);

extern const uint8_t DES_SBOX[8][4][16];
extern const uint8_t DES_IP[64];
extern const uint8_t DES_FP[64];
extern const uint8_t DES_K1P[28];
extern const uint8_t DES_K2P[28];
extern const uint8_t DES_CP[48];
extern const uint8_t DES_EP[48];
extern const uint8_t DES_P[32];

#endif
