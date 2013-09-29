#include "fotocopiadora.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>

using std::cout;
using std::endl;
sem_t semInitClient, semEndClient, lockTurn;
/* the semaphores */
sem_t full, empty;
pthread_mutex_t mutex;
pthread_mutex_t mutex1;
pthread_attr_t attr; //Set of thread attributes


int _Turno;   // Cual hilo se esta atendiendo
int _IdThread; // Id glogal de hilos

//int _SizeClients;     // Cantidad maxima de clientes permitidos para cada fila
QVector<int> _TeachersQueue(0);
int _IdTeacher;       // Id del profesor que esta atendiendo
QVector<int> _StudentsQueue(0);
int _IdStudent;       // Id del estudiante que esta atendiendo

Fotocopiadora::Fotocopiadora()
{
    _Turno = 1000;
    _IdThread = 0;
    pthread_attr_init(&attr); /* Get the default attributes */
}


/**
  Clientes  ----> Productor
  */
void *Client(void *arg){

    datasClient *message;
    message = (datasClient*)arg;

    sem_wait(&empty); /* Lock empty semaphore if not zero */
    pthread_mutex_lock(&mutex);

    if(message->IdClient == 0){ // Se va a agregar un estudiante
        _StudentsQueue.append(0);
        //printf("Id %d ...\n", message->Idthread);
        //printf("Id2 %d ...\n", _StudentsQueue.);
        _IdStudent ++;
    }
    else{ // Se va a agregar un profesor
        _TeachersQueue.append(1);
        _IdTeacher ++;
    }

    pthread_mutex_unlock(&mutex);
    sem_post(&full); /* Increment semaphore for # of full */
    /**
    while(1){
        //cout <<message<< endl;
        //if (message == 1)
        sem_wait(&full); // Lock empty semaphore if not zero
        pthread_mutex_lock(&mutex1);
        if(_Turno == message->Idthread){
            printerPaper(*message);
        }
        pthread_mutex_unlock(&mutex1);
        sem_post(&empty); //Increments semaphore for # of empty
    }
*/
    //return (void*)(1);
}


/**
  Imprime lo q el cliente quiera
  */
void *printerPaper(int data){

    printf("Imprimiendo %d ...\n", data);

    int datoEnviar;
    for(int i; i < 10000000; i++){
        datoEnviar ++;
    }
}


/**
  Crea hilos de acuerdo a su id y cantidad
  */
void createClient(int id, int number){

    //pthread_attr_t attr;
    //pthread_attr_init(&attr);

    pthread_t threadClient[number];
    datasClient _datasClient[number];

    for (int i=0; i < number; i++){

        _datasClient[i].Idthread = _IdThread;
        _datasClient[i].IdClient = id;
        pthread_create(&threadClient[i], &attr, &Client, (void*)&_datasClient[i]);
        _IdThread ++;

        //pthread_detach(threadClient[i]);
    }

    //for(int i=0; i < number; i++){
      //  pthread_join(threadClient[i], NULL);
    //}
}

/**
  Trabajador de la fotocopiadora q va a decidir cual cliente atender --> Consumidor
  */
void *trabajadorFotocopiadora(void *arg){

    while(1){
        //printf("Scheduler ...");
        //sem_wait(&semInitClient); //Down -> pide tickets, sino se durme
        sem_wait(&full); /* Lock empty semaphore if not zero */
        pthread_mutex_lock(&mutex);

        Scheduller();

        pthread_mutex_unlock(&mutex);
        sem_post(&empty); /* Increments semaphore for # of empty */
    }
}

/**
  Se crea el trabajador de la fotocopiadora
  */
void createWork(){
    //pthread_attr_t attrr;
    //pthread_attr_init(&attrr);
    pthread_t threadWork;
    pthread_create(&threadWork, &attr, &trabajadorFotocopiadora, NULL);

    //pthread_join(threadWork, NULL);
}

/**
  Planificador de cual cliente atenter
  */
void Scheduller(){

    int id;

    if ( (_IdTeacher == 0) && (_IdStudent != 0) ){ // Sigue un estudiante

        id = _StudentsQueue.first();
        _IdStudent --;
        _StudentsQueue.pop_front(); // Elimina el primero

    }else{
        if(_IdTeacher > 2){
            int data = randInt(1,100);

            if(data > 50){ // =========> Students
                id = _StudentsQueue.first();
                _IdStudent --;
                _StudentsQueue.pop_front(); // Elimina el primero
            }
            else{ // ==========> Teachers
                id = _TeachersQueue.first();
                _IdTeacher --;
                _TeachersQueue.pop_front(); // Elimina el primero
            }
        }else{
            id = _TeachersQueue.first();
            _IdTeacher --;
            _TeachersQueue.pop_front(); // Elimina el primero
        }
    }
    //printf("Id %d ...\n", id);
    printerPaper(id);
}

int randInt(int low, int high) {
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

/**
  Iniciar todos los párametros
  */
void Fotocopiadora::initAll(){

    _IdThread = 1;
    _Turno = 1;
    _IdTeacher = 0;
    _IdStudent = 0;

    //_TeachersQueue[_SizeClients];
    //_StudentsQueue[_SizeClients];



    sem_init(&lockTurn,0,1);
    sem_init(&semInitClient,0,1);
    sem_init(&semEndClient,0,1);
    sem_init(&full, 0, 0); /* Create the full semaphore and initialize to 0 */
    sem_init(&empty, 0, 1); /* Create the empty semaphore and initialize to BUFFER_SIZE */
    pthread_mutex_init(&mutex, NULL); /* Initialize mutex lock */
    pthread_mutex_init(&mutex1, NULL); /* Initialize mutex lock */

    createWork();

    //insertClient(0,4);
    //pthread_mutex_destroy(&lockTurn); // Desinicializa el mutex

}

int getCountTeacher(){
   return _IdTeacher;
}

int getCountStudents(){
   return _IdStudent;
}
