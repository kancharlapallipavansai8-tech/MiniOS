#include "minios.h"

/* Section 5.3: MiniOS job manager IPC
 * TODO 1: Write result to the pipe descriptor
 */
int send_result(int fd, const char *result)
{
    if (result == NULL)
    {
        return -1;
    }
    ssize_t bytes_written = write(fd, result, strlen(result));
    return (bytes_written >= 0) ? 0 : -1;
}

/* TODO 2 & 3: Read result from pipe and ensure it is null-terminated */
int receive_result(int fd, char *buf, size_t capacity)
{
    if (buf == NULL || capacity == 0)
    {
        return -1;
    }
    ssize_t bytes_read = read(fd, buf, capacity - 1);
    if (bytes_read < 0)
    {
        perror("read");
        return -1;
    }
    buf[bytes_read] = '\0';
    return 0;
}

/* Spawns a worker process to compute a job and returns the output via pipe */
int submit_job(const char *job_name)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (pid == 0)
    {
        /* Child Worker: writes to pipe */
        close(pipefd[0]); /* Close unused read end */

        char output[128];
        snprintf(output, sizeof(output), "Job '%s' completed successfully by PID %d", job_name, getpid());

        send_result(pipefd[1], output);
        close(pipefd[1]);
        exit(0);
    }

    /* Parent MiniOS Manager: reads from pipe */
    close(pipefd[1]); /* Close unused write end */

    char response[128];
    receive_result(pipefd[0], response, sizeof(response));
    close(pipefd[0]);

    waitpid(pid, NULL, 0);
    printf("[MiniOS IPC] %s\n", response);

    return 0;
}