#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

#define MAX 10//Maximo de procesos en cola
//Estructura de una cola de procesos listos para ejecutarse
typedef struct {
    pcb_t *procs[MAX];//Array con los procesos
    int head;//Inicio de la cola
    int tail;//Fin de la cola
    int count;//Cantidad de procesos en cola
} ready_queue_t;

void  queue_init(ready_queue_t *q); //Metodo para crear la cola
int   queue_enqueue(ready_queue_t *q, pcb_t *p);//Metodo para insertar en cola
pcb_t *queue_dequeue(ready_queue_t *q);//Metodo para retirar de cola
int   queue_empty(ready_queue_t *q); //Ver si la cola esta vacia

#endif
