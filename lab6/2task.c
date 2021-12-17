#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


char* add_slash(char* path) {
    if (path[strlen(path) - 1] != '/')
        strcat(path, "/");
    return path;
}

int filescan(char *full_patch, char *substr, char *name){

    FILE *fileptr;
    char *buffer;
    long filelen;

    fileptr = fopen(full_patch, "rb");  // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file

    bool found = strstr(buffer, substr) != NULL;

    //printf("%s", buffer);
    printf("pid %d has checked %ld bytes in '%s'. Given bytes were %s.\n", getpid(), filelen, name, (found ? "found" : "not found"));

    free(buffer);
}

int main (int argc, char *argv[])
{
    if(argc < 4){ exit(EINVAL); }
    
    char *source_dir = (char *)malloc(strlen(argv[1]) * sizeof(char));
    realpath(argv[1], source_dir);
    source_dir = add_slash(source_dir);

    char *str = (char *)malloc(strlen(argv[2]) * sizeof(char));
    strcpy(str, argv[2]);

    int N = atoi(argv[3]);
    
    DIR *directory;

    if (!(directory = opendir(source_dir))) {
        fprintf(stderr, "%s: %s: %s\n", "Error", source_dir, strerror(errno));
        return -1;
    }

    struct dirent *dir_item;
    long counter = 0;

    while((dir_item = readdir(directory)) != NULL) {
        if(strcmp(".", dir_item->d_name) == 0 ||
            strcmp("..", dir_item->d_name) == 0)
            continue;

        char next_item[PATH_MAX];
        
        strcpy(next_item, source_dir);
        strcat(next_item, dir_item->d_name);
        counter++;

        pid_t pid;
        
        if(counter == N)
            wait(-1);
        
        if((pid = fork()) == 0){
            filescan(next_item, argv[2], next_item);
            exit(0);
        }
    }

    closedir(directory);
    free(source_dir);
    free(str);
}