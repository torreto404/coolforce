#ifndef HASH_ALGO_H
#define HASH_ALGO_H

typedef struct
{
	const char *name;
	size_t hash_len;
	size_t hash_hex_len;

	void (*compute_bin)(const unsigned char *input, size_t len, unsigned char *output);
    	char *(*compute_hex)(const unsigned char *input, size_t len);
} Hash_algo;

const Hash_algo *hash_algo_find(const char *name);

const char **hash_algo_list(size_t *count);

#endif
