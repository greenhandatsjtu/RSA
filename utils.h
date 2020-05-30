#ifndef RSA_UTILS_H
#define RSA_UTILS_H

void convertCharToNum(char *text, int x[]);

void convertTextToNum(int decodedNum[], const int textLen, const char sendText[]);

void convertNumToChar(int decodedNum[], int length);

void convertNumToText(const int encodedNum[], const int textLen, char sendText[]);

int Euclid(long long int m, long long int n);

bool isCoprime(long long m, long long n);

long long generatePrime();

int repeatMod(long long int m, int e, long long int n);

void RSA(char *text);

#endif //RSA_UTILS_H
