#define _POSIX_C_SOURCE 199309L
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <linux/types.h>
#include <linux/time_types.h>

int main ()
{
    time_t timer;
    char buffer[26];
    struct tm* tm_info;
    pid_t pid1, pid2; 
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    timer = time(NULL);
    tm_info = localtime(&timer);

    if ((pid1 = fork()) == 0) {
        pid1 = getpid();
        printf ("Child process with pid=%d\n", getpid()); 
        printf ("Parent process pid=%d\n", getppid());

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        int64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
        strftime(buffer, 26, "%H:%M:%S", tm_info);


        fputs(buffer, stdout);
        printf(":%ld\n", delta_us);


        exit(0);
    }

    if ((pid2 = fork()) == 0) {
        pid2 = getpid();
        printf ("Child process with pid=%d\n", getpid()); 
        printf ("Parent process pid=%d\n", getppid());

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        int64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
        strftime(buffer, 26, "%H:%M:%S", tm_info);


        fputs(buffer, stdout);
        printf(":%ld\n", delta_us);

        exit(0);
    }

    char *command = (char*)malloc(sizeof(char) * 50);
    sprintf(command, "ps -p %d %d", pid1, pid2);
    system(command);

    free(command);
    exit(0);
}