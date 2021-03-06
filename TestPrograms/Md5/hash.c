//Compile with gcc hash.c -lcrypto -o salida
#include<openssl/md5.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
int main(){

    int n;
    MD5_CTX c;
    char buf[7]={"HOLA"};
    ssize_t bytes;
    unsigned char out[MD5_DIGEST_LENGTH];

    MD5_Init(&c);
    bytes = strlen(buf);
    MD5_Update(&c, buf, bytes);
    MD5_Final(out,&c);

    for(n=0; n<MD5_DIGEST_LENGTH; n++)
        printf("%02x", out[n]);
    
    printf("\n");
    return(0);
}
