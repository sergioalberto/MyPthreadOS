CC=gcc
FORK-WEBSERVER= fork-webserver.c
THREAD-WEBSERVER= thread-webserver.c


OUTPUT=stress-client fork-webserver thread-webserver

all: $(OUTPUT)

fork-webserver: $(FORK-WEBSERVER)	
	$(CC) -o fork-webserver $(FORK-WEBSERVER)

thread-webserver: $(THREAD-WEBSERVER)
	$(CC) -o thread-webserver $(THREAD-WEBSERVER) mypthread.c
	
stress-client: stress-client.c
	$(CC) -o stress-client stress-client.c mypthread.c
	
clean:
	rm -f $(OUTPUT)
