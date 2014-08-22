CC=gcc
SRCPATH=source/
OBJPATH=obj/
all: client.o server.o rsa.o
	$(CC) $(OBJPATH)client.o -lcrypto -o client
	$(CC) $(OBJPATH)server.o -lcrypto -o server
client.o:
	$(CC) $(SRCPATH)client.c -c -Wall -g -lcrypto -o $(OBJPATH)client.o
server.o:
	$(CC) $(SRCPATH)server.c -c -Wall -g -lcrypto -o $(OBJPATH)server.o
rsa.o:
	$(CC) $(SRCPATH)rsa.c -c -Wall -g -lcrypto -o $(OBJPATH)rsa.o
