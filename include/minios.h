#ifndef MINIOS_H
#define MINIOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Chapter 3 */
void show_system_info(void);
void shell_loop(void);

/* Chapter 4 */
int run_program(char **args);

#endif /* MINIOS_H */