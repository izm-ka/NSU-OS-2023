#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

extern char** environ;

int execvpe(char* filename, char* arg[], char* envp[]) {
    char** ptr = environ;
    environ = envp;
    execvp(filename, arg);
    environ = ptr;
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No executed file\n");
        exit(EXIT_FAILURE);
    }

    char* oldPath = getenv("PATH");

    if (oldPath == NULL) {
        fprintf(stderr, "PATH not found in environment\n");
        exit(EXIT_FAILURE);
    }

    printf("Old PATH: %s\n", oldPath);

    char* newPath = malloc(strlen(oldPath) + 2);
    if (newPath == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    sprintf(newPath, "%s.", oldPath);

    printf("Modified PATH: %s\n", newPath);

    char* nenv[] = { "NAME=value","nextname=nextvalue","HOME=/xyz","PATH=.",(char*)0 };


    if (execvpe(argv[1], &argv[1], nenv) == -1) {
        perror("execvpe");
        exit(EXIT_FAILURE);
    }

    return 0;
}
