//#########################################################################
//
// This program is used to test servers, and prints some statistical info
//
// Author: Sergio GQ
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

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>         
#include <sys/stat.h>      
#include <errno.h>

#include <sys/shm.h>
#include <sys/stat.h>
#include <netdb.h>
#include <dirent.h>
#include <signal.h>

#include "mypthread.h"

// Definiciones
#define BUF_SIZE 1024     // buffer
#define OK_IMAGE    "HTTP/1.0 200 OK\nContent-Type:image/gif\n\n"
#define OK_TEXT     "HTTP/1.0 200 OK\nContent-Type:text/html\n\n"
#define OK_PDF     "HTTP/1.1 200 OK\nContent-Type:application/pdf\n\n"
#define OK_VIDEO     "HTTP/1.1 200 OK\nContent-Type:video/x-msvideo\n\n"
#define OK_ISO     "HTTP/1.1 200 OK\nContent-Type:application/x-cd-image\n\n"
#define OK_EXE     "HTTP/1.1 200 OK\nContent-Type:application/x-ms-dos-executable\n\n"
#define OK_BIN     "HTTP/1.1 200 OK\nContent-Type:application/octet-stream\n\n"
#define OK_SWF     "HTTP/1.1 200 OK\nContent-Type:application/x-shockwave-flash\n\n"
#define NOTOK_404   "HTTP/1.0 404 Not Found\nContent-Type:text/html\n\n"
#define MESS_404    "<html><body><h1>FILE NOT FOUND</h1></body></html>"


// Declaracion de variables y estructuras
char delimitadores[2] = "/ " "";
struct sockaddr_in serv_addr;

/**
typedef struct {
    pthread_t thread_tid;      
    long    thread_count;     
} Thread;
Thread *tptr; //array of Thread structures; calloc'ed
*/

int listenfd, nthreads;
char* dir;
socklen_t addrlen;
//pthread_mutex_t mlock;
//pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;


/**
	Encargada de responder a las solicitudes de los clientes
*/
void *thread_main(void *arg) {

	int  connfd_aux;
	char *file_name, *req, *newpath;
	char get[3], *path, http[9];
	char sendBuff[1025];
	char out_buf[1025];
	unsigned int fh;                    
	unsigned int buf_len;
	unsigned int retcode;
	char hostpath[1024];

	req = (char*)malloc(1024*sizeof(char));
	path = (char*)malloc(1024*sizeof(char));
	newpath = (char*)malloc(1024*sizeof(char));

	printf("Thread %d starting\n", (int) arg);
	for ( ; ; ) {

		//pthread_mutex_lock(&mlock);
		printf("Escucho %d \n", (int) arg);
		connfd_aux = accept(listenfd, (struct sockaddr*)NULL, NULL);
		memset(sendBuff, '0', sizeof(sendBuff));
		retcode = recv(connfd_aux, req, 1025, 0);

		if (retcode < 0) {
			printf("Recv error detected ...\n");
		}
		else { 
			sscanf(req, "%s %s %s", get, path, http); //reads and parses the http get request
			printf("\n\tProcessing request...\n");
			newpath = path + 1; //ignores the first slash
			sprintf(hostpath,"%s/%s", dir, newpath);

			fh = open(&hostpath[0], O_RDONLY, S_IREAD | S_IWRITE);

			if (fh == -1) {
				printf("File %s not found - sending an HTTP 404 \n", &newpath[0]);
				strcpy(out_buf, NOTOK_404);
				send(connfd_aux, out_buf, strlen(out_buf), 0);
				strcpy(out_buf, MESS_404);
				send(connfd_aux, out_buf, strlen(out_buf), 0);
			}
			else
			{
				printf("File %s is being sent \n", &newpath[0]);
				if ((strstr(newpath, ".jpg") != NULL)||(strstr(newpath, ".gif") != NULL)||(strstr(newpath, ".png") != NULL))
				{
					strcpy(out_buf, OK_IMAGE);
				}
				else if ((strstr(newpath, ".pdf") != NULL)) {
					strcpy(out_buf, OK_PDF);
				}
				else if ((strstr(newpath, ".avi") != NULL)){
					strcpy(out_buf, OK_VIDEO);
				}
				else if ((strstr(newpath, ".iso") != NULL)){
					strcpy(out_buf, OK_ISO);
				}
				else if ((strstr(newpath, ".exe") != NULL)){
					strcpy(out_buf, OK_EXE);
				}
				else if ((strstr(newpath, ".swf") != NULL)){
					strcpy(out_buf, OK_SWF);
				}
				else if ((strstr(newpath, ".bin") != NULL)){
					strcpy(out_buf, OK_BIN);
				}
				else {
					strcpy(out_buf, OK_TEXT);
				}

				send(connfd_aux, out_buf, strlen(out_buf), 0);
				buf_len = 1;
				while (buf_len > 0)
				{
					buf_len = read(fh, out_buf, BUF_SIZE);
					if (buf_len > 0) {
						send(connfd_aux, out_buf, buf_len, 0);
					}
				}
				close(fh);       // close the file
			}
		}
		printf("\n\tEnd of tranmission\n");
		//pthread_mutex_unlock(&mlock);
		close(connfd_aux);
		//tptr[(int) arg].thread_count++;
	}
	free(req);
	free(path);
	free(newpath);
}

/**
	Crea los hilos de servidor
*/
void thread_make(int i)
{
	MY_THREAD_CREATE(&thread_main, &i);
	return;                    
}

/**
	Cierra el servidor
*/
void *CerrarServidor(void *arg)
{
	char salir;
	scanf("%c", &salir);
	if(salir == 'q'){
		exit(0);
	}
}

/**
	Main del servidor con threads
*/
int main(int argc, char **argv)
{
	int i, port;
	void sig_int(int), thread_make(int);

	if (argc != 7) {
		fprintf(stderr, "Usage: -n <N-forks> -w <path-root> -p <port>\n");
		exit(1);
	}

	dir = argv[4]; // Dirrecion
	port = atoi(argv[6]); // puerto

	if (opendir(dir) == NULL) {
		fprintf(stderr, "Directory not found\n");
		exit(1);
	}

	int nthreads = atoi(argv[2]); // Numero de threads

	//tptr = calloc(nthreads, sizeof(Thread));
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
        
	//pthread_t idHilo;
	//pthread_create(&idHilo, NULL, &CerrarServidor, NULL);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(listenfd, 1000); //listening to a maximum of 1000 connections
	
	//pthread_t thread_id[num_threads];
	//#######################################################################
	contador_t = 0;
	join = 0;
	Head = Current = NULL;        /* initialize pointers  */

	if (setjmp(MAIN) == 0)        /* initialize scheduler */   
        	Scheduler();
   	//#######################################################################


	for (i = 0;  i < nthreads; i++)
		thread_make(i);   
    

	//########################################################################
	if(!join)
		longjmp(SCHEDULER,1);         /* start scheduler      */
	//########################################################################

	MY_THREAD_JOIN();

	for ( ; ; )
		pause();                
}



