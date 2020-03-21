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
#define 	MAXLINE 	4096
const int Q_LEN = 5;  // number of waiting clients


//********************************************************************
//
// lsClient() function
//
// This function performs ls operation on client directory and  print them into a text file and then print it on screen.
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
void lsClient()
{
    int fd1, fd2, fd3;
    int stdout = dup(1);
    fd1 = open("test.txt",O_RDWR | O_CREAT|O_EXCL,S_IRWXU);
    dup2(fd1,1);
    system("ls");
    dup2(stdout,1);
    close(stdout);
    fd2 = open("test.txt",O_RDONLY,S_IRWXU);
    if(fd2<0)
    {
        printf("can not open");
        return 0;
    }
    char buffer[1000];
    fd3 = read (fd2, &buffer, 1000);

    close(fd2);
    close(fd3);
    const char s[4] = "\n";
    char* tok;


    tok = strtok(buffer, s);

    // Checks for delimeter
    int c=1;
    while (tok != 0)
    {
        printf("%d. %s\n",c++, tok);

        // Use of strtok
        // go through other tokens
        tok = strtok(0, s);
    }

}

//********************************************************************
//
// lsServer() function
//
// This function reads a file which contains the information performed by ls operation on server

// Return Value
// ------------
// void
// Local Variables
// ---------------
// fd1, fd2, fd3, stdout		int		            for file descriptor
// buffer	                    chat []		        for storing information from opened file
// tok, s                       char                used for printing information in separate line
//*******************************************************************

void lsServer()
{
    int fd1, fd2, fd3;
    fd2 = open("slist.txt",O_RDONLY,S_IRWXU);
    if(fd2<0)
    {
        printf("can not open");
        return 0;
    }
    char buffer[2000];
    fd3 = read (fd2, &buffer, 2000);
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

int main(int argc, char **argv)
{

    int server_port, sock, sock1, datafd, code;
    uint16_t port;
    struct sockaddr_in servaddr, data_addr;
    char command[20], ip[50], str[MAXLINE+1];


    if(argc != 3)
    {
        printf("Invalid Number of Arguments\n");
        exit(1);
    }

    //get server port
    sscanf(argv[2], "%d", &server_port);

    //set up control connection--------------------------------------------------
    if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Failed to create a socket" );
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(server_port);

    /// convert srting decimal doted IP into numeric binary form
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error");
        exit(1);
    }

    if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        printf("Cannot connect to server\n");
        exit(1);
    }
    printf("Connection successfully established with the ftp server\n");



    while(1)
    {

        bzero(command, strlen(command));
        //get command from user
        printf("ftp> ");
        //code = get_command(command);
        //scanf("%s",command);
        scanf("%[^\n]%*c", command);
        // fgets(command, strlen(command), stdin);

        //user has entered quit
        if((strcmp(command, "bye")==0) )
        {
            char quit[1024];
            sprintf(quit, "bye");
            write(sock, quit, strlen(quit));
            bzero(quit, (int)sizeof(quit));
            read(sock,quit, 1024);
            printf("Server Response: %s\n", quit);
            printf("Closing the connection\n");
            break;
        }
        if( (strcmp(command, "ls client")==0) )
        {
            lsClient();
        }
        if((strcmp(command, "ls server")==0))
        {


            //sock1 = socket(AF_INET, SOCK_STREAM, 0);
            //connect(sock1, (struct sockaddr *) &servaddr, sizeof(servaddr);

            char buff[2000];
            int bytesReceived=0;
            write(sock, "ls", 2);
            bzero(buff, (int)sizeof(buff));
            FILE *fp = fopen("slist.txt", "w");
            if(NULL == fp)
            {
                printf("Error opening file");
                return 1;
            }
            while((bytesReceived = read(sock, buff, 2000)) > 0)
            {
                printf("Bytes received %d\n",bytesReceived);
                fwrite(buff, 1,bytesReceived,fp);
            }

            fclose(fp);
            lsServer();

        }

    }
    close(sock);
    return 0;
}

