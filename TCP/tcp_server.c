#include "tcp.h"
#include <ctype.h>
#define PORT "8080"
int main()
{
#if defined(_WIN32)
    WSADATA d,
        if (WSAStar(MAKE(2, 2), &d) < 0)
    {
        fprintf(stderr, "Error on starting the server \n");
        return 1;
    }
#endif
    struct addrinfo hints, *server_address;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    printf("Getting the address ...............\n");
    if (getaddrinfo(0, PORT, &hints, &server_address) != 0)
    {
        fprintf(stderr, "Error on getting the name of the address  %d \n", errno);
        return 1;
    }
    printf("Connecting the Socket ................ \n");
    SOCKET server_socket;
    server_socket = socket(server_address->ai_family, server_address->ai_socktype, server_address->ai_protocol);
    if (!ISSOCKETVALID(server_socket))
    {
        fprintf(stderr, "Error on getting the socket Error code : %d \n", GETSOCKETERRNO());
        return 1;
    }
    printf("Binding the socket with the ip address ..............\n");
    if (bind(server_socket, server_address->ai_addr, server_address->ai_addrlen) < 0)
    {
        fprintf(stderr, "Error on Binding the socke to the ip address  %d \n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(server_address);
    printf("Listening to the connecton ...............\n");
    if (listen(server_socket, 10) < 0)
    {
        fprintf(stderr, "listen Failed() %d \n", GETSOCKETERRNO());
        return 1;
    }
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server_socket, &master);
    SOCKET max_socket = server_socket;
    printf("Waiting for conncections ...... \n");
    while (1)
    {
        fd_set reads;
        reads = master;
        if (select(max_socket + 1, &reads, 0, 0, 0) < 0)
        {
            fprintf(stderr, "Select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
        SOCKET i;
        for (i = 1; i <= max_socket; ++i)
        {
            if (FD_ISSET(i, &reads))
            {
                if (i == server_socket)
                {
                    printf("New connection ..... \n");
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    SOCKET socket_client = accept(server_socket,
                                                  (struct sockaddr *)&client_address,
                                                  &client_len);

                    if (!ISSOCKETVALID(socket_client))
                    {
                        fprintf(stderr, "accept() Error. (%d)", GETSOCKETERRNO());

                        return 1;
                    }
                    FD_SET(socket_client, &master);
                    if (socket_client > max_socket)
                    {
                        max_socket = socket_client;
                    }
                    char address_buffer[1024];
                    getnameinfo((struct sockaddr *)&client_address, client_len,
                                address_buffer, sizeof(address_buffer),
                                0, 0, NI_NUMERICHOST);
                    printf("the connection was taken by %s \n", address_buffer);
                }
                else
                {
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);
                    if (bytes_received < 1)
                    {
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);
                        continue;
                    }
                    int j;
                    for (j = 0; j < bytes_received; ++j)
                    {
                        read[j] = toupper(read[j]);
                    }
                    // sending the data
                    printf("Sending back to the client ....... \n");
                    send(i, read, bytes_received, 0);
                } // checking if is new connection or old connection is sending some data
            } // fdiset checking
        } // socket_checking
    } // while
    printf("Closing Socket ..........\n");
    CLOSESOCKET(server_socket);
#if defined(_WIN32)
    WSACleanup();

#endif
    printf("Finisehd .....");
    return 0;
}
