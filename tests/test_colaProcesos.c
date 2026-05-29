#include <stdio.h>
#include "../include/queue.h"
#include "../include/pcb.h"
/*
*Este programa verifica que los procesos se ingreses y se retiren de la cola correctamente
*Se verifican PID al salir, retorno de funciones correctas y funciones de condiciones de cola vacia y llena
*/
int main(void) {
	ready_queue_t q;
	queue_init(&q);
	
	if (queue_empty(&q) != 1) {//Verificamos que NO existan datos antes de proceder
		printf("Error, la cola NO esta vacía\n");
		return 1;
	}
	
	//Creamos procesos con los datos necesarios
	pcb_t p1 = {.pid = 1, .burst_time = 5, .remaining_time = 5};
	pcb_t p2 = {.pid = 2, .burst_time = 3, .remaining_time = 3};
	pcb_t p3 = {.pid = 3, .burst_time = 7, .remaining_time = 7};
	
	//Los insertamos en la cola
	queue_enqueue(&q, &p1);
	queue_enqueue(&q, &p2);
	queue_enqueue(&q, &p3);
	
	if (q.count != 3) { //Vemos que existan exactamente 3 procesos, los que creamos e insertamos
		printf("ERROR:  No se agregaron 3 procesos\n");
		return 1;
	}
	printf("Existen los 3 procesos\n");
	
	pcb_t *out = queue_dequeue(&q);//Sacamos el primer ingresado, el primer ingresado debe ser el de pid=1
	if (out->pid != 1) {
		printf("ERROR: primer proceso no es PID=1\n");
		return 1;
	}
	out = queue_dequeue(&q);
	if (out->pid != 2) {
		printf("ERROR: segundo proceso no es PID=2\n");
		return 1;
	}
	out = queue_dequeue(&q);
	if (out->pid != 3) {
		printf("ERROR:  tercer proceso no es PID=3\n");
		return 1;
	}
	printf("El orden FIFO es correcto\n");
	
	if (queue_empty(&q) != 1) { //Verificamos que la cola quede vacia despues de retirar los 3 procesos
		printf("ERROR: cola no quedo vacia\n");
		return 1;
	}
	printf("La cola esta vacia tras dequeue\n");
	
	if (queue_dequeue(&q) != NULL) {//Si ya sacamos todos, al intentar sacar otro debe retornar NULL
		printf("ERROR: No se retiro null al final\n");
		return 1;
	}
	printf("Al intentar sacar otro retorna NULL\n");
	
	printf("TODOS LOS TEST SE COMPLETARON CORRECTAMENTE\n");
	return 0;
}
