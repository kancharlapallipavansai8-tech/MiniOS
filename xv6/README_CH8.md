# Chapter 8: Crossing into Kernel Space (xv6 System Call Architecture)

## End-to-End System Call Execution Flow

1. **User Program (`user/test_syscall.c`)**: Calls C wrapper `getreadcount()`.
2. **Assembly Stub (`user/usys.S`)**: Loads `SYS_getreadcount` (`22`) into register `a7` and executes the `ecall` instruction.
3. **Hardware Privilege Switch**: CPU switches from **User Mode (U-mode)** to **Supervisor Mode (S-mode)**, saves user registers into the process's `trapframe`, and jumps to `uservec` $\rightarrow$ `usertrap()`.
4. **Syscall Dispatcher (`kernel/syscall.c`)**: `syscall()` reads `p->trapframe->a7`, validates the index against `syscalls[]`, and invokes `sys_getreadcount()`.
5. **Kernel Handler (`kernel/sysfile.c`)**: Reads protected kernel variable `total_read_calls` and returns the integer value.
6. **Return to User Space**: `syscall()` writes the return value into `p->trapframe->a0`, restores user registers via `sret`, and resumes user execution.