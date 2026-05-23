#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>
#include "queue.h"

#ifndef N_CPUS
#define N_CPUS 2
#endif

#ifndef QUANTUM
#define QUANTUM 2
#endif

//Estructura de datos compartidos entre programas, concurrencia y conteo
typedef struct {
    ready_queue_t   queue;
    pthread_mutex_t mutex; //Evita condiciones de carrera en la cola
    pthread_cond_t  cond_not_empty;//Condicion para hacer esperar a los CPU si esta vacia la cola
    pthread_cond_t  cond_not_full;//Condicion para no crear mas procesos si esta llena la cola

    int procs_generated;//Procesos creados
    int procs_finished;//Procesos terminados
    int done; //Avisar que no hay mas procesos
    int current_tick; //Unidades de tiempo para calcular throughput
} kernel_state_t;

extern kernel_state_t kernel;

void sync_init(void);
void sync_destroy(void);

#endif 
