#include <stdio.h>
#include <stdlib.h>
#include "../include/pcb.h"

/*
*Este programa verifica que los procesos se creen correctamente, ademas de que su caluclo de metricas sea el adecuado 
*/
pcb_t *pcb_create(int pid, int burst_time, int arrival_time, int priority);

int main(void) {
	pcb_t *p = pcb_create(42, 8, 3, 2);//Se usa la funcion para crear un proceso con valores arbitrarios
	
	//Se utilizan ifs para validar la correcta asignacion de datos en el PCB
	if (p == NULL) { //La creacion no debe retornar null
		printf("ERROR: pcb_create retorno NULL\n");
		return 1;
	}
	printf("El pcb_create no retorna NULL\n");
	
	if (p->pid != 42) {
		printf("ERROR: pid incorrecto: %d\n", p->pid);
		return 1;
	}
	printf("El pid asignado correctamente\n");
	
	if (p->burst_time != 8) {
		printf("ERROR:  burst_time incorrecto: %d\n", p->burst_time);
		return 1;
	}
	if (p->remaining_time != 8) {
		printf("ERROR:remaining_time debe ser igual a burst_time al crear\n");
		return 1;
	}
	printf("El burst_time y remaining_time iguales al crear\n");
	
	if (p->arrival_time != 3) {
		printf("ERROR:arrival_time incorrecto: %d\n", p->arrival_time);
		return 1;
	}
	printf("El arrival_time correcto\n");
	
	if (p->state != STATE_READY) {
		printf("ERROR:  estado inicial no es READY\n");
		return 1;
	}
	printf("El estado inicial si es READY\n");
	
	if (p->start_time != -1) {
		printf("ERROR: start_time debe iniciar en -1\n");
		return 1;
	}
	printf("El start_time si inicia en -1\n");
	
	free(p);//Libreamos la memoria utilizada por el procesos
	
	
	
	int turnaround = p.finish_time - p.arrival_time; //Se calculan metricas dependientes de los valores asignados
	int response   = p.start_time  - p.arrival_time;
	int waiting    = p.finish_time - p.arrival_time - p.burst_time;
	
	if (turnaround != 9) {//Se verifica que los valores coincidan con lo esperado
		printf("ERROR:  turnaround incorrecto: %d (esperado 9)\n", turnaround);
		return 1;
	}
	printf("El turnaround = %d\n", turnaround);
	
	if (response != 2) {
		printf("[ERROR: response time incorrecto: %d (esperado 2)\n", response);
		return 1;
	}
	printf("El  response time = %d\n", response);
	
	if (waiting != 4) {
		printf("ERROR: waiting time incorrecto: %d (esperado 4)\n", waiting);
		return 1;
	}
	printf("El waiting time es correcto = %d\n", waiting);
	
	if (waiting < 0) {
		printf("ERROR:waiting time no puede ser negativo\n");
		return 1;
	}
	printf("El waiting time no es negativo\n");
	
	printf("TODOS LOS TEST SE COMPLETARON CORRECTAMENTE\n");
	return 0;
}
