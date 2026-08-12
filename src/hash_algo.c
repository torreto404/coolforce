#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hash_algo.h"
#include "md5.h"

void md5_compute_bin(const unsigned char *input, size_t len, unsigned char *output)
{
	unsigned char digest[16];

	MD5_CTX ctx;

	md5_init(&ctx);
	md5_update(&ctx, input, len);
	md5_final(digest, &ctx);

	memcpy(output, digest, 16);
}

static char *md5_compute_hex(const unsigned char *input, size_t len)
{
	unsigned char digest[16];

	char *output = malloc(33);
	
	if (output == NULL) {
        	return NULL;
    	}

    	md5_compute_bin(input, len, digest);

    	for (int i = 0; i < 16; i++) {
        	sprintf(output + (i * 2), "%02x", digest[i]);
    	}
    	output[32] = '\0';

    	return output;
}

static const Hash_algo algorithms[] = {
    {
        .name = "md5",
        .hash_len = 16,
        .hash_hex_len = 32,
        .compute_bin = md5_compute_bin,
        .compute_hex = md5_compute_hex
    }
};

static const size_t NUM_ALGORITHMS = sizeof(algorithms) / sizeof(algorithms[0]);

const Hash_algo *hash_algo_find(const char *name)
{
    if (name == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < NUM_ALGORITHMS; i++) {
        if (strcmp(algorithms[i].name, name) == 0) {
            return &algorithms[i];
        }
    }

    return NULL;
}

const char **hash_algo_list(size_t *count)
{
    static const char *names[10];
    
    if (count == NULL) {
        return NULL;
    }

    *count = NUM_ALGORITHMS;

    for (size_t i = 0; i < NUM_ALGORITHMS && i < 10; i++) {
        names[i] = algorithms[i].name;
    }

    return names;
}
