CC=gcc
THREAD-WEBSERVER= thread-webserver.c

OUTPUT=stress-client thread-webserver photocopier

all: $(OUTPUT)

thread-webserver: $(THREAD-WEBSERVER)
	$(CC) -o thread-webserver $(THREAD-WEBSERVER) mypthread.c
	
stress-client: stress-client.c
	$(CC) -o stress-client stress-client.c mypthread.c

photocopier: photocopier.c
	$(CC) -pthread -o photocopier photocopier.c 
	
clean:
	rm -f $(OUTPUT)
