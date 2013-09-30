#ifndef PHOTOCOPIER_H
#define PHOTOCOPIER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

class photocopier
{
    public:
        photocopier();
};

typedef int buffer_item;
#define BUFFER_SIZE 19

typedef struct {
    int Idthread;
    int IdClient;
} datasClient;

void createClients(int id, int number);
void createClientsTest(int id);
void init();
int randInt();
void *producer(void *param);
void *consumer(void *param);
int generarSolicitudImpresion(buffer_item item);
int sheduler(buffer_item *item);
int search(int position);
int getTeacher();
int getStudents();

#endif // PHOTOCOPIER_H
