#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "sync.h"
#include "metrics.h"

//Hilo para el CPU
void *cpu_thread(void *arg) {
    int cpu_id = *(int *)arg;
    free(arg);

    while (1) {
        pthread_mutex_lock(&kernel.mutex);//Activa el mutex 

        /*
         * Esperar mientras la cola este vacia Y todavia puedan llegar procesos.
         * Si done==1 y la cola esta vacia, ya terminamos.
         */
        while (queue_empty(&kernel.queue) && !kernel.done) {//Condicion de esperar si la cola esta vacia y el subprograma aun no acaba
            pthread_cond_wait(&kernel.cond_not_empty, &kernel.mutex);
        }

        if (queue_empty(&kernel.queue) && kernel.done) { //Si la cola está vacía y el subprograma ya termino
            pthread_mutex_unlock(&kernel.mutex);//Se quita el mutex
            break;  //Se rompe el bucle de ejecuciones
        }

        pcb_t *p = queue_dequeue(&kernel.queue); //Sacamos un proceso de la cola
        p->state = STATE_RUNNING;//Asignamos su estado a ejecutandose

      
        if (p->start_time < 0) {//Vemos si el proceso no ha sido traido a la CPU antes
            p->start_time    = kernel.current_tick; //Su tiempo de inicio es el actual
            p->response_time = p->start_time - p->arrival_time;//Calculamos su tiempo de respuesta
        }

        pthread_mutex_unlock(&kernel.mutex);//Quitamos el mutex
 
		int exec;
		if(p->remaining_time < QUANTUM){//Si el tiempo que le falta es menor a un QUANTUM, se asigna ese tiempo para su ejecucion, sino el Quantum
			exec = p->remaining_time;
		}else{
			exec= QUANTUM;
		}
        printf("[CPU %d] Ejecutando PID %d | restante: %d | quantum: %d\n",
               cpu_id, p->pid, p->remaining_time, exec);

        sleep(exec); //Simulamos la ejecucion

        //Activamos mutex para guardar sus datos
        pthread_mutex_lock(&kernel.mutex);

        p->remaining_time -= exec;//Disminuimos el tiempo que ejecuto al tiempo que le falta
        kernel.current_tick += exec;//Aumentamos al tiempo global el tiempo que demoro en ejecutar este proceso

        if (p->remaining_time <= 0) {//Si el tiempo que le queda es menor a cero, ya se termino el proceso
            p->state       = STATE_DONE;//Cambiamos su estado
            p->finish_time = kernel.current_tick;//Calculamos su timepo global al que termino
            p->waiting_time = p->finish_time - p->arrival_time - p->burst_time;//Calculamos tiempo que espero en cola
            kernel.procs_finished++;//Aumentamos la cantidad de procesos terminados
            printf("[CPU %d] PID %d FINALIZADO (turnaround=%d, espera=%d, respuesta=%d)\n",
                   cpu_id, p->pid,
                   p->finish_time - p->arrival_time,
                   p->waiting_time,
                   p->response_time);
            metrics_record(p);  //Guardamos una copia del proceso en el array de terminados
            free(p);
        } else {//Si el proceso aun no acaba
            
            p->state = STATE_READY;//Se cambia su estado a listo
            queue_enqueue(&kernel.queue, p);//Se lo pone nuevamente en la pila
            printf("[CPU %d] Context switch PID %d (restante=%d)\n", //Imprimimos el cambio de proceso a ser ejecutado por el CPU
                   cpu_id, p->pid, p->remaining_time);
            pthread_cond_signal(&kernel.cond_not_empty);//Levantamos la espera del CPU si ya hay un proceso en la cola 
        }

        pthread_cond_signal(&kernel.cond_not_full);//Levantamos la espera del CPU si ya hay un proceso en la cola
        pthread_mutex_unlock(&kernel.mutex);
    }

    printf("[CPU %d] Terminando.\n", cpu_id);
    return NULL;
}
