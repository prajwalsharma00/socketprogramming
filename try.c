#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    struct dirent *entry;
    char *path = "/Users/prajwalsharma/Desktop";
    DIR *dir = opendir(path);
    while ((entry = readdir(dir)) != NULL)
    {
        char newpath[2048];
        sprintf(newpath, "%s/%s", path, entry->d_name);
        if (entry->d_type == DT_DIR)
        {
        }
    }
    printf("%s\n", entry->d_name);
    open()
}