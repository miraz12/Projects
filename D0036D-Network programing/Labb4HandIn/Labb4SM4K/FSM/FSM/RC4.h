#pragma once
#include <string>

class RC4
{
public:
	RC4(const char* key, size_t length);
	~RC4();
	///Encrypts buffer
	char* Encrypt(char* text, size_t length);
	///Decrypts buffer
	char* Decrypt(char* cipher, size_t length);

private:
	///Pseudo-random generation algorithm
	char* PRGA(char* text, size_t length);
	/// key-scheduling algorithm
	void KSA(const char* key, size_t length);
	const char* key;
private:
	unsigned char S[256];
};