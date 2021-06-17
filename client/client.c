//Perintah Compile :
//gcc client.c -o client
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

#define SIZE_BUF 100

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char uname[1024], pswrd[1024], unpass[30000];
    char request[1024], response[1024];
    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (!getuid())
    {
        if (sock == -1)
        {
            printf("Could not create socket");
        }
        puts("Socket created");

        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_family = AF_INET;
        server.sin_port = htons(7000);

        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            perror("Connection failed");
            return 1;
        }

        puts("Connected");

        int root = 0;
        int commandTrue = 0;
        while (1)
        {
            root = 1;
            printf("root@DBSISOP >> ");
            fgets(request, sizeof(request), stdin);

            if (send(sock, request, strlen(request), 0) < 0)
            {
                puts("Request failed");
                return 1;
            }

            if (recv(sock, response, 1024, 0) < 0)
            {
                puts("Response failed");
                break;
            }

            puts("Server reply :");
            puts(response);
            memset(&response, '\0', sizeof(response));
        }
    }
    else
    {
        if (sock == -1)
        {
            printf("Could not create socket");
        }
        puts("Socket created");

        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_family = AF_INET;
        server.sin_port = htons(7000);

        //Connect to remote server
        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            perror("Connection failed");
            return 1;
        }

        puts("Connected");

        strcpy(uname, argv[2]);
        strcpy(pswrd, argv[4]);
        sprintf(unpass, "%s:::%s:::", uname, pswrd);
        send(sock, unpass, sizeof(unpass), 0);
        if (recv(sock, response, 1024, 0) < 0)
        {
            printf("Login failed!\n");
            exit(1);
        }
        int root = 0;
        int commandTrue = 0;
        while (1)
        {
            printf("%s@DBSISOP >> ", argv[2]);
            fgets(request, sizeof(request), stdin);

            if (send(sock, request, 1024, 0) < 0)
            {
                puts("Request failed");
                return 1;
            }

            if (recv(sock, response, 1024, 0) < 0)
            {
                puts("Response failed");
                break;
            }

            puts("Server response :");
            puts(response);
            memset(&response, '\0', sizeof(response));
        }
    }
    close(sock);
    return 0;
}