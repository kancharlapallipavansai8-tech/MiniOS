#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/mach_host.h>
#endif

int main(void)
{
    printf("=== Experiment 2: Reading Memory Info in C ===\n");

#ifdef __APPLE__
    /* macOS Implementation: Darwin Kernel Memory Statistics */
    int64_t memsize_bytes = 0;
    size_t len = sizeof(memsize_bytes);

    if (sysctlbyname("hw.memsize", &memsize_bytes, &len, NULL, 0) == 0) {
        printf("Platform: macOS (via Darwin sysctl hw.memsize)\n");
        printf("Total Physical Memory : %lld bytes (%.2f GB)\n",
               memsize_bytes, (double)memsize_bytes / (1024 * 1024 * 1024));
    } else {
        perror("sysctlbyname hw.memsize");
    }

    vm_size_t page_size;
    mach_port_t host_port = mach_host_self();
    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);

    if (host_page_size(host_port, &page_size) == KERN_SUCCESS &&
        host_statistics64(host_port, HOST_VM_INFO64, (host_info64_t)&vm_stats, &count) == KERN_SUCCESS) {
        printf("Virtual Memory Page Size : %lu bytes\n", page_size);
        printf("Kernel Free Pages        : %u (%.2f MB free)\n",
               vm_stats.free_count, ((double)vm_stats.free_count * page_size) / (1024 * 1024));
        printf("Kernel Active Pages      : %u (%.2f MB active)\n",
               vm_stats.active_count, ((double)vm_stats.active_count * page_size) / (1024 * 1024));
    }

#else
    /* Linux Implementation: Reading /proc/meminfo */
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("fopen /proc/meminfo");
        return 1;
    }

    char line[256];
    printf("Platform: Linux (via /proc/meminfo)\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "MemTotal:", 9) == 0 || strncmp(line, "MemAvailable:", 13) == 0) {
            printf("%s", line);
        }
    }
    fclose(fp);
#endif

    return 0;
}
