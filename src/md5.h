#ifndef MD5_H
#define MD5_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
	uint32_t state[4];
	uint32_t count[2];
	unsigned char buffer[64]; 
} MD5_CTX;

void md5_init(MD5_CTX *context);
void md5_update(MD5_CTX *context, const unsigned char *data, size_t len);
void md5_final(unsigned char digest[16], MD5_CTX *context);

#endif
