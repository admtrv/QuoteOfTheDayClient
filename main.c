#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#define BUF_LEN 2048

#pragma comment(lib, "Ws2_32.lib")

void logMessage(FILE* file, char* message)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char date[80];
    strftime(date, sizeof(date), "%Y-%m-%d", t);

    fprintf(file, "%s: %s\n", date, message);
}

int main()
{

    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed!\n");
        return 1;
    }

    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo("djxmmx.net", "17", &hints, &result);
    if (iResult != 0)
    {
        printf("Getaddrinfo failed!\n");
        WSACleanup();
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;
    ptr = result;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Socket failed!\n");
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("Server connection failed!\n");
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        WSACleanup();
        return 1;
    }
    else
        printf("Server connection successful!\n");

    FILE* file = fopen("logger.txt", "a");

    char recvbuf[BUF_LEN];

    iResult = recv(ConnectSocket, recvbuf, BUF_LEN, 0);
    if (iResult > 0)
    {
        recvbuf[iResult] = '\0';
        printf("Quote of the Day: %s\n", recvbuf);
        logMessage(file, recvbuf);
    }
    else if (iResult == 0)
        printf("Connection closed!\n");
    else
        printf("Recv failed!\n");

    fclose(file);

    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
