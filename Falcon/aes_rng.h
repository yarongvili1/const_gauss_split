#include<stdio.h>
#include <stdint.h>     //for int8_t
#include <string.h>     //for memcmp
#include <wmmintrin.h>  //for intrinsics for AES-NI
#include <unistd.h>
#include <fcntl.h>

#define aes_buf_size 512
uint8_t aes_buf[aes_buf_size];
int32_t aes_buf_pointer;

void aes_random_bytes_init(void);

void aes_random_bytes(uint8_t *data);

void aes_fill_buffer();

static inline uint64_t aes_get_u64();

static inline uint8_t aes_get_u8();
