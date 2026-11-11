#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/sysctl.h>

long get_current_uptime(void) {
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};
    if (sysctl(mib, 2, &boottime, &len, NULL, 0) == 0) {
        time_t now = time(NULL);
        return now - boottime.tv_sec;
    }
    return -1;
}

int main(void)
{
    printf("=== Experiment 1: Which Value Changes? ===\n");

    long first_uptime = get_current_uptime();
    printf("Reading 1 - Uptime: %ld seconds\n", first_uptime);

    printf("Waiting 3 seconds...\n");
    sleep(3);

    long second_uptime = get_current_uptime();
    printf("Reading 2 - Uptime: %ld seconds\n", second_uptime);

    printf("Difference (Delta): %ld seconds\n", second_uptime - first_uptime);
    printf("Conclusion: The uptime counter increments continuously with wall-clock time.\n\n");

    return 0;
}
