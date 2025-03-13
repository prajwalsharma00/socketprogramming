#include "udp.h"

int main(int argc, char *argv[])
{
#if defined(_WIN32)
    WSADATA d;
    IF(WSAStartup(MAKE(2, 2), &d))
    {
        fprintf(stderr, "Error \n");
        return 1;
    }
#endif
    if (argc < 3)
    {
        fprintf(stdout, "./udp_client ipaddress portnumber");
        return 1;
    }
    struct addrinfo *server_address, hints;
    hints.ai_socktype = SOCK_DGRAM;
    getaddrinfo(argv[1], argv[2], &hints, &server_address);
    printf("Creating the client socket ... \n");
    SOCKET client_socket;
    client_socket = socket(server_address->ai_family, server_address->ai_socktype, server_address->ai_protocol);
    if (!ISSOCKETVLAID(client_socket))
    {
        fprintf(stderr, "Error on getting the socket (%d)", GETSOCKETERROR());
        return 1;
    }
    else
    {
        fprintf(stdout, "Socket have been sucessfully created ...\n");
    }
    while (1)
    {

        struct timeval ts;
        ts.tv_sec = 1;
        ts.tv_usec = 10000;
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(client_socket, &reads);
        FD_SET(0, &reads);
        if (select(client_socket + 1, &reads, 0, 0, &ts) < 0)
        {
            fprintf(stderr, "No more conneciton.. \n");
            break;
        } // if
        // If there's input from stdin (keyboard)
        if (FD_ISSET(0, &reads))
        {
            printf("Enter the message followed by enter key \n");
            char buffer[1024];
            fgets(buffer, sizeof(buffer), stdin); // Read user input

            // Remove newline character if present from fgets()
            buffer[strcspn(buffer, "\n")] = 0;

            int sent_bytes = sendto(client_socket, buffer, strlen(buffer), 0,
                                    server_address->ai_addr, server_address->ai_addrlen);
            printf("%d Data sent\n", sent_bytes);

            memset(buffer, 0, sizeof(buffer));
        }
        if (FD_ISSET(client_socket, &reads))
        {
            char buffer[1024];
            socklen_t server_len = sizeof(*server_address);
            int received_bytes = recvfrom(client_socket, buffer, sizeof(buffer), 0,
                                          server_address->ai_addr, &server_len);
            if (received_bytes > 0)
            {
                printf("%d Data received from server\n", received_bytes);
                printf("[SERVER]: %.*s \n", received_bytes, buffer); // Only print the actual data received
            }
        }

    } // while
    freeaddrinfo(server_address);
    CLOSESOCKET(client_socket);
#if defined(_WIN32)
    WSACleanup();
#endif
    printf("Finished ... \n");
}