#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

    //  Local includes
#include "../include/rsa.h"

#define TRUE    1
#define FALSE   0
//Variables used in authentication section
#define CLIENT_CODE "CLIENT"
#define SERVER_CODE "SERVER"
    // Servers status
#define ENVIO_RANDOM_ID 0
#define RECEIVE_PC_CODE 1
    // Defines Encryption
#define PATH_PRIVATE_KEY_CLIENT     "keys/privateClientKey.pem"
#define PATH_PUBLIC_KEY_CLIENT      "keys/publicClientKey.pem"
#define PATH_PRIVATE_KEY_SERVER     "keys/privateServerKey.pem"
#define PATH_PUBLIC_KEY_SERVER      "keys/publicServerKey.pem"
    // Variables used for encryption
char dataBuffer[2048]={};
unsigned char encryptedClient[2048]={};
unsigned char decryptedClient[2048]={};
int encrypted_length = 0;
int decrypted_length = 0;
unsigned char encryptedServer[2048]={};
unsigned char decryptedServer[2048]={};
    // Variables used in FSM
char bufferRx[255];
char statusSwServer = ENVIO_RANDOM_ID;
char ClientCode[10];
int  contWrite = 0;
int  tempContWrite = 0;

int main(int argc, char *argv[]){
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    //time_t ticks; 
    long randomID=0;
    long randomPC=0;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //serv_addr.sin_addr.s_addr = inet_addr("192.168.7.2");
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
    
    char canRead = 0;
    unsigned char ContinueFSMDoWhile = TRUE;
    while(1){
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        ContinueFSMDoWhile = TRUE;
        printf("conexion aceptada :)\n");
        // Identifier server FSM
        do{
            if(canRead){
                printf("Before read server\n");
                read(connfd,bufferRx,255); // <== read
                printf("server after read\n"); fflush(stdout);
                //printf("After read server:[%s]\n",bufferRx);
            }else{
                canRead = TRUE;
            }
            switch(statusSwServer){
                case ENVIO_RANDOM_ID:{ // 1.
                    printf("1. CASE 1\n"); fflush(stdout);
                    srand( time(NULL) );
                    randomID = rand() % 16777215;
                    printf("1. randomID from server:[%li]\n",randomID);
                        // char conversion to encrypt
                    sprintf(dataBuffer,"%li",randomID); 
                    int encrypted_length = public_encrypt((unsigned char *)dataBuffer,strlen(dataBuffer),(unsigned char *)(PATH_PUBLIC_KEY_CLIENT) ,encryptedServer);
                    if(encrypted_length == -1){
                        printLastError("Public Encrypt failed\n");
                        exit(0);
                    }
                    printf("1. Encrypted length = [%d]\n",encrypted_length);
                    contWrite = 0;
                    tempContWrite = 0;
                    do{
                        if( (tempContWrite = write(connfd,encryptedServer,encrypted_length)) > 0 ){
                            contWrite += tempContWrite;
                            printf("1. written:[%d]\n",contWrite);
                       }else{
                            printf("1. Error writting\n");
                        }
                    }while(contWrite < encrypted_length);

                    statusSwServer = RECEIVE_PC_CODE;
                    break;
                }
                case RECEIVE_PC_CODE:{ // 3.
                    char * pch;
                    char cont = 0;
                    long randomIDTemp = 0;
                    printf("3. CASE 3\n"); fflush(stdout);

                    // Decrypt info
                    decrypted_length = private_decrypt((unsigned char *)(bufferRx),strlen(bufferRx),(unsigned char *)(PATH_PRIVATE_KEY_SERVER),decryptedClient);
                    if(decrypted_length == -1){
                        printLastError("Private Decrypt failed");
                        exit(0);
                    }
                    decryptedClient[decrypted_length] = '\0';
                    printf("3. Decrypted Length = [%d]\n",decrypted_length);
                    printf("3. Decrypted Text = [%s]\n",decryptedClient);
                    strcpy(bufferRx,(char *)(decryptedClient));

                    printf ("3. Splitting string \"%s\" into tokens:\n",bufferRx);
                    pch = strtok (bufferRx,"$");
                    while (pch != NULL){
                        printf ("%s\n",pch);
                        switch(cont){
                            case 0:{
                                strcpy(ClientCode,pch);
                                break;
                            }
                            case 1:{
                                randomIDTemp = atol(pch);
                                break;
                            }
                            case 2:{
                                randomPC = atol(pch);
                                break;
                            }
                            default:{
                                printf("RECEIVE_PC_CODE SWITCH: It shouldn't be here\n");
                            }
                        }
                        cont++;
                        pch = strtok (NULL,"$");
                    }
                    // Validacion de la información llegada.                        
                    if( strstr(CLIENT_CODE,ClientCode) != NULL ){
                        printf("3. Ok server client code\n");
                    }else{
                        printf("3. ClientCode no OK :(\n");
                    }
                    if( randomIDTemp == randomID ){
                        printf("3. Ok server randomID\n");
                    }else{
                        printf("3. randomID no OK :(\n");
                    }                   
                    sprintf(bufferRx,"%s$%li$%li",SERVER_CODE,randomID,randomPC);
                    
                    encrypted_length = public_encrypt((unsigned char *)bufferRx,strlen(bufferRx),(unsigned char *)(PATH_PUBLIC_KEY_CLIENT) ,encryptedServer);
                    if(encrypted_length == -1){
                        printLastError("Public Encrypt failed ");
                        exit(0);
                    }
                    contWrite = 0;
                    tempContWrite = 0;
                    do{
                        if( (tempContWrite = write(connfd,encryptedServer,encrypted_length)) > 0 ){
                            contWrite += tempContWrite;
                            printf("3. written:[%d]\n",contWrite);
                        }else{                          
                            printf("3. Error writting\n");
                        }
                    }while(contWrite < encrypted_length);

                    ContinueFSMDoWhile = FALSE;
                    break;
                }
                default:{
                    printf("Default server fsm: It shouldn't be here\n");
                }
            }
        }while(ContinueFSMDoWhile);
        printf("End FSM, waiting new connection.\n");
        ContinueFSMDoWhile = TRUE;
        canRead = FALSE;
        statusSwServer = ENVIO_RANDOM_ID;
        // Identifier server FSM
        sleep(1);
     }
}
