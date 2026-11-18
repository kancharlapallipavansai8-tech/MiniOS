# MiniOS

A modular Operating System command shell, concurrency laboratory, and xv6 kernel-syscall reference implemented in C, natively adapted for macOS (Darwin/XNU).

---

## Project Overview

MiniOS demonstrates core Operating System internals through an interactive C shell (`MiniOS>`) alongside standalone systems experiments and RISC-V xv6 kernel extensions. Because macOS lacks Linux's `/proc` filesystem and POSIX unnamed semaphores (`sem_init`), MiniOS uses native BSD `sysctl`, Mach microkernel APIs (`host_statistics64`), `pthread_mutex_t`, and POSIX named semaphores (`sem_open`).

### Core Capabilities
* **Kernel Telemetry (`system`):** Queries live system uptime and active, wired, and free physical RAM pages via BSD `sysctl` and Mach IPC.
* **Process Management (`run`):** Spawns external programs using `fork()`, Copy-On-Write address space isolation, `execvp()`, and zombie-free `waitpid()` reaping.
* **Inter-Process Communication (`submit`):** Executes background worker jobs and streams results back to the parent shell over unidirectional kernel `pipe()` buffers.
* **POSIX Multithreading (`workers`):** Runs parallel worker thread pools (`pthread_create`, `pthread_join`) sharing heap and global memory within a single PID.
* **Synchronization Lab (`race`):** Contrasts non-atomic counter race conditions against `pthread_mutex_t` mutual exclusion locks and bounded-buffer synchronization.
* **Kernel-Space Syscall Extension (`xv6/`):** Implements a custom `getreadcount()` system call across the RISC-V xv6 User-to-Kernel `ecall` trap boundary.

---

## Directory Layout

```text
MiniOS/
├── Makefile                    # Clang build orchestration (-Wall -Wextra -pthread)
├── README.md                   # Project overview, setup, and usage documentation
├── include/
│   └── minios.h                # Shared declarations across all MiniOS subsystems
├── src/
│   ├── main.c                  # Shell entry point
│   ├── shell.c                 # Interactive command tokenizer and dispatcher
│   ├── monitor.c               # macOS sysctl & Mach host_statistics64 telemetry
│   ├── process.c               # fork() -> execvp() -> waitpid() process runner
│   ├── ipc.c                   # Unidirectional pipe() worker communication
│   ├── workers.c               # POSIX multithreaded worker pool
│   └── sync.c                  # Unsynchronized race vs. mutex lock demonstration
├── examples/                   # Standalone OS experiments (uptime, fork, pipes, threads, sync)
├── tests/
│   └── verify_all.sh           # Automated end-to-end integration test suite
└── xv6/
    ├── kernel_syscall_patch.c  # Kernel syscall table & sys_getreadcount() handler
    ├── user_syscall_test.c     # User-space ecall stub & verification utility
    └── README_CH8.md           # User-to-Kernel trap execution walkthrough

git clone [https://github.com/kancharlapallipavansai8-tech/MiniOS.git](https://github.com/kancharlapallipavansai8-tech/MiniOS.git)
cd MiniOS
make

./minios

MiniOS> help             # Display available commands
MiniOS> system           # Show macOS uptime and physical RAM metrics
MiniOS> run date         # Execute an external binary via fork/execvp/waitpid
MiniOS> submit log_scan  # Run a worker job and collect output via pipe()
MiniOS> workers 4        # Spawn 4 concurrent POSIX threads
MiniOS> race             # Compare unsynchronized vs. mutex-locked counters
MiniOS> quit             # Exit MiniOS

bash tests/verify_all.sh
