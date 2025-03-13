#include "udp.h"
#include <time.h>
#define PORT "8080"
static int i;
int main()
{
#if defined(_WIN32)
    WSADATA d;
    if (WSAStart(MAKE(2, 2), &d))
    {
        fprintf(stderr, "U
            nable to start the socket ....,\n");
        return 1;
    }
#endif
    i = 1;
    time_t time;
    struct tm *timeinfo;
    // get address for our server for this we woudl use the local address using loopback address
    struct addrinfo *server_address, hints;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    printf("Setting up ip address and port for the server .... \n");
    getaddrinfo(0, PORT, &hints, &server_address);
    printf("Sucessfully got the ip adress and port for server ... \n");
    printf("Creating socket .... \n");

    SOCKET server_socket;
    server_socket = socket(server_address->ai_family,
                           server_address->ai_socktype, server_address->ai_protocol);
    if (!ISSOCKETVLAID(server_socket))
    {
        fprintf(stderr, "Error on Crearing the socket ().(%d)", GETSOCKETERROR());
        return 1;
    }
    else
    {
        fprintf(stdout, "Socket have been created sucessfully ... \n");
    }
    printf("Binding the socket ..... \n");
    if (bind(server_socket, server_address->ai_addr,
             server_address->ai_addrlen) < 0)
    {
        fprintf(stderr, "Error on Binding ().(%d)", GETSOCKETERROR());
        return 1;
    }
    freeaddrinfo(server_address);
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server_socket, &master);
    int max_socket = server_socket;
    while (1)
    {
        fd_set reads;
        reads = master;
        if (select(max_socket + 1, &reads, 0, 0, 0) < 0)
        {
            fprintf(stderr, "Error on select\n");
            return 1;
        }

        if (FD_ISSET(server_socket, &reads))
        {

            struct sockaddr_storage client_address;
            socklen_t client_len = sizeof(client_address);
            char read[1024];
            int bytes_received = recvfrom(server_socket, read, sizeof(read),
                                          0, (struct sockaddr *)&client_address,
                                          &client_len);
            printf("%d Bytes received from the client .. \n", bytes_received);
            printf(" %d Bytes Received [CLIENT]: %.*s\n", bytes_received, bytes_received, read);
            char time_buffer[1024];
            char buffer[1024];
            timeinfo = localtime(&time);
            ß
                strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S",
                         timeinfo);

            sprintf(buffer, "%d.  %s :THE MESSAGE HAVE BEEN RECIEVD BY THE SERVER", i++, time_buffer);
            sendto(server_socket, buffer, sizeof(buffer), 0,
                   (struct sockaddr *)&client_address, client_len);

        } // if

    } // while
    CLOSESOCKET(server_socket);
#if defined(_WIN32)
    WSACleanup();

#endif
    printf("Finished ... \n");
}
