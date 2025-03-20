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
        *p = '\0';
        strcpy(*protocol, url_copy);
        p += 3;
    }
    else
    {

        p = url_copy;
    }
    char *temp = p;

    while (*temp && *temp != ':' && *temp != '/')
        temp++;

    if (*temp == ':')
    {
        *temp = '\0';

        strcpy(*hostname, p);
        p = temp + 1;
        temp = strchr(p, '/');
        if (temp)
        {
            *temp = '\0';
            strcpy(*port, p);
            p = temp + 1;
            if (p)
            {
                strcpy(*path, p);
            }
        }
        else
        {
            strcpy(*port, temp);
        }
    }
    else

    {
        *temp = '\0';

        strcpy(*hostname, p);
        p = temp + 1;
        if (*p)
        {
            strcpy(*path, p);
        }
    }

    // while (*temp && *temp != '/')
    //     temp++;
    // if (*temp == '/')
    // {

    // }
    free(url_copy);

    printf("The protocol found is %s \n", *protocol);
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
    urlparser("https://www.youtube.com:90/watch?v=-TkoO8Z07hI", &hostname, &port, &protocol, &path);
}
