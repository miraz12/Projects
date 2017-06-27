#include "RC4.h"
#include <stdio.h>
#include <string.h>

RC4::RC4()
{
	
}

RC4::RC4(const char* key, size_t length) : key(key)
{
	KSA(key, length);
}

RC4::~RC4()
{
	memset(S, 0, 256);
	delete key;
}

char* RC4::Encrypt(char* text, size_t length)
{
	return PRGA(text, length);
}

char* RC4::Decrypt(char* cipher, size_t length)
{
	return PRGA(cipher, length);
}

void RC4::KSA(const char* key, size_t length)
{
	for (size_t i = 0; i < 256; i++)
		S[i] = i;

	size_t j = 0;

	for (size_t i = 0; i < 256; i++)
	{
		j = (j + S[i] + key[i % length]) % 256;
		std::swap(S[i], S[j]);
	}
}

char* RC4::PRGA(char* text, size_t length)
{
	unsigned char k;
	size_t j = 0;
	size_t i = 0;

	for (size_t index = 0; index < length; index++)
	{
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		std::swap(S[i], S[j]);
		k = S[(S[i] + S[j]) % 256];
		text[index] ^= k;
	}

	return text;
}