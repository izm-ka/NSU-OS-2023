#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

extern char** environ;

int execvpe(char* filename, char* arg[], char* envp[]) {
    char** ptr = environ;
    environ = envp;

    for (int i = 0; envp[i] != NULL; i++) {
        if (strncmp(envp[i], "PATH=", 5) == 0) {
            char* newPath = malloc(strlen(envp[i]) + 2);
            if (newPath) {
                sprintf(newPath, "%s.", envp[i]);
                envp[i] = newPath;
            }
            break;
        }
    }
    execvp(filename, arg);
    environ = ptr;

    if (newPath) {
        free(newPath);
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No executed file\n");
        exit(EXIT_FAILURE);
    }
    char* nenv[] = { "NAME=value","nextname=nextvalue","HOME=/xyz","PATH=.",(char*)0 };

    if (execvpe(argv[1], &argv[1], nenv) == -1) {
        perror("execvpe");
        exit(EXIT_FAILURE);
    }

    return 0;
}
