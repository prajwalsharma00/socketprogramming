#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void urlparser(char *url, char **hostname, char **port, char **protocol, char **path)
{
    snprintf(*protocol, sizeof(*protocol), "NaN");
    snprintf(*path, sizeof(*path), "NaN");
    snprintf(*port, sizeof(*port), "NaN");

    char *url_copy = strdup(url);
    char *p = strstr(url_copy, "://");
    if (p)
    {
        *p = 0;
        strcpy(*protocol, url_copy);
        p += 3;
    }
    else
    {

        p = malloc(sizeof(char) * 256);
        strcpy(p, url_copy);
    }
    char *temp = p;

    while (*temp && *temp != ':' && *temp != '/')
        temp++;

    if (*temp == ':')
    {
        int size = temp - p;
        printf("The length of address is %d \n", size);
        strncpy(*hostname, p, size);
        p += (size + 1);
        char *temp_one = strstr(temp, "/");
        if (temp_one)
        {
            strncpy(*port, p, temp - temp);
            p += (strlen(*port));
        }
        else
        {
            strcpy(*port, temp + 1);
        }
    }
    else
    {
        printf("the vlaueo f %ld\n", temp - p);
        strncpy(*hostname, p, temp - p);
        p += (strlen(*hostname) + 1);
    }
    while (*temp && *temp != '/')
        temp++;
    if (*temp == '/')
    {
        strcpy(*path, temp + 1);
    }

    // while (*temp && *temp != '/')
    //     temp++;
    // if (*temp == '/')
    // {

    // }

    printf("The protocol found is %s \n", *protocol);
    printf("The new p locaiton is %s \n", p);
    printf("The new port is %s \n", *port);
    printf("The new path is %s \n", *path);
    printf("The new hostname is %s \n", *hostname);
}
int main()

{
    char *hostname = malloc(sizeof(char) * 256);
    char *protocol = malloc(sizeof(char) * 256);
    char *port = malloc(sizeof(char) * 256);
    char *path = malloc(sizeof(char) * 256);
    urlparser("https://netfree.cc/home", &hostname, &port, &protocol, &path);
}
