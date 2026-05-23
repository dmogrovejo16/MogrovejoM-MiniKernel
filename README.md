1. Proyecto Integrador MiniKernel

Simulador de scheduling Round Robin con multiples CPUs usando hilos POSIX.

2. Estructura

MiniKernel/
├── include/
│   ├── pcb.h        # Process Control Block
│   ├── queue.h      # Cola de procesos listos
│   ├── metrics.h    # Metricas de rendimiento
│   └── sync.h       # Estado global compartido (mutex, condvars)
├── src/
│   ├── main.c       # Generador de procesos + orquestacion
│   ├── queue.c      # Cola circular
│   ├── cpu.c        # Hilos CPU con Round Robin
│   └── metrics.c    # Reporte de metricas
├── tests/
├── Makefile
└── README.md
```

3. Compilar y ejecutar

1. Entramos a Bash
2. Ejecutamos make ./minikernel

4. Parametros a Utilizar

Numero de CPUS a simular(N_CPUS)=2
QUANTUM = 2  
Total de procesos=10 
Burst Maximo=random 1-10    
Tiempo de llegada entre procesos=random 1-5 

5. Metricas reportadas

5.1. Throughput: procesos terminados sobre tiempo total
5.2. Response time: primera ejecucion menos tiempo de llegada
