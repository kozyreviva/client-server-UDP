#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define SA struct sockaddr
#define PORT 8000
#define MAXSIZE 1024

int main (int argc, char *argv[]) {
    SOCKET mysock;
    SOCKADDR_IN local_addr, cliaddr;
    WORD wsaVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    int clisize = sizeof(cliaddr);
    char buff[MAXSIZE];

    printf("Server UDP with connect\n\n");

    if(WSAStartup(wsaVersion,&wsaData)) {
        printf("WSASrartup error %d\n",WSAGetLastError());
        return -1;
    }

    mysock = socket(AF_INET,SOCK_DGRAM,0);
    if(mysock==INVALID_SOCKET) {
        printf("Socket error %d\n",WSAGetLastError());
        WSACleanup();
        return -1;
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(PORT);

    if(bind(mysock,(SA*)&local_addr, sizeof(local_addr))) {
        printf("Bind error %d\n",WSAGetLastError());
        closesocket(mysock);
        WSACleanup();
        return -1;
    }

    printf("Wait massages from client.\n\n");

    for(;;) {
        int n = recvfrom(mysock, &buff[0], sizeof(buff)-1,0,(SA*)&cliaddr,&clisize);
        if(n==SOCKET_ERROR) 
            printf("Recvfrom error %d\n",WSAGetLastError());
        buff[n] = 0;

        printf("C=>S %s\n",&buff[0]);

        sendto(mysock, &buff[0],n,0,(SA*)&cliaddr,sizeof(cliaddr));

        printf("Massage send!!!\n");

    }
    return 0;
}