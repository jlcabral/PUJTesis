#include <openssl/md5.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <time.h>

#define TRUE            1
#define FALSE           0
#define PATH_APP        "/home/jlcabral/gitRepo/LocalPort/"
#define FILES_FOLDER    PATH_APP "filesClient/"
#define STRINGS_FILE    FILES_FOLDER "tablasString.txt"
#define FRMWR_FILE      FILES_FOLDER "EMBEDDED"
#define T2_FILE1        FILES_FOLDER "T2_F1_C14.jpg"
#define T2_FILE2        FILES_FOLDER "T2_F1_C15.jpg"
#define T2_FILE3        FILES_FOLDER "T2_F1_C16.jpg"
#define T3_FILE1        FILES_FOLDER "T3_F1_C15.jpg"
#define T3_FILE2        FILES_FOLDER "T3_F1_C16.jpg"
#define T3_FILE3        FILES_FOLDER "T3_F1_C17.jpg"
#define T3_HUELLA1      FILES_FOLDER "T3_huella_I1"
#define STRINGS_TO_TRANSFER  STRINGS_FILE " " 
#define T2_TO_TRANSFER  T2_FILE1 T2_FILE2 T2_FILE3 " "
#define T3_TO_TRANSFER  T3_FILE1 T3_FILE2 T3_FILE3 T3_HUELLA1 " "
#define T4_TO_TRANSFER_I1   FILES_FOLDER "T4_F1_C15.jpg " FILES_FOLDER "T4_F1_C16.jpg " FILES_FOLDER "T4_F1_C17.jpg " \
                            FILES_FOLDER "T4_F1_C18.jpg " FILES_FOLDER "T4_huella_I1 " 
#define T4_TO_TRANSFER_I2   FILES_FOLDER "T4_F2_C15.jpg " FILES_FOLDER "T4_F2_C16.jpg " FILES_FOLDER "T4_F2_C17.jpg " \
                            FILES_FOLDER "T4_F2_C18.jpg " FILES_FOLDER "T4_huella_I2 "
#define T4_TO_TRANSFER_I3   FILES_FOLDER "T4_F3_C15.jpg " FILES_FOLDER "T4_F3_C16.jpg " FILES_FOLDER "T4_F3_C17.jpg "\
                            FILES_FOLDER "T4_F3_C18.jpg " FILES_FOLDER "T4_huella_I3 "
#define T4_TO_TRANSFER_I4   FILES_FOLDER "T4_F4_C15.jpg " FILES_FOLDER "T4_F4_C16.jpg " FILES_FOLDER "T4_F4_C17.jpg "\
                            FILES_FOLDER "T4_F4_C18.jpg " FILES_FOLDER "T4_huella_I4 "
#define T4_TO_TRANSFER_I5   FILES_FOLDER "T4_F5_C15.jpg " FILES_FOLDER "T4_F5_C16.jpg " FILES_FOLDER "T4_F5_C17.jpg "\
                            FILES_FOLDER "T4_F5_C18.jpg " FILES_FOLDER "T4_huella_I5 "
#define T4_TO_TRANSFER_I6   FILES_FOLDER "T4_F6_C15.jpg " FILES_FOLDER "T4_F6_C16.jpg " FILES_FOLDER "T4_F6_C17.jpg "\
                            FILES_FOLDER "T4_F6_C18.jpg " FILES_FOLDER "T4_huella_I6 "
#define T4_TO_TRANSFER_I7   FILES_FOLDER "T4_F7_C15.jpg " FILES_FOLDER "T4_F7_C16.jpg " FILES_FOLDER "T4_F7_C17.jpg "\
                            FILES_FOLDER "T4_F7_C18.jpg " FILES_FOLDER "T4_huella_I7 "
#define T4_TO_TRANSFER_I8   FILES_FOLDER "T4_F8_C15.jpg " FILES_FOLDER "T4_F8_C16.jpg " FILES_FOLDER "T4_F8_C17.jpg "\
                            FILES_FOLDER "T4_F8_C18.jpg " FILES_FOLDER "T4_huella_I8 "
#define T4_TO_TRANSFER_I9   FILES_FOLDER "T4_F9_C15.jpg " FILES_FOLDER "T4_F9_C16.jpg " FILES_FOLDER "T4_F9_C17.jpg "\
                            FILES_FOLDER "T4_F9_C18.jpg " FILES_FOLDER "T4_huella_I9 "
#define T4_TO_TRANSFER_I10  FILES_FOLDER "T4_F10_C15.jpg " FILES_FOLDER "T4_F10_C16.jpg " FILES_FOLDER "T4_F10_C17.jpg "\
                            FILES_FOLDER "T4_F10_C18.jpg " FILES_FOLDER "T4_huella_I10 "

//Variables used in authentication section
#define CLIENT_CODE     "CLIENT"
    // client status
#define RECEIVE_RANDOM_ID   0
    //Variables used
unsigned char bufferClient[255];
unsigned char statusSwClient = RECEIVE_RANDOM_ID;

typedef struct tablasString tablasString; 
struct tablasString{
    char hay110;
    char hayT;
    char hayTabla1;
    char hayTabla2;
    char hayTabla3;
    char hayTabla4;
    char indexTabla4;
    char hayTabla5;
    char hayFirmware;
};

tablasString flagTablas;
long randomPC=0;
long randomID=0;
//Funcion existencia de archivos
int fileExist(char *filename){
    struct stat buffer;
    return ( stat(filename, &buffer) == 0);
};
//Variables para trabajo actualizacion tablas
int drivers = 0;
char contDriversIndex = 1;
char flagDrivers[11];

//comentario de prueba
int main(int argc, char *argv[]){
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 2){
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0){
        printf("\n inet_pton error occured\n");
        return 1;
    }
    //  1. ESTABLECIMIENTO DE CONEXION POR SOCKETS
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
       printf("\n Error : Connect Socket Failed \n");
       return 1;
    }
    // END 1. ESTABLECIMIENTO DE CONEXION POR SOCKETS
    
    // 2. OBTENER EL PASSWORD
    char *password;
    password = getpass("Enter password: ");
    printf("pass:[%s]\n",password);

    // END 2. OBTENER EL PASSWORD
    
    // 3. HASH del password
    int hashIndex;
    MD5_CTX contextMD5;
    ssize_t bytes;
    unsigned char outMD5[MD5_DIGEST_LENGTH];

    MD5_Init(&contextMD5);
    bytes = strlen(password);
    MD5_Update(&contextMD5, password, bytes);
    MD5_Final(outMD5,&contextMD5);
    for(hashIndex=0; hashIndex < MD5_DIGEST_LENGTH; hashIndex++)
        printf("%02x", outMD5[hashIndex]);
    
    printf("\n");
    if(strncmp(outMD5,"4dfd1f83ab93dddf6ee678c2b4eeedd0",32)){ //keyword embebidos
        printf("password ok\n");
    }else{
        printf("password error, the app will close\n");    
    }
    // END 3. HASH del password
    
    // 4. Recolección de la información a cargar
     
        // 4.1 Ingreso del string de control 
    char strControl[17];
    printf("Enter the control string: "); fflush(stdout);
    scanf("%17s",strControl);
    printf("The control string written is: [%s]\n",strControl);
        // 4.2 Caracterización del string ingresado (strControl)
    // 110
    if( (*strControl == '1') && (*(strControl+1)=='1') && 
        (*(strControl+2)=='0') ){
        printf("Codigo 110 Ok\n");
        flagTablas.hay110 = 1;
    }else{
        printf("Codigo 110 :(\n"); exit(1);
    }
    // $ [Separador]
    if( *(strControl+3)=='$' && *(strControl+5)=='$' ){
        printf("hay$s!\n");
    }else{
        printf("Error en los separadores. \n"); exit(1);
    }
    // Analisis Firmware
    if( *(strControl+4)=='F' || *(strControl+4)=='_' ){
        if(*(strControl+4)=='F'){
            flagTablas.hayFirmware = 1;
            //  FRMWR_FILE 
            if( !fileExist(FRMWR_FILE) ){
                printf("FRMWR_FILE No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
                printf("Operacion cancelada\n");
                exit(EXIT_FAILURE);
            }
        }else{
            flagTablas.hayFirmware = 0; 
        }        
        printf("hayFirmware:[%d]!\n",flagTablas.hayFirmware);
    }else{
        printf("error en simbolo firmware \n"); exit(1);
    }
    // Hay T
    if( *(strControl+6)=='T' || *(strControl+6)=='_' ){
        printf("Campo T valido\n");
        flagTablas.hayT = 1;
        if(*(strControl+6)=='T'){
            flagTablas.hayT = 1;
        }else{
            flagTablas.hayT = 0; 
        }
    }else{
        printf("Error, campo T no valido");
    }
    // Verificacion "hayTabla" y "hayFirmware"
    if( !flagTablas.hayT && !flagTablas.hayFirmware){
        printf("No existen tablas y firmware por actualizar. " \
                "Se cierra la conexión\n");
        exit(1);
    }
    // Hay tabla 1
    if( *(strControl+8)=='1' || *(strControl+8)=='_' ){
        printf("Campo Tabla 1 valido\n");
        flagTablas.hayTabla1 = 1;
        //  STRINGS_FILE 
        if( !fileExist(STRINGS_FILE) ){
            printf("STRINGS_FILE No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
    }else{
        printf("Error, campo tabla 1 no valido");    
    }
    // Hay tabla 2
    if( *(strControl+10)=='2' || *(strControl+10)=='_' ){
        printf("Campo Tabla 2 valido\n");
        flagTablas.hayTabla2 = 1;
        //  STRINGS_FILE 
        if( !fileExist(STRINGS_FILE) ){
            printf("STRINGS_FILE No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
        //  T2_FILE1 
        if( !fileExist(T2_FILE1) ){
            printf("T2_FILE1 No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
        //  T2_FILE2 
        if( !fileExist(T2_FILE2) ){
            printf("T2_FILE2 No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
        //  T2_FILE3 
        if( !fileExist(T2_FILE3) ){
            printf("T2_FILE3 No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
    }else{
        printf("Error, campo tabla 2 no valido");    
    }
    // Hay tabla 3
    if( *(strControl+12)=='3' || *(strControl+12)=='_' ){
        printf("Campo Tabla 3 valido\n");
        flagTablas.hayTabla3 = 1;
        //  STRINGS_FILE 
        if( !fileExist(STRINGS_FILE) ){
            printf("STRINGS_FILE No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
        //  T3_FILE1 
        if( !fileExist(T3_FILE1) ){
            printf("T3_FILE1 No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
        //  T3_FILE2 
        if( !fileExist(T3_FILE2) ){
            printf("T3_FILE2 No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
        //  T3_FILE3 
        if( !fileExist(T3_FILE3) ){
            printf("T3_FILE3 No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
        //  T3_huella
        if( !fileExist(T3_HUELLA1) ){
            printf("T3_FILE3 No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }
    }else{
        printf("Error, campo tabla 3 no valido");    
    }
    // Hay tabla 4
    if( *(strControl+14)=='4' || *(strControl+14)=='_' ){
        printf("Campo Tabla 4 valido\n");
        flagTablas.hayTabla4 = 1;
    }else{
        printf("Error, campo tabla 4 no valido"); 
    }
    // Hay tabla 5
    if( *(strControl+16)=='5' || *(strControl+16)=='_' ){
        printf("Campo Tabla 5 valido\n");
        flagTablas.hayTabla5 = 1;
        //  STRINGS_FILE 
        if( !fileExist(STRINGS_FILE) ){
            printf("STRINGS_FILE No encontrado, revisar que se encuentre en [%s]\n",FILES_FOLDER);
            printf("Operacion cancelada\n");
            exit(EXIT_FAILURE);
        }

    }else{
        printf("Error, campo tabla 5 no valido");    
    }
    //Validacion si hubo indicacion tabla 4 
    if( flagTablas.hayTabla4 == 1 ){
        printf("Ingresar el numero de conductores: "); fflush(stdout);
        scanf("%2s",strControl);
        //int drivers = 0;
        drivers = atoi(strControl);
        // Ingreso del número de conductores
        if( drivers>0 && drivers<11 ){
            flagTablas.indexTabla4 = 1;
            printf("drivers a registrar:[%d]\n",drivers);
        }else{
            printf("mal numero drivers\n"); exit(1);
        }
        // Ingreso index de los conductores
        //char flagDrivers[11];
        //char contDriversIndex = 1;
        do{
            printf("Enter the index to modify [%d/%d]: ",contDriversIndex,drivers); fflush(stdout);
            scanf("%2s",strControl);
            // Limite de numeros a ingresar
            if( atoi(strControl) >0 && atoi(strControl)<11 ){
                flagTablas.indexTabla4 = 1;
                printf("drivers a registrar:[%d]\n",drivers);
            }else{
                printf("mal numero drivers\n"); exit(1);
            }            
            flagDrivers[contDriversIndex] = atoi(strControl);
            contDriversIndex++;
        }
        while(contDriversIndex <= drivers);
        char i=0;
        for(i=1;i<=drivers;i++)
            printf("[%d]",flagDrivers[i]);
        printf("\n");
        //Revision de existencia archivos tabla 4
        char fileNameTempArray[50];
        char pathFiles[100];
        for(contDriversIndex=1;contDriversIndex<=drivers;contDriversIndex++){
            
            strcpy(pathFiles,FILES_FOLDER);
            sprintf( fileNameTempArray,"T4_F%d_C16.jpg",flagDrivers[contDriversIndex] );
            strcat(pathFiles,fileNameTempArray);
            printf("table4[%s]-driver#[%d]\n",fileNameTempArray,flagDrivers[contDriversIndex]);
            if( !fileExist(pathFiles) ){
                printf("[%s] No encontrado, revisar que se encuentre en [%s]\n",fileNameTempArray,FILES_FOLDER);
                printf("Operacion cancelada\n");
                exit(EXIT_FAILURE);
            }
           
            strcpy(pathFiles,FILES_FOLDER);
            sprintf( fileNameTempArray,"T4_F%d_C17.jpg",flagDrivers[contDriversIndex] );
            strcat(pathFiles,fileNameTempArray);
            printf("table4[%s]-driver#[%d]\n",fileNameTempArray,flagDrivers[contDriversIndex]);
            if( !fileExist(pathFiles) ){
                printf("[%s] No encontrado, revisar que se encuentre en [%s]\n",fileNameTempArray,FILES_FOLDER);
                printf("Operacion cancelada\n");
                exit(EXIT_FAILURE);
            }

            strcpy(pathFiles,FILES_FOLDER);
            sprintf( fileNameTempArray,"T4_F%d_C18.jpg",flagDrivers[contDriversIndex] );
            strcat(pathFiles,fileNameTempArray);
            printf("table4[%s]-driver#[%d]\n",fileNameTempArray,flagDrivers[contDriversIndex]);
            if( !fileExist(pathFiles) ){
                printf("[%s] No encontrado, revisar que se encuentre en [%s]\n",fileNameTempArray,FILES_FOLDER);
                printf("Operacion cancelada\n");
                exit(EXIT_FAILURE);
            }
            
            strcpy(pathFiles,FILES_FOLDER);
            sprintf( fileNameTempArray,"T4_huella_I%d",flagDrivers[contDriversIndex] );
            strcat(pathFiles,fileNameTempArray);
            printf("table4[%s]-driver#[%d]\n",fileNameTempArray,flagDrivers[contDriversIndex]);
            if( !fileExist(pathFiles) ){
                printf("[%s] No encontrado, revisar que se encuentre en [%s]\n",fileNameTempArray,FILES_FOLDER);
                printf("Operacion cancelada\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    //END  4. Recolección de la información a cargar
    
    // Authentication State
    do{
       printf("1. before read client\n");
       read(sockfd,bufferClient,255); // <== read
       printf("2. after read client\n");
       switch(statusSwClient){
        case RECEIVE_RANDOM_ID:{
            long *pLongBufferClient = 0;
            pLongBufferClient = (long *)(bufferClient);
            randomID = *pLongBufferClient;
            printf("[%li]\n",randomID);
            //generate random PC
            srand( time(NULL) );
            randomPC = rand() % 16777215;
            sprintf(bufferClient,"%s$%i$%i",CLIENT_CODE,randomID,randomPC); 
            write(sockfd,bufferClient,strlen(bufferClient));
            break;
        }
        default:{
            printf("Default client fsm: It shouldn't be here\n");
            break;
        }
       }
    }while(1);

    // END Authentication State
    
    //5. Transferencia de la informacion
    char sshCommandTransferFilesArray[400];
    password = getpass("Enter Identifier Device linux password: ");
    printf("pass:[%s]\n",password);

    strcpy(sshCommandTransferFilesArray,"sshpass -p ");
    strcat(sshCommandTransferFilesArray,password);
    strcat(sshCommandTransferFilesArray," scp ");
    if(flagTablas.hayTabla1 | flagTablas.hayTabla5){
        strcat(sshCommandTransferFilesArray,STRINGS_TO_TRANSFER);
    }
    if(flagTablas.hayTabla2){
        strcat(sshCommandTransferFilesArray,T2_TO_TRANSFER);
    }
    if(flagTablas.hayTabla3){
        strcat(sshCommandTransferFilesArray,T3_TO_TRANSFER);
    }
    if(flagTablas.hayTabla4){
        // loop
        for(contDriversIndex=1;contDriversIndex<=drivers;contDriversIndex++){
            if( flagDrivers[contDriversIndex]== 1){
                strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I1);
            }else 
                if( flagDrivers[contDriversIndex]== 2){
                    strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I2);
                }else if( flagDrivers[contDriversIndex]== 3){
                            strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I3);
                        }else if( flagDrivers[contDriversIndex]== 4){
                                    strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I4);
                                }else if( flagDrivers[contDriversIndex]== 5){
                                            strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I5);
                                        }else if( flagDrivers[contDriversIndex]== 6){
                                                    strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I6);
                                                }else if( flagDrivers[contDriversIndex]== 7){
                                                            strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I7);
                                                        }else if( flagDrivers[contDriversIndex]== 8){
                                                                    strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I8);
                                                                }else if( flagDrivers[contDriversIndex]== 9){
                                                                            strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I9);
                                                                        }else if( flagDrivers[contDriversIndex]== 10){
                                                                                    strcat(sshCommandTransferFilesArray,T4_TO_TRANSFER_I10);
                                                                        }
        } // end for
    }
        // Ejecución del comando
    system(sshCommandTransferFilesArray);
    // END 5. Transferencia de la informacion
     
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0){
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF){
            printf("\n Error : Fputs error\n");
        }
    }
    if(n < 0){
        printf("\n Read error \n");
    }
    return 0;
}
