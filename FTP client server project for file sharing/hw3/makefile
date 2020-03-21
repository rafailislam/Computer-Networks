all: server/ftp_server client/ftp_client

server/ftp_server: server/ftp_server.c
	gcc server/ftp_server.c  -w -o server/ftp_server

client/ftp_client: client/ftp_client.c
	gcc client/ftp_client.c -w -o client/ftp_client
clean:
	/bin/rm -f *.o *~ *.dat core ftp_server ftp_client


