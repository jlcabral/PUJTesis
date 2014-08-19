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

#define TRUE    1
#define FALSE   0
//Variables used in authentication section
#define CLIENT_CODE "CLIENT"
#define SERVER_CODE "SERVER"
    // Servers status
#define ENVIO_RANDOM_ID 0
#define RECEIVE_PC_CODE 1
    // Variables used
unsigned char bufferRx[255];
unsigned char statusSwServer = ENVIO_RANDOM_ID;
unsigned char ClientCode[10];

int main(int argc, char *argv[]){
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 
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

    while(1){
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        printf("conexion aceptada :)\n");
        // Identifier server FSM
        do{
            if(canRead){
                read(connfd,bufferRx,255); // <== read
            }else{
                canRead = TRUE;    
            }
            switch(statusSwServer){
                case ENVIO_RANDOM_ID:{ // 1.
                    srand( time(NULL) );
                    randomID = rand() % 16777215;
                    printf("randomID from server:[%li]\n",randomID);
                    write(connfd,&randomID,3);
                    statusSwServer = RECEIVE_PC_CODE;
                    break;
                }
                case RECEIVE_PC_CODE:{ // 3.
                    char * pch;
                    char cont = 0;
                    long randomIDTemp = 0;
                    printf ("Splitting string \"%s\" into tokens:\n",bufferRx);
                    pch = strtok (bufferRx,"$");
                    while (pch != NULL){
                        printf ("%s\n",pch);
                        switch(cont){
                            case 0:{
                                strcpy(ClientCode,pch);
                                break;
                            }
                            case 1:{
                                randomIDTemp = atoi(pch);
                                break;
                            }
                            case 2:{
                                randomPC = atoi(pch);
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
                        printf("Ok server client code\n");
                    }else{
                        printf("ClientCode no OK :(\n");
                    }
                    if( randomIDTemp != randomID ){
                        printf("Ok server randomID\n");
                    }else{
                        printf("randomID no OK :(\n");
                    }
                    break;
                    
                    sprintf(bufferRx,"%s$%i$%i",SERVER_CODE,randomID,randomPC); 
                    write(connfd,bufferRx,strlen(bufferRx));
                    //statusSwServer =
                } 
                default:{
                    printf("Default server fsm: It shouldn't be here\n");
                }
            }
        }while(1);


        // Identifier server FSM

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));
        close(connfd);
        sleep(1);
     }
}
