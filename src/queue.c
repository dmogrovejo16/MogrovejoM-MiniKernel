#include <stdio.h>
#include <string.h>
#include "queue.h"


void queue_init(ready_queue_t *q) { //Metodo para crear la cola
    memset(q, 0, sizeof(*q)); 
    q->head = 0;//Inicializamos valores en cero
    q->tail = 0;
    q->count = 0;
}

int queue_enqueue(ready_queue_t *q, pcb_t *p) {//Metodo para insertar en cola
    if (q->count >= MAX) return -1;  //Si la cola ya esta llena error 
    q->procs[q->tail] = p;
    q->tail = (q->tail + 1) % MAX;
    q->count++;
    return 0;
}

pcb_t *queue_dequeue(ready_queue_t *q) {//Metodo para retirar de cola
    if (q->count == 0) return NULL;//Si la cola esta vacia error
    pcb_t *p = q->procs[q->head];
    q->head = (q->head + 1) % MAX;
    q->count--;
    return p;
}

int queue_empty(ready_queue_t *q) {//Ver si la cola esta vacia
    return q->count == 0;
}
