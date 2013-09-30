//#########################################################################
//
// This is my photocopier
//
// Author: Sergio GQ and Edgar Rojas
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//#########################################################################

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
