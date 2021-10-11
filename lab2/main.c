#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"
#include "fcntl.h"
#include "stdlib.h"

int main(int argc, char* argv[]) {
    char *name;
    size_t len;
    printf("Enter file:");
    if (getline(&name, &len, stdin) == -1) {
        perror("can't get file");
    }
    name[strlen(name) - 1] = '\0';
    int filedescriptor[2];
    if (pipe(filedescriptor) != 0) {
        perror("can't create pipe");
    }
    int readd = filedescriptor[0];
    int writed = filedescriptor[1];
    int file = open(name, O_RDONLY);
    if (file == -1) {
        perror("can't open the file");
    }
    pid_t pid = fork();
    if (pid == 0) {
        if(dup2(writed, fileno(stdout)) == -1) {
            exit(1);
        }
        close(writed);
        if(dup2(file, fileno(stdin)) == -1) {
            exit(1);
        }
        execl("childprocess", "childprocess", NULL);
        perror("can't execute child process");
        exit(EXIT_FAILURE);
    } else if(pid == -1) {
        perror("pid error");
    }

    char *buf[BUFSIZ];
    int n;
    while (n = (read(readd, &buf, sizeof(char))) > 0) {
        write(1, &buf, n);
    }
    return 0;
}