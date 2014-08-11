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

//Funcion existencia de archivos
int fileExist(char *filename){
    struct stat buffer;
    return ( stat(filename, &buffer) == 0);
};
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
    }else{
        printf("Error, campo tabla 1 no valido");    
    }
    // Hay tabla 2
    if( *(strControl+10)=='2' || *(strControl+10)=='_' ){
        printf("Campo Tabla 2 valido\n");
        flagTablas.hayTabla2 = 1;
    }else{
        printf("Error, campo tabla 2 no valido");    
    }
    // Hay tabla 3
    if( *(strControl+12)=='3' || *(strControl+12)=='_' ){
        printf("Campo Tabla 3 valido\n");
        flagTablas.hayTabla2 = 1;
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
    }else{
        printf("Error, campo tabla 5 no valido");    
    }
    //Validacion si hubo indicacion tabla 4 
    if( flagTablas.hayTabla4 == 1 ){
        printf("Ingresar el numero de conductores: "); fflush(stdout);
        scanf("%2s",strControl);
        int drivers = 0;
        drivers = atoi(strControl);
        // Ingreso del número de conductores
        if( drivers>0 && drivers<11 ){
            flagTablas.indexTabla4 = 1;
            printf("drivers a registrar:[%d]\n",drivers);
        }else{
            printf("mal numero drivers\n"); exit(1);
        }
        // Ingreso index de los conductores
        char flagDrivers[11];
        char contDriversIndex = 1;
        do{
            printf("Enter the index to modify [%d/%d]: ",contDriversIndex,drivers); fflush(stdout);
            scanf("%1s",strControl);
            flagDrivers[contDriversIndex] = atoi(strControl);
            contDriversIndex++;
        }
        while(contDriversIndex <= drivers);
        char i=0;
        for(i=1;i<=drivers;i++)
            printf("[%d]",flagDrivers[i]);
        printf("\n");
    }

    //END  4. Recolección de la información a cargar
    
    //5. Recolección de la información a cargar
        // PATH_BASH_CONFIG_SHOT
    if( !fileExist(PATH_BASH_CONFIG_SHOT) ){
        printf("BASH_FILE_RUNNING_SYSTEM No encontrado, revisar que se encuentre en [%s]\n",WORKING_PATH);
        printf("Operacion cancelada\n");
        exit(EXIT_FAILURE);
    }
    
    // END, 5. Recolección de la información a cargar

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
