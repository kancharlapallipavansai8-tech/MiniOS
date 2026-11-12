#include "minios.h"

#define MAX_ARGS 32

static void parse_and_execute(char *line)
{
    char *args[MAX_ARGS];
    int argc = 0;

    char *token = strtok(line, " \t");
    while (token != NULL && argc < MAX_ARGS - 1)
    {
        args[argc++] = token;
        token = strtok(NULL, " \t");
    }
    args[argc] = NULL;

    if (argc == 0)
    {
        return;
    }

    if (strcmp(args[0], "help") == 0)
    {
        printf("\nMiniOS Commands:\n");
        printf("  help             - Show available commands\n");
        printf("  system           - Display OS uptime and memory metrics\n");
        printf("  run <cmd> [args] - Execute a program using fork/exec/waitpid\n");
        printf("  submit <job>     - Run a job in a worker process and collect result via pipe\n");
        printf("  workers <n>      - Spawn n concurrent POSIX threads inside MiniOS\n");
        printf("  quit             - Exit MiniOS\n\n");
    }
    else if (strcmp(args[0], "system") == 0)
    {
        show_system_info();
    }
    else if (strcmp(args[0], "run") == 0)
    {
        if (argc < 2)
        {
            printf("Usage: run <program> [arguments...]\n");
        }
        else
        {
            run_program(&args[1]);
        }
    }
    else if (strcmp(args[0], "submit") == 0)
    {
        if (argc < 2)
        {
            printf("Usage: submit <job_name>\n");
        }
        else
        {
            submit_job(args[1]);
        }
    }
    else if (strcmp(args[0], "workers") == 0)
    {
        int count = (argc >= 2) ? atoi(args[1]) : 4;
        run_workers(count);
    }
    else if (strcmp(args[0], "quit") == 0)
    {
        printf("Exiting MiniOS. Goodbye!\n");
        exit(0);
    }
    else
    {
        printf("Unknown command: '%s'. Type 'help' for valid commands.\n", args[0]);
    }
}

void shell_loop(void)
{
    char line[256];

    printf("=========================================\n");
    printf(" Welcome to MiniOS (macOS Native Shell)  \n");
    printf(" Type 'help' for commands, 'quit' to exit\n");
    printf("=========================================\n\n");

    while (1)
    {
        printf("MiniOS> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
        {
            break;
        }

        line[strcspn(line, "\n")] = '\0';
        parse_and_execute(line);
    }
}