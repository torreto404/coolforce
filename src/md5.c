#include <string.h>

#include "md5.h"

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-n)))

static const uint32_t T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static unsigned char PADDING[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static void md5_process_block(MD5_CTX *context, const unsigned char *data)
{
	uint32_t a = context->state[0];
	uint32_t b = context->state[1];
	uint32_t c = context->state[2];
	uint32_t d = context->state[3];

	uint32_t M[16];
	for (size_t i = 0; i < 16; i++)
	{
		M[i] = ((uint32_t)data[i*4]) | ((uint32_t)data[i*4+1] << 8) | ((uint32_t)data[i*4+2] << 16) | ((uint32_t)data[i*4+3] << 24);
	}

	// ROUND 1
	a = b + ROTATE_LEFT((a + F(b,c,d) + M[0] + T[0]), S11);
	d = a + ROTATE_LEFT((d + F(a,b,c) + M[1] + T[1]), S12);
	c = d + ROTATE_LEFT((c + F(d,a,b) + M[2] + T[2]), S13);
	b = c + ROTATE_LEFT((b + F(c,d,a) + M[3] + T[3]), S14);
	a = b + ROTATE_LEFT((a + F(b,c,d) + M[4] + T[4]), S11);
	d = a + ROTATE_LEFT((d + F(a,b,c) + M[5] + T[5]), S12);
	c = d + ROTATE_LEFT((c + F(d,a,b) + M[6] + T[6]), S13);
	b = c + ROTATE_LEFT((b + F(c,d,a) + M[7] + T[7]), S14);
	a = b + ROTATE_LEFT((a + F(b,c,d) + M[8] + T[8]), S11);
	d = a + ROTATE_LEFT((d + F(a,b,c) + M[9] + T[9]), S12);
	c = d + ROTATE_LEFT((c + F(d,a,b) + M[10] + T[10]), S13);
	b = c + ROTATE_LEFT((b + F(c,d,a) + M[11] + T[11]), S14);
	a = b + ROTATE_LEFT((a + F(b,c,d) + M[12] + T[12]), S11);
	d = a + ROTATE_LEFT((d + F(a,b,c) + M[13] + T[13]), S12);
	c = d + ROTATE_LEFT((c + F(d,a,b) + M[14] + T[14]), S13);
	b = c + ROTATE_LEFT((b + F(c,d,a) + M[15] + T[15]), S14);

	//ROUND 2
	a = b + ROTATE_LEFT((a + G(b,c,d) + M[1] + T[16]), S21);
	d = a + ROTATE_LEFT((d + G(a,b,c) + M[6] + T[17]), S22);
	c = d + ROTATE_LEFT((c + G(d,a,b) + M[11] + T[18]), S23);
	b = c + ROTATE_LEFT((b + G(c,d,a) + M[0] + T[19]), S24);
	a = b + ROTATE_LEFT((a + G(b,c,d) + M[5] + T[20]), S21);
	d = a + ROTATE_LEFT((d + G(a,b,c) + M[10] + T[21]), S22);
	c = d + ROTATE_LEFT((c + G(d,a,b) + M[15] + T[22]), S23);
	b = c + ROTATE_LEFT((b + G(c,d,a) + M[4] + T[23]), S24);
	a = b + ROTATE_LEFT((a + G(b,c,d) + M[9] + T[24]), S21);
	d = a + ROTATE_LEFT((d + G(a,b,c) + M[14] + T[25]), S22);
	c = d + ROTATE_LEFT((c + G(d,a,b) + M[3] + T[26]), S23);
	b = c + ROTATE_LEFT((b + G(c,d,a) + M[8] + T[27]), S24);
	a = b + ROTATE_LEFT((a + G(b,c,d) + M[13] + T[28]), S21);
	d = a + ROTATE_LEFT((d + G(a,b,c) + M[2] + T[29]), S22);
	c = d + ROTATE_LEFT((c + G(d,a,b) + M[7] + T[30]), S23);
	b = c + ROTATE_LEFT((b + G(c,d,a) + M[12] + T[31]), S24);

	// ROUND 3
	a = b + ROTATE_LEFT((a + H(b,c,d) + M[5] + T[32]), S31);
	d = a + ROTATE_LEFT((d + H(a,b,c) + M[8] + T[33]), S32);
	c = d + ROTATE_LEFT((c + H(d,a,b) + M[11] + T[34]), S33);
	b = c + ROTATE_LEFT((b + H(c,d,a) + M[14] + T[35]), S34);
	a = b + ROTATE_LEFT((a + H(b,c,d) + M[1] + T[36]), S31);
	d = a + ROTATE_LEFT((d + H(a,b,c) + M[4] + T[37]), S32);
	c = d + ROTATE_LEFT((c + H(d,a,b) + M[7] + T[38]), S33);
	b = c + ROTATE_LEFT((b + H(c,d,a) + M[10] + T[39]), S34);
	a = b + ROTATE_LEFT((a + H(b,c,d) + M[13] + T[40]), S31);
	d = a + ROTATE_LEFT((d + H(a,b,c) + M[0] + T[41]), S32);
	c = d + ROTATE_LEFT((c + H(d,a,b) + M[3] + T[42]), S33);
	b = c + ROTATE_LEFT((b + H(c,d,a) + M[6] + T[43]), S34);
	a = b + ROTATE_LEFT((a + H(b,c,d) + M[9] + T[44]), S31);
	d = a + ROTATE_LEFT((d + H(a,b,c) + M[12] + T[45]), S32);
	c = d + ROTATE_LEFT((c + H(d,a,b) + M[15] + T[46]), S33);
	b = c + ROTATE_LEFT((b + H(c,d,a) + M[2] + T[47]), S34);

	// ROUND 4
	a = b + ROTATE_LEFT((a + I(b,c,d) + M[0] + T[48]), S41);
	d = a + ROTATE_LEFT((d + I(a,b,c) + M[7] + T[49]), S42);
	c = d + ROTATE_LEFT((c + I(d,a,b) + M[14] + T[50]), S43);
	b = c + ROTATE_LEFT((b + I(c,d,a) + M[5] + T[51]), S44);
	a = b + ROTATE_LEFT((a + I(b,c,d) + M[12] + T[52]), S41);
	d = a + ROTATE_LEFT((d + I(a,b,c) + M[3] + T[53]), S42);
	c = d + ROTATE_LEFT((c + I(d,a,b) + M[10] + T[54]), S43);
	b = c + ROTATE_LEFT((b + I(c,d,a) + M[1] + T[55]), S44);
	a = b + ROTATE_LEFT((a + I(b,c,d) + M[8] + T[56]), S41);
	d = a + ROTATE_LEFT((d + I(a,b,c) + M[15] + T[57]), S42);
	c = d + ROTATE_LEFT((c + I(d,a,b) + M[6] + T[58]), S43);
	b = c + ROTATE_LEFT((b + I(c,d,a) + M[13] + T[59]), S44);
	a = b + ROTATE_LEFT((a + I(b,c,d) + M[4] + T[60]), S41);
	d = a + ROTATE_LEFT((d + I(a,b,c) + M[11] + T[61]), S42);
	c = d + ROTATE_LEFT((c + I(d,a,b) + M[2] + T[62]), S43);
	b = c + ROTATE_LEFT((b + I(c,d,a) + M[9] + T[63]), S44);

	context->state[0] += a;
	context->state[1] += b;
	context->state[2] += c;
	context->state[3] += d;

	memset(M, 0, sizeof(M));
}

void md5_init(MD5_CTX *context)
{
	context->state[0] = 0x67452301;
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;

	context->count[0] = 0;
	context->count[1] = 0;

	memset(context->buffer, 0, 64);
}

void md5_update(MD5_CTX *context, const unsigned char *data, size_t len)
{
	if (len == 0)
	{
		return;
	}
	size_t i;
	size_t part_len;
	size_t index;
	index = (context->count[0] >> 3) & 0x3F;

	uint32_t old_count = context->count[0];
	context->count[0] += (len << 3);
	if (context->count[0] < old_count)
	{
		context->count[1] += 1;
	}
	context->count[1] += (len >> 29);
	part_len = 64 - index;

	if (len >= part_len)
	{
		memcpy(context->buffer+index, data, part_len);
		md5_process_block(context, context->buffer);

		for (i = part_len; i + 63 < len; i+=64)
		{
			md5_process_block(context, data+i);
		}
		index = 0;
		memcpy(context->buffer, data+i, len - i);
	}
	else
	{
		memcpy(context->buffer+index, data, len);
	}
}

void md5_final(unsigned char digest[16], MD5_CTX *context)
{
	unsigned char bits[8];
	size_t index;
	size_t padLen;

	if (context == NULL)
	{
		return;
	}

	for (int i = 0; i < 4; i++)
    	{
        	bits[i]     = (unsigned char)(context->count[0] >> (i * 8));
        	bits[i + 4] = (unsigned char)(context->count[1] >> (i * 8));
	}

	index = (context->count[0] >> 3) & 0x3F;

        padLen = (index < 56) ? (56 - index) : (120 - index);
        md5_update(context, PADDING, padLen);
	md5_update(context, bits, 8);
        
	for (int i = 0; i < 4; i++)
    	{
        	digest[i*4 + 0] = (unsigned char)(context->state[i] & 0xFF);
        	digest[i*4 + 1] = (unsigned char)((context->state[i] >> 8) & 0xFF);
        	digest[i*4 + 2] = (unsigned char)((context->state[i] >> 16) & 0xFF);
        	digest[i*4 + 3] = (unsigned char)((context->state[i] >> 24) & 0xFF);
    	}

    	memset(context, 0, sizeof(MD5_CTX));

}
