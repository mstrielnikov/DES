#ifndef DES_KEY_H
#define DES_KEY_H

#include <stdint.h>

void des_key_expansion(uint64_t key64b, uint64_t *keys48b);

#endif
