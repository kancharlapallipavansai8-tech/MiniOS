#include "minios.h"

void shell_loop(void)
{
    char line[256];

    printf("=========================================\n");
    printf(" Welcome to MiniOS (macOS Native Shell)  \n");
    printf(" Type 'help' for commands, 'quit' to exit\n");
    printf("=========================================\n\n");

    while (1) {
        printf("MiniOS> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            continue;
        }

        if (strcmp(line, "help") == 0) {
            printf("\nMiniOS Commands:\n");
            printf("  help       - Show available commands\n");
            printf("  system     - Display OS uptime and memory metrics\n");
            printf("  quit       - Exit MiniOS\n\n");
        } else if (strcmp(line, "system") == 0) {
            show_system_info();
        } else if (strcmp(line, "quit") == 0) {
            printf("Exiting MiniOS. Goodbye!\n");
            break;
        } else {
            printf("Unknown command: '%s'. Type 'help' for valid commands.\n", line);
        }
    }
}
