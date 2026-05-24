#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "sync.h"
#include "metrics.h"
#include "pcb.h"

//Para datos globales
kernel_state_t kernel;

//Inicializacion de mutex y condicionales
void sync_init(void) {
    queue_init(&kernel.queue);
    pthread_mutex_init(&kernel.mutex, NULL);
    pthread_cond_init(&kernel.cond_not_empty, NULL);
    pthread_cond_init(&kernel.cond_not_full, NULL);
    kernel.procs_generated = 0;
    kernel.procs_finished  = 0;
    kernel.done            = 0;
    kernel.current_tick    = 0;
}

void sync_destroy(void) {//Inicializacion de metodo para parar la sincronizacion
    pthread_mutex_destroy(&kernel.mutex);
    pthread_cond_destroy(&kernel.cond_not_empty);
    pthread_cond_destroy(&kernel.cond_not_full);
}

//Metodo para crear un proceso nuevo
pcb_t *pcb_create(int pid, int burst_time, int arrival_time) {
    pcb_t *p = malloc(sizeof(pcb_t));
    if (!p) return NULL;
    p->pid            = pid;
    p->burst_time     = burst_time;
    p->remaining_time = burst_time;
    p->arrival_time   = arrival_time;
    p->state          = STATE_READY;
    p->start_time     = -1;  //porque aun no ha iniciado
    p->finish_time    = 0;
    p->waiting_time   = 0;
    p->response_time  = 0;
    return p;
}

//Creamos un hilo para generar los procesos
void *process_generator(void *arg) {
    (void)arg;
    srand((unsigned)time(NULL));

    for (int i = 0; i < TOTAL_PROCS; i++) {
        //Simulamos que cada proceso llegan en orden aleatorio
        int delay = (rand() % 5) + 1;
        sleep(delay);

        pthread_mutex_lock(&kernel.mutex);

        //Si la cola esta llena hay que esperar
        while (kernel.queue.count >= MAX) {
            pthread_cond_wait(&kernel.cond_not_full, &kernel.mutex);
        }
		
		int burst    = (rand() % 10) + 1;
		//Creamos el prceso con tiempo global y burst aleatorio
        pcb_t *p = pcb_create(i + 1, burst, kernel.current_tick);
		
        if (!p) {//Si hubo un error creando, se libera el mutex para tratar de crear otro
            pthread_mutex_unlock(&kernel.mutex);
            continue;
        }

        queue_enqueue(&kernel.queue, p);//Ponemos el nuevo proceso en la cola
        kernel.procs_generated++;//Aumentamos la cantidad de procesos que hay

        printf("Proceso PID=%d creado , burst=%d , arrival=%d",
               p->pid, p->burst_time, p->arrival_time);

        pthread_cond_signal(&kernel.cond_not_empty);//Despertamos al cpu porque la cola ya no esta vacia
        pthread_mutex_unlock(&kernel.mutex);
    }

    //Cuando se creen todos se indica con done que se acabo la creacion
    pthread_mutex_lock(&kernel.mutex);//Mutex para que evitar condicion de carrera en done
    kernel.done = 1;
    pthread_cond_broadcast(&kernel.cond_not_empty);//Los procesos que fueron dormidos hasta acabar la creacion se despiertan
    pthread_mutex_unlock(&kernel.mutex);

    printf("Se han creado todos los procesos.\n");
    return NULL;
}

//Hilo de un CPU
void *cpu_thread(void *arg);


int main(void) {
    printf(" Programa MiniKernel , CPUs=%d , QUANTUM=%d , Procesos=%d \n\n",
           N_CPUS, QUANTUM, TOTAL_PROCS);

    sync_init(); //Valores globales en 0
    metrics_init(); //Metricas en 0

    //Creamos un hilo que genere los prcesos
    pthread_t gen_tid;
    pthread_create(&gen_tid, NULL, process_generator, NULL);

    //Creamos los 2 hilos del CPU
    pthread_t cpu_threads[N_CPUS];
    for (int i = 0; i < N_CPUS; i++) {
        int *id = malloc(sizeof(int));//Memoria dinamica para el id del CPU para que no todos los hilos tengan el mismo id
        *id = i;
        pthread_create(&cpu_threads[i], NULL, cpu_thread, id);
	}

    //Esperamos que todos sterminen antes de seguir
    pthread_join(gen_tid, NULL);
    for (int i = 0; i < N_CPUS; i++) {
        pthread_join(cpu_threads[i], NULL);
    }


    //Imprimimos las metricas
    metrics_report();

	//Finalizamos la sincronizacion
    sync_destroy();
    printf("\n Fin del Programa MiniKernel \n");
    return 0;
}
