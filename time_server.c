#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock.h>
#include <ws2tcpip.h>
pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //used for constant like AF_INET LIKE THOSE
#include <arpa/inet.h>  //basically used to maniputlate ipv4 or ipv6 address with inet_pton or inet_htop
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif
#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO (errno)

#endif
#include <stdio.h>
#include <time.h>
#include <string.h>

    int main()
{
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKE(2, 2), &d))
    {
        fprintf(stderr, "Failed to initilziaed \n");
        return 0;
    }
#endif
    printf("Configuring the local address..... \n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating Socket.....\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen))
    {
        fprintf(stderr, "Error in creating teh socket \n");
        return 1;
    }
    printf("Binding our socket to local addresses ...\n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed %d \n", GETSOCKETERRNO);
        return 1;
    }
    freeaddrinfo(bind_address);
    printf("Listening -----");
    if (listen(socket_listen, 10) < 0)
    {
        fprintf(stderr, "Listen() failed %d \n", GETSOCKETERRNO);
    };

    printf("Waiting for the connection --\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)(&client_address), &client_len);
    if (!ISVALIDSOCKET(socket_client))
    {
        fprintf(stderr, "accept() failed %d\n", GETSOCKETERRNO);
        return 1;
    }

    printf("Client is Connected ........\n");
    char address_buffer[INET6_ADDRSTRLEN];
    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    printf("%s \n", address_buffer);

    printf("Reading Request\n");
    char request[1024];
    int bytes_recieved = recv(socket_client, request, sizeof(request), 0);
    printf("Received %d bytes \n", bytes_recieved);
    printf("%.*s", bytes_recieved, request);

    printf("Sending Response.......\n");
    const char *response =
        "HTTP/1.1 200 OK \r\n"
        "Connection:close \r\n"
        "Content-Type: text /plain\r\n\r\n"
        "Local time is :";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Send %d of %d bytes \n. ", bytes_sent, (int)strlen(response));

    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    printf("Send %d of %d bytes\n", bytes_sent, (int)strlen(time_msg));

    printf("closing connection .....\n");
    CLOSESOCKET(socket_client);
    printf("Closing Listening socket \n");
    CLOSESOCKET(socket_listen);

#if defined(_WIN32)
    WSACleanup();
#endif
    printf("Finished \n");
    return 0;
}