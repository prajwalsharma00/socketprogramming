#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main()
{
    int server_fd, new_socket;
    size_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addresslen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from the server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Faile\n");
        exit(EXIT_FAILURE);

    } // af unix is used to communicate between same device , sock_stream means we use tcp , 0 is for ip protocol
    // forecfully attached the socket to port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("socket error\n");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("binding error \n");
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("Error in listenign the socket \n");
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addresslen)) < 0)
    {
        perror("Error in accepting new connection \n");
    }
    valread = read(new_socket, buffer, sizeof(buffer) - 1);
    printf("%s \n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    close(new_socket);
    close(server_fd);
    return 0;
}