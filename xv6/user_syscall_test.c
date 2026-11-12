/* ============================================================================
 * Chapter 8: User Space Interface & Test Utility (xv6 user/test_syscall.c)
 * Demonstrates how a unprivileged user program invokes our new system call.
 * ============================================================================ */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/* ----------------------------------------------------------------------------
 * 1. user/user.h: Function prototypes exposed to user applications
 * ---------------------------------------------------------------------------- */
int getreadcount(void);
int minios_info(void);

/* ----------------------------------------------------------------------------
 * 2. user/usys.pl: Assembly stub generation rule
 *    entry("getreadcount"); -> generates RISC-V assembly:
 *      .global getreadcount
 *      getreadcount:
 *          li a7, SYS_getreadcount   # Load syscall ID into register a7
 *          ecall                     # Hardware trap: User Mode -> Kernel Mode
 *          ret                       # Return result in register a0
 * ---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * 3. user/test_syscall.c: Verification program run inside xv6 shell ($ test_syscall)
 * ---------------------------------------------------------------------------- */
#ifdef XV6_USER_BUILD
int main(int argc, char *argv[])
{
    int before = getreadcount();
    printf("Initial read() syscall count in kernel: %d\n", before);

    /* Trigger actual read() syscalls */
    char buf[16];
    int fd = open("README", 0);
    if (fd >= 0)
    {
        read(fd, buf, sizeof(buf));
        read(fd, buf, sizeof(buf));
        close(fd);
    }

    int after = getreadcount();
    printf("Updated read() syscall count in kernel: %d\n", after);
    printf("Delta verified: %d read syscalls trapped into kernel.\n", after - before);

    minios_info();
    exit(0);
}
#endif