#include "minios.h"
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <time.h>

void show_system_info(void)
{
    printf("\n=== MiniOS System Status (macOS) ===\n");

    /* 1. System Uptime via sysctl kern.boottime */
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};

    if (sysctl(mib, 2, &boottime, &len, NULL, 0) == 0) {
        time_t now = time(NULL);
        time_t uptime_seconds = now - boottime.tv_sec;
        long days = uptime_seconds / 86400;
        long hours = (uptime_seconds % 86400) / 3600;
        long mins = (uptime_seconds % 3600) / 60;
        long secs = uptime_seconds % 60;

        printf("Uptime       : %ld days, %02ld:%02ld:%02ld (%ld total seconds)\n",
               days, hours, mins, secs, uptime_seconds);
    } else {
        perror("sysctl boottime");
    }

    /* 2. Total RAM via sysctl hw.memsize */
    int64_t total_mem = 0;
    len = sizeof(total_mem);
    if (sysctlbyname("hw.memsize", &total_mem, &len, NULL, 0) == 0) {
        printf("Physical RAM : %.2f GB\n", (double)total_mem / (1024 * 1024 * 1024));
    }

    /* 3. Memory Statistics via Mach Host API */
    vm_size_t page_size;
    mach_port_t host_port = mach_host_self();
    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);

    if (host_page_size(host_port, &page_size) == KERN_SUCCESS &&
        host_statistics64(host_port, HOST_VM_INFO64, (host_info64_t)&vm_stats, &count) == KERN_SUCCESS) {
        long long active_mem = (long long)vm_stats.active_count * (long long)page_size;
        long long free_mem   = (long long)vm_stats.free_count   * (long long)page_size;
        long long wired_mem  = (long long)vm_stats.wire_count   * (long long)page_size;

        printf("Active Memory: %.2f GB\n", (double)active_mem / (1024 * 1024 * 1024));
        printf("Wired Memory : %.2f GB\n", (double)wired_mem / (1024 * 1024 * 1024));
        printf("Free Memory  : %.2f GB\n", (double)free_mem / (1024 * 1024 * 1024));
    }

    printf("=====================================\n\n");
}
