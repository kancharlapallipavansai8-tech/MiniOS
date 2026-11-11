#include <stdio.h>
#include <time.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/mach_host.h>

int main(void)
{
    /* 1. Uptime via sysctl kern.boottime */
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};

    if (sysctl(mib, 2, &boottime, &len, NULL, 0) == 0) {
        time_t now = time(NULL);
        time_t uptime_seconds = now - boottime.tv_sec;
        printf("macOS Uptime: %ld seconds (%ld hours, %ld mins)\n",
               uptime_seconds, uptime_seconds / 3600, (uptime_seconds % 3600) / 60);
    } else {
        perror("sysctl boottime");
        return 1;
    }

    /* 2. Total RAM via sysctl hw.memsize */
    int64_t total_mem = 0;
    len = sizeof(total_mem);
    if (sysctlbyname("hw.memsize", &total_mem, &len, NULL, 0) == 0) {
        printf("Physical RAM: %.2f GB\n", (double)total_mem / (1024 * 1024 * 1024));
    }

    /* 3. Memory Statistics via Mach Host API */
    vm_size_t page_size;
    mach_port_t host_port = mach_host_self();
    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);

    if (host_page_size(host_port, &page_size) == KERN_SUCCESS &&
        host_statistics64(host_port, HOST_VM_INFO64, (host_info64_t)&vm_stats, &count) == KERN_SUCCESS) {
        long long free_bytes = (long long)vm_stats.free_count * page_size;
        long long active_bytes = (long long)vm_stats.active_count * page_size;
        printf("Memory State: %.2f GB Active, %.2f GB Free\n",
               (double)active_bytes / (1024 * 1024 * 1024),
               (double)free_bytes / (1024 * 1024 * 1024));
    }

    return 0;
}
