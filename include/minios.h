#ifndef MINIOS_H
#define MINIOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void show_system_info(void);
void shell_loop(void);

#endif /* MINIOS_H */
