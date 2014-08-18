#include<openssl/rsa.h>
#include<openssl/engine.h>
#include<string.h>
int main(){
    int numberBytes=0;
    unsigned char p[1024];
    unsigned char plaintext[11] = {"HOLA_MUNDO"};
    unsigned char *ciphertext = NULL;
    int bytesEncrypted=0;
    int i = 0;
    RSA *pRSA;
    pRSA = RSA_new();
    do{ 
        printf("It entered to key generation\n");
        pRSA = RSA_generate_key(1024,RSA_F4,NULL,NULL);
    }
    while( RSA_check_key(pRSA) != 1 );
    printf("Ok key generation\n");
    BN_bn2mpi(pRSA->p, p);
    printf("[%s]\n",p);
    //RSA_blinding_on(pRSA,NULL);
    
    numberBytes = RSA_size(pRSA);
    ciphertext = (char *)malloc(numberBytes);
    printf("RSA_size:[%d]\n",numberBytes);
    bytesEncrypted = RSA_public_encrypt(strlen(plaintext),
                        plaintext,ciphertext,pRSA,RSA_PKCS1_PADDING );
    printf("Dataencrypted\n");
    for(i=0;i<numberBytes;i++)
        printf("[0x%x]",ciphertext[i]);
    printf("\n");
    printf("1. encrypt:[%d]\n",bytesEncrypted);
    char decryptedText[128];
    numberBytes = RSA_private_decrypt(bytesEncrypted,ciphertext,decryptedText,pRSA,RSA_PKCS1_PADDING );
    decryptedText[numberBytes]='\0';
    printf("2. decrypted:[%d]\n",numberBytes);
    printf("DataClear:[%s]\n",decryptedText);

    RSA_free(pRSA);
    return 0; 
}
