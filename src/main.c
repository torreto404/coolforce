#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "hash_algo.h"

void print_help(const char *progname)
{
    printf("Usage: %s -a <alg> -h <hash> -w <wordlist>\n", progname);
    printf("Options:\n");
    printf("  -a <alg>    Hash algorithm (md5)\n");
    printf("  -h <hash>   Target hash to crack\n");
    printf("  -w <file>   Path to wordlist file\n");
    printf("\nExample:\n");
    printf("  %s -a md5 -h 5d41402abc4b2a76b9719d911017c592 -w rockyou.txt\n", progname);
}

int hex_to_bin(const char *hex, unsigned char *bin, size_t bin_len)
{
    if (strlen(hex) != bin_len * 2) {
        return -1;
    }

    for (size_t i = 0; i < bin_len; i++) {
        char byte_str[3] = {hex[i*2], hex[i*2+1], '\0'};
        bin[i] = (unsigned char)strtol(byte_str, NULL, 16);
    }

    return 0;
}

int main(int argc, char *argv[])
{
	char* algorithm = NULL;
	char* hash = NULL;
	char* wordlist = NULL;
	int rez = 0;

	opterr = 0;

	while((rez = getopt(argc, argv, "a:h:w:")) != -1)
	{
		switch (rez) {
		case 'a': algorithm = optarg; break;
		case 'h': hash = optarg; break;
		case 'w': wordlist = optarg; break;
		case '?': printf("Error found !\n"); return 1;
		}
	}

	if(algorithm == NULL || hash == NULL || wordlist == NULL)
	{
		print_help(argv[0]);
		return 1;
	}

	if(strcmp(algorithm, "md5") != 0)
	{
		printf("Error: unsupported algorithm '%s'. Only 'md5' is supported.\n", algorithm);
		return 1;
	}

	const Hash_algo *algo = hash_algo_find(algorithm);

	unsigned char target_bin[32];
    	if (hex_to_bin(hash, target_bin, algo->hash_len) != 0) {
        	printf("Error: failed to convert hash to binary\n");
        	return 1;
    	}

	FILE* fp;

	fp = fopen(wordlist, "r");

	if (fp == NULL)
	{
		printf("Error open file.\n");
		return 1;
	}

	char* line = NULL;
	size_t len = 0;
	ssize_t nread;
    	unsigned char hash_bin[32];
    	int found = 0;

	printf("Starting brute-force with %s...\n", algo->name);

	while ((nread = getline(&line, &len, fp)) != -1) {

        if (nread > 0 && line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
            nread--;
        }

        if (nread == 0) {
            continue;
        }

        algo->compute_bin((const unsigned char *)line, nread, hash_bin);

        if (memcmp(hash_bin, target_bin, algo->hash_len) == 0) {
            printf("\n[+] Password found!\n");
            printf("    Word: %s\n", line);
            printf("    Hash: %s\n", hash);
            found = 1;
            break;
        }
    }

    fclose(fp);
    if (line) {
        free(line);
    }

    if (!found) {
        printf("\n[-] Password not found in wordlist.\n");
    }

    return 0;
}
