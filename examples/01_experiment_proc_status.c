#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef __APPLE__
#include <libproc.h>
#endif

int main(void)
{
    pid_t my_pid = getpid();
    printf("=== Experiment 3: Inspecting Process Status ===\n");
    printf("Current Process ID (PID): %d\n", my_pid);

#ifdef __APPLE__
    /* macOS equivalent: Using Darwin libproc */
    struct proc_bsdinfo proc_info;
    int ret = proc_pidinfo(my_pid, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info));

    if (ret > 0) {
        printf("Process Name     : %s\n", proc_info.pbi_name);
        printf("Parent PID (PPID): %d\n", proc_info.pbi_ppid);
        printf("Process Status   : %s\n", (proc_info.pbi_status == 2 ? "Running (SRUN)" : "Sleeping/Other"));
        printf("Total Threads    : %d\n", proc_info.pbi_nthreads);
    } else {
        perror("proc_pidinfo");
    }
#else
    /* Linux equivalent: Reading /proc/<PID>/status */
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/status", my_pid);
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("fopen /proc/<PID>/status");
        return 1;
    }

    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "Name:", 5) == 0 ||
            strncmp(line, "State:", 6) == 0 ||
            strncmp(line, "VmSize:", 7) == 0 ||
            strncmp(line, "Threads:", 8) == 0) {
            printf("%s", line);
        }
    }
    fclose(fp);
#endif

    return 0;
}
