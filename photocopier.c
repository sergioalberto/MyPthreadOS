#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef int buffer_item;
#define BUFFER_SIZE 19

typedef struct {
	int Idthread;
	int IdClient;
} datasClient;

/* Create Buffer */
buffer_item buffer[BUFFER_SIZE];

/* Semaphore and Mutex lock */
sem_t cEmpty;
sem_t cFull;
sem_t mutex;

/* Threads */
pthread_t tid; /* Thread ID */
pthread_attr_t attr; /* Thread attributes */

void *producer(void *param);
void *consumer(void *param);
void init();
int search(int position);
int randInt();

/* Progress Counter */
int cg;
int thread;
int students;
int teacher;

/**
	Main de la aplicacion
*/
main(int argc, char *argv[]){
	/* Variables */
	int argarray, c1;

	/* Argument counter checks */
	if(argc != 3){
		fprintf(stderr, "usage: -n <#-Clients>\n");
		return -1;
	}

	/* Get args from command line and change them into integers */
	argarray = atoi(argv[2]); /* How long to sleep before ending */

	init();

	datasClient _datasClient[argarray];

	/* Create the consumer threads */
	pthread_create(&tid, &attr, consumer, NULL);   

	pthread_t tid1[argarray]; /* Thread ID *

	/* Do actual work from this point forward */
	/* Create the producer threads */
	for(c1 = 1; c1 <= argarray; c1++){

		_datasClient[c1].Idthread = thread;
		if (randInt() > 50){
			_datasClient[c1].IdClient = 0;
		}else{
			_datasClient[c1].IdClient = 1;
		}
		pthread_create(&tid1[c1], &attr, producer, (void*)&_datasClient[c1]);
		printf("Creating client #%d\n", c1);    
		thread ++;
	}


	/* Ending it */
	sleep(20);

	printf("Production complete.\n");

}

/**
	Inicializa el entorno de la aplicacion
*/
void init() {

	int c2;
	sem_init(&mutex, 0, 1); /* Initialize mutex lock */
	pthread_attr_init(&attr); /* Initialize pthread attributes to default */
	sem_init(&cFull, 0, 0); /* Initialize full semaphore */
	sem_init(&cEmpty, 0, BUFFER_SIZE); /* Initialize empty semaphore */
	cg = 0; /* Initialize global counter */ 
	thread = 1;
	students = 0;
	teacher = 0;

	for(c2=0; c2<BUFFER_SIZE; c2++){ /* Initialize buffer */
		buffer[c2] = 33;
	}
}

/**
	Genera un numero random entre 0 y 100
*/
int randInt() {
    // Random number between 0 and 100
    return rand() % 100;
}

/**
	Los diferentes clientes generan solicitudes de impresion
*/
void *producer(void *param){

	/* Variables */
	buffer_item item;
	datasClient *message;
	message = (datasClient*)param;

	sleep(1);      

	item = message->IdClient;
	sem_wait(&cEmpty); /* Lock empty semaphore if not zero */
	sem_wait(&mutex);

	if(generarSolicitudImpresion(item)){
		fprintf(stderr, "Producer error"); 
	}
	else{
		printf("Ingreso un: %d\n", item); 
	}

	sem_post(&mutex);
	sem_post(&cFull); /* Increment semaphore for # of full */
}

/**
	Espera a hacer las impresiones correspondientes
*/
void *consumer(void *param){

	buffer_item item;

	while(1) {
		sleep(1);

		sem_wait(&cFull); /* Lock empty semaphore if not zero */
		sem_wait(&mutex);
		if(sheduler(&item)){
			fprintf(stderr, "print error "); 
		}
		else{
			printf("Imprimiendo a: %d\n", item);
		}

		sem_post(&mutex);
		sem_post(&cEmpty); /* Increments semaphore for # of empty */
	}
}

/**
	Ingresa al buffer compartido la solicitud de impresion
*/
int generarSolicitudImpresion(buffer_item item){

	if(cg < BUFFER_SIZE) /* Buffer has space */
	{
		if (item == 0){ // Se va a ingresar students
			students ++;
		}else{          // Se va a ingresar teachers
			teacher ++;
		}

		buffer[cg] = item;
		cg ++;
		return 0;
	}else /* Buffer full */
	{
		return -1;
	}
}

/**
	- Planifica cual va a ser el siguiente cliente a imprimir de acuerdo a las 
	caracteristicas dadas en el enunciado de la progra
	- El profesor tiene prioridad, si hay solo un profe, se le imprime a este,
	si hay mas de dos se tira una moneda al aire.
*/
int sheduler(buffer_item *item)
{
	if(cg > 0) /* Buffer has something in it */
	{
		if ( (teacher == 0) && (students != 0) ){ // Sigue un estudiante
			*item = buffer[(cg-1)];
			students --;
		}else{
			if(teacher >= 2){
				int data = randInt();

				if(data > 50){ // =========> Students
					*item = buffer[(cg-1)];
					students --;
				}
				else{ // ==========> Teachers
					*item = search((cg-1));
					//*item = buffer[(cg-1)];
					teacher --;
				}
			}else{ // ==========> Teachers
				//*item = search((cg-1));
				*item = buffer[(cg-1)];
				teacher --;
			}
		}
		cg--;
		return 0;
	}
	else{ /* Buffer empty */
		return -1;
	}
}

/**
	Encargada de buscar el primer profesor, dada una posicion
*/
int search(int position){
	
	int result = 50;
	int i = 0; 
	int pos = 0;
	int flag = 0;

	for(i=0; i < BUFFER_SIZE; i++){
		if(buffer[i] == 1){
			pos = i;
			flag = 1;
			break;
		}
	}
	
	if(flag == 1){
		result = buffer[pos];
		buffer[pos] = buffer[position];
		buffer[position] = result;
	}else{
		result = buffer[position];
	}

	return result;	
}
