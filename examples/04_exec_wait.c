#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Section 4.4 & 4.5: exec() and waitpid()
 * Child replaces its memory image with an external binary ('date').
 * Parent reaps the child and inspects its termination status.
 */
int main(void)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child (PID %d): Executing external program 'date'...\n", getpid());
        execlp("date", "date", NULL);

        /* execlp returns ONLY if an error occurred */
        perror("execlp failed");
        exit(1);
    }

    /* Parent waits for child */
    printf("Parent: Waiting for child PID %d to terminate...\n", pid);
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
    {
        printf("Parent: Child process finished with exit code %d.\n", WEXITSTATUS(status));
    }

    return 0;
}