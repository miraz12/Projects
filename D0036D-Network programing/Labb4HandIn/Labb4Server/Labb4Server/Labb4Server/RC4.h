#pragma once
#include <string>

class RC4
{
public:
	RC4(const char* key, size_t length);
	RC4();
	~RC4();
	///Encrypt
	char* Encrypt(char* text, size_t length);
	///Decrypt
	char* Decrypt(char* cipher, size_t length);

private:
	void KSA(const char* key, size_t length);
	char* PRGA(char* text, size_t length);
	const char* key;
private:
	unsigned char S[256];
};