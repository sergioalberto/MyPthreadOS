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
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include  <sys/time.h> 

// Declaracion de variables
char *archivo;
char *ip;
int num_threads;
char * port;
int *pthread_tim_wait;
int contador = 0;
pthread_mutex_t bloqueo;

typedef enum {FALSE, TRUE} bool;

/**
	Encargada de mandar las solicitudes a los servidores
*/
void *llamada(void * param)
{
	int sockfd, bindfd; // socket and bind file descriptors
	char *ptr;
	char getrequest[1024];
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	sprintf(getrequest, "GET %s HTTP/1.1\nHOST: %s\nConnection:close\n\n", archivo, ip);

	// gets linked list of results of a specified hostname
	if ( getaddrinfo(ip, port, &hints, &res) != 0 ) {
	    fprintf(stderr, "Host or IP not valid\n"); //quits program if the hostname was not found
	    exit(0);
	}

	// creates a socket from hostname results and passes everything to a file descriptor
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// bind the socket to the port passed to getaddrinfo()
	bindfd = bind(sockfd, res->ai_addr, res->ai_addrlen);

	// establish a connection and quits if there is a connection error
	if ( connect(sockfd, res->ai_addr, res->ai_addrlen) != 0 ) {
	    fprintf(stderr, "Connection error\n");
	    exit(0);
	}

	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	// writes the HTTP request to the socked file descriptor
	write(sockfd, getrequest, strlen(getrequest));
	bool flag = 1;

	while(flag) {
		char *s;
		int i = 0;
		s = (char *) malloc (1000*sizeof(char));
		while(read(sockfd, s+i, 1) != 0){
			if (i > 998){
				break;
			}
			i++;
		}
		if(i<999) {
			flag = 0;
		}
	}
	close(sockfd);
	printf("Termino un cliente\n");
	pthread_exit(NULL);
	return NULL;
}

/**
	Main del cliente
*/
int main(int argc, char *argv[])
{
	if(argc != 7){
		printf("Usage: -n <N-threads> -u <url> -p <port>\n");
		return 1;
	}

	char *ptr, *host, path[100], *dns;
	dns = argv[4];
	ptr = strstr(dns, "/");
	strcpy(path, ptr);
	host = strtok(dns, "/"); //truncate the argument to a PATH and DNS name
	archivo = path;
	ip = dns;
	port= argv[6];
	num_threads = atoi(argv[2]);

	int creardor,cerrar;
	pthread_t thread_id[num_threads];
	
	//creando hilos
	printf(" Accediendo ...\n");
	for(creardor=0; creardor < num_threads; creardor++){
		printf("Conexión: %d\n",creardor);
		pthread_create( &thread_id[creardor], NULL, llamada, NULL );
		sleep(1);
	}
	
	//waits all threads to finish
	for(cerrar=0; cerrar < num_threads; cerrar++) {
		pthread_join( thread_id[cerrar], NULL);
	}

	printf("Listo !!\n");

	return 0;
}
