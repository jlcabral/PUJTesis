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
#define SERVER_CODE "SERVER"
    // Servers status
#define ENVIO_RANDOM_ID 0
    // Variables used
unsigned char bufferRx[255];
unsigned char statusSwServer = ENVIO_RANDOM_ID;


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
                case ENVIO_RANDOM_ID:{
                    srand( time(NULL) );
                    randomID = rand() % 16777215;
                    printf("randomID from server:[%li]\n",randomID);
                    write(connfd,&randomID,3);
                    break;
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
