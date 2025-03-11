#include "tcp.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "./tcpclient IPADDRESS PORT\n");
        return 0;
    }
    printf("creating the address for the server ......\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *server_address;
    if (getaddrinfo(argv[1], argv[2], &hints, &server_address))
    {
        fprintf(stderr, "Error in getting the address %d \n", GETSOCKETERRNO());
        return 1;
    }
    printf("Creating the socket .......\n");
    SOCKET client_socket;
    client_socket = socket(server_address->ai_addr->sa_family, server_address->ai_socktype, server_address->ai_protocol);
    if (!ISSOCKETVALID(client_socket))
    {
        fprintf(stderr, "Error in creating the socket (%d)\n", GETSOCKETERRNO());
    }
    printf("Connecting the socket to the server ...\n");
    if (connect(client_socket, server_address->ai_addr, server_address->ai_addrlen) < 0)
    {
        fprintf(stderr, "Error in Connecting the socket to the server \n");
    }
    freeaddrinfo(server_address);
    while (1)
    {
        fd_set dataset;
        struct timeval timediff;
        timediff.tv_sec = 0;
        timediff.tv_usec = 100000;
        FD_ZERO(&dataset);
        FD_SET(client_socket, &dataset);
        FD_SET(0, &dataset);
        select(client_socket + 1, &dataset, 0, 0, &timediff);
        if (FD_ISSET(client_socket, &dataset))
        {

            char received_buffer[4096];
            int bytes_received = recv(client_socket, received_buffer, 4096, 0);
            if (bytes_received < 1)
            {
                printf("Connection closed by peer.\n");
                break;
            }
            printf("DATA RECEIVED ----: %.*s", bytes_received, received_buffer);
        }
        if (FD_ISSET(0, &dataset))
        {
            char terminal_buffer[1096];
            if (!fgets(terminal_buffer, 4096, stdin))
                break;
            int bytes_sent = send(client_socket, terminal_buffer, strlen(terminal_buffer), 0);
            printf("%d Bytest sent \n", bytes_sent);
        }
    }
    CLOSESOCKET(client_socket);
}
