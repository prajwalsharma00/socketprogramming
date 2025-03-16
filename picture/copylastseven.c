#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stdout, "Error: Must be this format 'copylastseven sourcedir destinationdir'\n ");
    }
    DIR *source_folder = opendir(argv[1]);
    DIR *destination_folder = opendir(argv[2]);
    time_t current_time;
    if (!source_folder || !destination_folder)
    {
        fprintf(stderr, "Error No such folder exist . Please check the path .. \n");
        return 0;
    }
    struct dirent *directory;
    struct stat filename;
    while ((directory = readdir(source_folder)))
    {
        if (!strcmp(directory->d_name, "..") || !strcmp(directory->d_name, "."))
        {
        }
        else
        {
            current_time = time(NULL);
            struct timespec ts;
            char source[1024];
            char destination[1024];
            snprintf(source, sizeof(source), "%s/%s", argv[1], directory->d_name);
            snprintf(destination, sizeof(destination), "%s/%s", argv[2], directory->d_name);

            if (stat(source, &filename) == 0)
            {
                if (filename.st_mtime > 0)
                {
                    double diff_time = difftime(current_time, filename.st_mtime);
                    int diff_day = diff_time / (60 * 60 * 24);
                    if (diff_day > 7)
                    {
                        fprintf(stdout, "%s File have been moved from %s to %s \n", source, argv[1], argv[2]);
                        if (rename(source, destination) == 0)
                        {
                            printf("File have been sucessfully transmitted.\n ");
                        }
                    }
                }
            };
        }
    }
    return 0;
}