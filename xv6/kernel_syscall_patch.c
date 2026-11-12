/* ============================================================================
 * Chapter 8: Kernel Space Modifications (xv6-riscv Architecture)
 * Shows the exact 4 kernel-side steps to register and handle a new syscall.
 * ============================================================================ */

#include <stdint.h>

/* ----------------------------------------------------------------------------
 * 1. kernel/syscall.h: Assign a unique syscall number
 * ---------------------------------------------------------------------------- */
#define SYS_fork 1
#define SYS_exit 2
#define SYS_wait 3
#define SYS_pipe 4
#define SYS_read 5
#define SYS_getreadcount 22 /* Newly added system call number */
#define SYS_minios_info 23  /* Custom diagnostic kernel call */

/* ----------------------------------------------------------------------------
 * 2. kernel/sysfile.c: Global kernel counter incremented inside sys_read()
 * ---------------------------------------------------------------------------- */
static uint64_t total_read_calls = 0;

/* Inside existing sys_read(void), the kernel increments this counter:
 *   total_read_calls++;
 */
uint64_t sys_getreadcount(void)
{
    /* Executes in privileged Supervisor Mode: safe to read kernel globals */
    return total_read_calls;
}

/* ----------------------------------------------------------------------------
 * 3. kernel/sysproc.c: Custom kernel diagnostic handler
 * ---------------------------------------------------------------------------- */
extern int proc_active_count(void); /* Kernel process table helper */

uint64_t sys_minios_info(void)
{
    /* In xv6, kernel printf outputs directly to the UART serial console */
    /* printf("[Kernel Mode] Active processes in ptable: %d\n", proc_active_count()); */
    return 1;
}

/* ----------------------------------------------------------------------------
 * 4. kernel/syscall.c: System Call Dispatch Table
 * When a trap occurs, trap.c calls syscall(), which reads register a7
 * (the syscall number) and indexes into this function pointer table.
 * ---------------------------------------------------------------------------- */
extern uint64_t sys_fork(void);
extern uint64_t sys_exit(void);
extern uint64_t sys_wait(void);
extern uint64_t sys_pipe(void);
extern uint64_t sys_read(void);

static uint64_t (*syscalls[])(void) = {
    [SYS_fork] = sys_fork,
    [SYS_exit] = sys_exit,
    [SYS_wait] = sys_wait,
    [SYS_pipe] = sys_pipe,
    [SYS_read] = sys_read,
    [SYS_getreadcount] = sys_getreadcount,
    [SYS_minios_info] = sys_minios_info,
};

/* Kernel dispatcher logic */
void handle_syscall_trap(int syscall_num, uint64_t *return_reg_a0)
{
    int max_syscalls = sizeof(syscalls) / sizeof(syscalls[0]);

    if (syscall_num > 0 && syscall_num < max_syscalls && syscalls[syscall_num])
    {
        /* Invoke kernel function and store return value in trapframe->a0 */
        *return_reg_a0 = syscalls[syscall_num]();
    }
    else
    {
        /* Invalid syscall number */
        *return_reg_a0 = (uint64_t)-1;
    }
}