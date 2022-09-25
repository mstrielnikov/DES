#ifndef MESSAGGEIO_H
#define MESSAGGEIO_H

#include "messageio.h"
#include "key.h"

class DES{
  public:
    DES(Message message, const char * key_file);
    ~DES() = default;

    Message encrypt();
    Message decrypt();

  private:
    static void feistel_decrypt(uint32_t * N1, uint32_t * N2, uint64_t * keys48b);
    static void feistel_encrypt(uint32_t * N1, uint32_t * N2, uint64_t * keys48b);
    static void round_feistel_cipher(uint32_t * N1, uint32_t * N2, uint64_t key48b);
    static void feistel_cipher(uint32_t * N1, uint32_t * N2, uint64_t * keys48b);
    static void swap(uint32_t * N1, uint32_t * N2);
    static void feistel_round(uint32_t * N1, uint32_t * N2, uint64_t key48b);
    static uint32_t magic_fn(uint32_t block32b, uint64_t key48b);

    uint8_t * key;
    uint8_t * message;
    size_t cipher_len;

    uint64_t keys48b[16] = {0};
    uint32_t N1, N2;
};

DES :: DES(Message msg, char const * key_file){
  this->key = (uint8_t*)msg.read_txt(key_file).c_str();
  this->message = msg.data;
  this->cipher_len = msg.data_len;

  key_expansion(
      join_8bits_to_64bits(this->key),
      this->keys48b
  );
}

Message DES :: encrypt(){
  for (size_t i = 0; i < this->cipher_len; i += 8) {
      split_64bits_to_32bits(
          initial_permutation(
              join_8bits_to_64bits(this->message + i)
          ),
          &this->N1, &this->N2
      );
      feistel_encrypt(&this->N1, &this->N2, this->keys48b);
      split_64bits_to_8bits(
          final_permutation(
              join_32bits_to_64bits(this->N1, this->N2)
          ),
          (this->message + i)
      );
  }
}

Message DES :: decrypt(){
  for (size_t i = 0; i < this->cipher_len; i += 8) {
      split_64bits_to_32bits(
          initial_permutation(
              join_8bits_to_64bits(this->message + i)
          ),
          &this->N1, &this->N2
        );
      feistel_decrypt(&this->N1, &this->N2, this->keys48b);
      split_64bits_to_8bits(
          final_permutation(
              join_32bits_to_64bits(this->N1, this->N2)
          ),
          (this->message + i)
      );
  }
}

void DES :: feistel_encrypt(uint32_t * N1, uint32_t * N2, uint64_t * keys48b) {
  for (int8_t round = 0; round < 16; ++round) {
      round_feistel_cipher(N1, N2, keys48b[round]);
  }
  swap(N1, N2);
}

void DES :: feistel_decrypt(uint32_t * N1, uint32_t * N2, uint64_t * keys48b) {
  for (int8_t round = 15; round >= 0; --round) {
      round_feistel_cipher(N1, N2, keys48b[round]);
  }
  swap(N1, N2);
}

uint32_t DES :: magic_fn(uint32_t block32b, uint64_t key48b) {
    uint64_t block48b = expansion_permutation(block32b);
    block48b ^= key48b;
    block32b = substitutions(block48b);
    return permutation(block32b);
}

void DES :: feistel_round(uint32_t * N1, uint32_t * N2, uint64_t key48b) {
    uint32_t temp = *N2;
    *N2 = magic_fn(*N2, key48b) ^ *N1;
    *N1 = temp;
}

void DES :: swap(uint32_t * N1, uint32_t * N2) {
    uint32_t temp = *N1;
    *N1 = *N2;
    *N2 = temp;
}

#endif