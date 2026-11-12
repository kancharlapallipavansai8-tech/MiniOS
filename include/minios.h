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

/* Chapter 5 */
int send_result(int fd, const char *result);
int receive_result(int fd, char *buf, size_t capacity);
int submit_job(const char *job_name);

/* Chapter 6 */
int run_workers(int count);

/* Chapter 7 */
void run_sync_demo(void);

#endif /* MINIOS_H */