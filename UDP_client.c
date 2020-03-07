#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define SA struct sockaddr
#define MAXLINE 1024
#define PORT 8000
#define SERVADDR "127.0.0.1"

int main(int atgc, char *argv[]) {
    SOCKET serv_sock;
    SOCKADDR_IN dest_addr, servaddr;
    WORD wsaVersion = MAKEWORD(2,2);
    WSADATA  wsaData;
    int servsize = sizeof(servaddr);
    char buff[MAXLINE];

    if(WSAStartup(wsaVersion, &wsaData)) {
        printf("WSAStartup error %d\n",WSAGetLastError());
        return -1;
    }

    serv_sock = socket(AF_INET,SOCK_DGRAM,0);
    if(serv_sock == INVALID_SOCKET) {
        printf("Socket () error %d\n",WSAGetLastError());
        WSACleanup();
        return -1;
    }

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(SERVADDR);
    dest_addr.sin_port = htons(PORT);

    if(connect(serv_sock,(SA*)&dest_addr,sizeof(dest_addr))<0) {
        printf("Connect error %d\n",WSAGetLastError());
        closesocket(serv_sock);
        WSACleanup();
        return -1;
    }

    for(;;) {

        printf("Enter massage:\nC<=S: ");
        fgets(&buff[0],sizeof(buff)-1,stdin);

        if(!strcmp(&buff[0],"quit\n"))
            break;

        sendto(serv_sock, &buff[0], strlen(&buff[0]), 0, (SA*)&dest_addr, sizeof(dest_addr));

        printf("Massage send\n");

        int n=recvfrom(serv_sock, &buff[0], sizeof(buff)-1, 0, (SA*)&servaddr, &servsize);
        if(n==SOCKET_ERROR) {
            printf("Recvfrom Error %d\n",WSAGetLastError());
            closesocket(serv_sock);
            WSACleanup();
            return -1;
        }
        buff[n] =0;

        printf("S=>C: %s\n",&buff[0]);
    }
    system("pause");
    closesocket(serv_sock);
    WSACleanup();
    return 0;
}