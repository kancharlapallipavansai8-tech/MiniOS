#include "minios.h"

int run_program(char **args)
{
    if (args == NULL || args[0] == NULL)
    {
        fprintf(stderr, "run: missing command name\n");
        return -1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        /* Child process executes the binary */
        execvp(args[0], args);
        perror("execvp");
        exit(127);
    }

    /* Parent waits for child */
    int status;
    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        if (exit_code != 0)
        {
            printf("[MiniOS] Process exited with code %d\n", exit_code);
        }
    }

    return 0;
}