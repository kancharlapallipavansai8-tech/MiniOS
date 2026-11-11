#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];

    /* 1. Create pipe */
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        /* Child process: reads from pipe */
        close(fd[1]); /* Close unused write end */

        char buffer[100];
        ssize_t n = read(fd[0], buffer, sizeof(buffer) - 1);
        if (n < 0)
        {
            perror("read");
            exit(1);
        }
        buffer[n] = '\0';
        printf("Child (PID %d) received message: \"%s\"\n", getpid(), buffer);

        close(fd[0]);
        exit(0);
    }

    /* Parent process: writes to pipe */
    close(fd[0]); /* Close unused read end */

    const char *message = "Hello from MiniOS parent process!";
    printf("Parent (PID %d) sending: \"%s\"\n", getpid(), message);
    write(fd[1], message, strlen(message));

    close(fd[1]); /* Close write end to signal EOF */
    waitpid(pid, NULL, 0);

    return 0;
}