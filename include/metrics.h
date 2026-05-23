#ifndef METRICS_H
#define METRICS_H

#include "pcb.h"

//Estructura para llevar los procesos completados
typedef struct {
    pcb_t copy[TOTAL_PROCS]; //Array de procesos terminados
    int   count;//Contador de procesos terminados
    pthread_mutex_t lock;
} metrics_store_t;

extern metrics_store_t mstore;

void metrics_init(void);
void metrics_record(pcb_t *p);   //Metodo para guardar la informacion de un proceso
void metrics_report(void); //Metodo para mostrar las metricas finales


#endif 
