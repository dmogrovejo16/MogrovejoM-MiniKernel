#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "metrics.h"
#include "sync.h"

metrics_store_t mstore;

void metrics_init(void) {
    memset(&mstore, 0, sizeof(mstore));
    pthread_mutex_init(&mstore.lock, NULL);
}

//Metodo para cuando un proceso ya termino
void metrics_record(pcb_t *p) {
    pthread_mutex_lock(&mstore.lock);//Mutex para que si dos terminan al mismo tiempo no se pierdan datos
    if (mstore.count < TOTAL_PROCS) { //Si aun no se llega al maximo de procesos
        mstore.copy[mstore.count++] = *p;  //Guardamos el proceso terminado 
    }
    pthread_mutex_unlock(&mstore.lock);
}

//Metodo que muestra el reporte final
void metrics_report(void) {
    pthread_mutex_lock(&mstore.lock);

    int n = mstore.count;//Obtenemos la cuenta de procesos terminados
    if (n == 0) {
        printf("\nSin procesos completados.\n");
        pthread_mutex_unlock(&mstore.lock);
        return;
    }

  
    int total_time = 0;

    printf("\n Reporte De Schedulling Round Robin \n");
    printf("%-6s  %-10s\n",
           "PID", "respuesta");

    for (int i = 0; i < n; i++) {
        pcb_t *p = &mstore.copy[i];//Sacamos la informacion del pcb de arreglo de terminados
        printf("%-6d %-10d\n",
               p->pid, p->response_time);
        
        if (p->finish_time > total_time) total_time = p->finish_time;
    }

    
float throughput;
	if (total_time > 0) {//Si el tiempo total es mayor a cero calcula el throughput
		throughput = (double)n / total_time;
	} else {
		throughput = 0;//Si no hubo tiempo no hay throughput
	}

    printf("-----------------------------------------------------\n");
    printf("Procesos completados : %d\n", n);
    printf("Throughput           : %.4f proc/unidad\n", throughput);

    pthread_mutex_unlock(&mstore.lock);
}



