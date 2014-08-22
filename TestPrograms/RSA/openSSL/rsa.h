#ifndef RSA_H
#define RSA_H

int public_encrypt(unsigned char * data,int data_len,unsigned char * keyPath, unsigned char *encrypted);
int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * keyPath, unsigned char *decrypted);
int private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted);
int public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted);
void printLastError(char *msg);

#endif
