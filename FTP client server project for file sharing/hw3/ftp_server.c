//********************************************************************
//
// Rafail Islam
// Computer Networks
// Assignment 3
// October 29, 2019
// Instructor: Dr. Ajay K. Katangur
//
//********************************************************************


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#define BUF_SIZE 2000
#define 	MAXLINE 	4096
const int Q_LEN = 5;  // number of waiting clients
//********************************************************************
//
// dolist() function
//
// This function performs ls operation on server directory and  print them into a text file and then print it on screen.
//
// Return Value
// ------------
// void
// Local Variables
// ---------------
// fd1, fd2, fd3, stdout		int		            for file descriptor
// buffer	                    chat []		        for storing information from opened file
// tok, s                       char                used for printing information in separate line
//*******************************************************************
void dolist()
{
    int fd1, fd2, fd3;
    int stdout = dup(1);
    fd1 = open("123.txt",O_RDWR | O_CREAT|O_EXCL,S_IRWXU);
    dup2(fd1,1);
    system("ls");
    dup2(stdout,1);
    close(stdout);
    fd2 = open("123.txt",O_RDONLY,S_IRWXU);
    if(fd2<0)
    {
        printf("can not open");
        exit(1);
    }
    char buffer[1000];
    fd3 = read (fd2, &buffer, 1000);
    //write(fd,buffer);
    close(fd2);
    close(fd3);
    const char s[4] = "\n";
    char* tok;
    tok = strtok(buffer, s);
    int c=1;
    while (tok != 0)
    {
        printf("%d. %s\n",c++, tok);
        tok = strtok(0, s);
    }

}
int clientInfo( char *ip, int *port)
{
    char *n1, *n2, *n3, *n4, *n5, *n6;
    int x5, x6;

    n1 = strtok(NULL, ",");
    n2 = strtok(NULL, ",");
    n3 = strtok(NULL, ",");
    n4 = strtok(NULL, ",");
    n5 = strtok(NULL, ",");
    n6 = strtok(NULL, ",");

    // convert ip into dotted decimal
    sprintf(ip, "%s.%s.%s.%s", n1, n2, n3, n4);

    x5 = atoi(n5);
    x6 = atoi(n6);
    *port = (256*x5)+x6;

    return 1;
}

int server_client(int *fd, char *cIP, int cPort, int sPort)
{

    struct sockaddr_in c_sin, s_sin;

    if ( (*fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket error");
        return -1;
    }

    //bind port for data connection to be server port - 1 by using a temporary struct sockaddr_in
    bzero(&s_sin, sizeof(s_sin));
    s_sin.sin_family = AF_INET;
    s_sin.sin_addr.s_addr = htonl(INADDR_ANY);
    s_sin.sin_port   = htons(sPort-1);

    while((bind(*fd, (struct sockaddr*) &s_sin, sizeof(s_sin))) < 0)
    {
        sPort--;
        s_sin.sin_port   = htons(sPort);
    }


    //initiate data connection fd with client ip and client port
    bzero(&c_sin, sizeof(c_sin));
    c_sin.sin_family = AF_INET;
    c_sin.sin_port   = htons(cPort);
    if (inet_pton(AF_INET, cIP, &c_sin.sin_addr) <= 0)
    {
        perror("inet_pton error");
        return -1;
    }

    if (connect(*fd, (struct sockaddr *) &c_sin, sizeof(c_sin)) < 0)
    {
        perror("connect error");
        return -1;
    }

    return 1;
}


int main(int argc, char **argv)
{
    struct sockaddr_in serv_sin;
    char buffer[500];
    int sockListen;
    int sockAccept, port;
    struct sockaddr_in cli_sin;
    unsigned int addrLen;   // or socklen_t addrLen
    int length;

    pid_t pid;

    if(argc != 2)
    {
        printf("Invalid Number of Arguments\n");
        printf("Usage: ./ftp_server <port_number>\n");
        exit(1);
    }
    // set port no. from command line argument
    sscanf(argv[1], "%d", &port);

    // Setup address structure
    bzero( &serv_sin, sizeof(serv_sin));
    serv_sin.sin_family = AF_INET;
    serv_sin.sin_addr.s_addr = INADDR_ANY;
    serv_sin.sin_port = htons(port);

    // Setup listening socket
    sockListen = socket(AF_INET, SOCK_STREAM, 0);
    if (sockListen < 0)
    {
        printf("Failed to create listening socket\n");
        exit(1);
    }

    if (bind(sockListen, (struct sockaddr *) &serv_sin, sizeof(serv_sin)) < 0)
    {
        printf("Failed to bind listening socket to address\n");
        exit(1);
    }

    if (listen(sockListen, Q_LEN) < 0)
    {
        printf("Failed to listen\n");
        exit(1);
    }


    addrLen = sizeof(cli_sin);
    // Wait for connection requests

    while (1)
    {
        sockAccept = accept(sockListen,(struct sockaddr*) NULL, NULL);
        if (sockAccept < 0)
        {
            printf("Failed to accept connection\n");
            exit(1);
        }
        printf("Connection successfully established with the ftp client\n");
        if((pid = fork()) == 0)
        {
            close(sockListen);

            int datafd, code, x = 0;
            int  cPort = 0;
            char msgfrom[MAXLINE+1];
            char cIP[60], command[1024];

            while(1)
            {
                bzero(msgfrom, (int)sizeof(msgfrom));
                bzero(command, (int)sizeof(command));

                //get client's data connection port
                if((x = read(sockAccept, msgfrom, MAXLINE)) < 0)
                {
                    printf("Cannot read from client");
                    break;
                }
                printf("*****************\n%s \n", msgfrom);
                if(strcmp(msgfrom, "bye") == 0)
                {
                    printf("Closing connection with client\n");
                    char goodbye[1024];
                    sprintf(goodbye,"Goodbye");
                    write(sockAccept, goodbye, strlen(goodbye));
                    close(sockAccept);
                    break;
                }
                // another connection
        /*
                int sockListen1 = socket(AF_INET, SOCK_STREAM, 0);

                bind(sockListen1, (struct sockaddr *) &serv_sin, sizeof(serv_sin));
                listen(sockListen1, Q_LEN);
                int sockAccept1 = accept(sockListen1,(struct sockaddr*) NULL, NULL);
                read(sockAccept1, command, MAXLINE);
        */
                if(strcmp(msgfrom, "ls") == 0)
                {
                 
                    dolist();

                    /*
                    clientInfo( cIP, &cPort);
                    if((server_client(&datafd, cIP, cPort, port)) < 0)
                    {
                        break;
                    }
                    */
                    FILE *fp = fopen("123.txt","rb");
                    if(fp==NULL)
                    {
                        printf("File open error");
                        return 1;
                    }
                    for (;;)
                    {
                        unsigned char buff[BUF_SIZE]= {0};
                        int nread = fread(buff,1,BUF_SIZE,fp);
                        printf("Bytes read %d \n", nread);


                        if(nread > 0)
                        {
                            write(sockAccept, buff, nread);
                        }

                        if (nread < BUF_SIZE)
                        {
                            if (feof(fp))
                                printf("End of file\n");
                            if (ferror(fp))
                                printf("Error reading\n");
                            break;
                        }
                    }
                    close(sockAccept);
                }


            }
            printf("Exiting Child Process...\n");
            close(sockAccept);
            _exit(1);

        }
        close(sockAccept);
    }
    return 0;
}



