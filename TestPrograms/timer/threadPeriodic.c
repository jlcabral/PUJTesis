#define _REENTRANT
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<sched.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>
#include<stdio.h>

#define SIGNUM SIGRTMAX
#define TIMER_RELTIME   0

// Declaración señales
sigset_t sigset; // Conjunto de señales
struct sigaction act;   //manejador de señal

timer_t temporizador;   //id temporizador
struct sigevent evp;    //evento asociado al temporizador
struct itimerspec param_temp;   //parámetros del temporizador

int cont = 0;

//Definiciones de tiempo
struct timespec PERIODO = {1, 0L}; //retardo temporizador de y valor de reinicio
//Signal handler
void ManejadorSig(int signo, siginfo_t *info, void *context);
//Declaración del thread
pthread_t thread;

void *f_thread(void *arg){
    //bloquea la señal: pthread_sigmask()
    //Los threads creados posteriormente heredan la máscara
    
    sigemptyset(&sigset);   //  crea una máscara vacía
    sigaddset(&sigset, SIGNUM); // Añade la señal (used to initialize a signal set)
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);
    printf("Señal # %d bloqueada por el proceso: %d\n",
                                        SIGNUM, getpid());
    
    //Asigna un manejador (Atrapa la señal)
    sigemptyset(&(act.sa_mask)); // Máscara vacía
    act.sa_sigaction = ManejadorSig;
    act.sa_flags = SA_SIGINFO;
    if( sigaction(SIGNUM, &act, NULL) < 0 ){
        perror("Sigaction ha fallado");
        exit(-1);
    }
    printf("Señal %d atrapada por el proceso: %d\n", SIGNUM, getpid());
    
    // Desbloquea la señal: pthread_sigmask()
    sigemptyset(&sigset); // Crea una máscara vacía
    sigaddset(&sigset, SIGNUM); //Añade la señal
    pthread_sigmask( SIG_UNBLOCK, &sigset, NULL);

    printf("Señal # %d Desbloqueada por el proceso: %d. Espero una señal\n",
                                                SIGNUM, getpid());
    //Crea el temporizador.
    evp.sigev_notify = SIGEV_SIGNAL;    //Tipo de notificación
    evp.sigev_signo = SIGNUM;           //Número de la señal
    evp.sigev_value.sival_int = 1;      //Valor pasado al manejador
    if(timer_create(CLOCK_REALTIME, &evp, &temporizador) != 0){
        perror("Error creando el temporizador");
        exit(-1) ;
    }
    //printf("Temporizador (señal # %d) Creado en el thread # %d\n",sig, pthread_self());

    //Configure el temporizador (periodo)
    param_temp.it_value = PERIODO;
    //Nuevo valor cuando expira el temporizador  (periodo)
    param_temp.it_interval = PERIODO;
    
    if( timer_settime(temporizador, TIMER_RELTIME, &param_temp, NULL) != 0){
        perror("Error configurando el temporizador");
        exit(-1);
    }
    printf("Temporizador activado en el proceso: %d\n",getpid());


    pthread_exit(NULL);
}

int main(void){

    pthread_create(&thread, NULL, &f_thread, NULL);
    for(;;){
        printf("Despertó SLEEP\n");
        sleep(3600);
    }
    //Borrar timer
    if(timer_delete(temporizador) != 0){
        perror("Error eliminando el temporizador");    
    }else{
        printf("Temporizador Eliminado en el proceso: %d\n",getpid());
    }

    pthread_join(thread,NULL);
    printf("Termina el programa. \n");
    exit(0);
}

void ManejadorSig(int signo, siginfo_t *info, void *context){
    printf("signo:[%d] ",signo);
    printf( "Soy el manejador de la señal #%d, Valor: %d",
            (info->si_signo), (info->si_value.sival_int));
    printf(" Code:(%d)--cont:[%d]\n", info->si_code, cont);
    cont++;
}
