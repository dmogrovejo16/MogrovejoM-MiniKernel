#ifndef PCB_H
#define PCB_H

//Estados de los procesos que creamos
#define STATE_NEW     0
#define STATE_READY   1
#define STATE_RUNNING 2
#define STATE_DONE    3

#ifndef TOTAL_PROCS
#define TOTAL_PROCS   10
#endif

typedef struct {
    int pid;
    int burst_time;//Tiempo requerido para completar su ejecucion
    int remaining_time;//Tiempo que falta para completar ejecucion
    int arrival_time;   //Tiempo en el que llego a la cola
    int state;

    /* metricas por proceso */
    int start_time;      //Tiempo en el que inicia su ejecucion
    int finish_time;     //Tiempo en completarse
    int waiting_time;    //Tiempo de espera
    int response_time;    //Tiempo de respuesta
} pcb_t;

pcb_t *pcb_create(int pid, int burst_time, int arrival_time);

#endif 
