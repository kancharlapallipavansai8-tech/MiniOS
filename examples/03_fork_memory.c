#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/* Section 4.3: The "Two Kitchens" Memory Isolation Experiment
 * Proves that parent and child have separate address spaces
 * even though variables share the exact same virtual memory address.
 */
int main(void)
{
    int secret = 42;

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        /* Child modifies its own copy */
        secret = 100;
        printf("Child  (PID %d): secret = %d  |  Virtual Address: %p\n",
               getpid(), secret, (void *)&secret);
    }
    else
    {
        /* Parent waits briefly to let child print first */
        sleep(1);
        printf("Parent (PID %d): secret = %d  |  Virtual Address: %p\n",
               getpid(), secret, (void *)&secret);
    }

    return 0;
}