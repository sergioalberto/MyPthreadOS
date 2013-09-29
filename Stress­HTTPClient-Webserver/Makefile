CC=gcc
FORK-WEBSERVER= fork-webserver.c
THREAD-WEBSERVER= thread-webserver.c

FLAGS= pthread

OUTPUT=stress-client fork-webserver thread-webserver

all: $(OUTPUT)

fork-webserver: $(FORK-WEBSERVER)	
	$(CC) -$(FLAGS) $(FORK-WEBSERVER) -o fork-webserver

thread-webserver: $(THREAD-WEBSERVER)
	$(CC) -$(FLAGS) $(THREAD-WEBSERVER) -o thread-webserver
	
stress-client: stress-client.c
	$(CC) -$(FLAGS) stress-client.c -o stress-client
	
clean:
	rm -f $(OUTPUT)
