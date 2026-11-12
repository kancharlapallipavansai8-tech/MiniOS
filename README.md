# MiniOS — Operating Systems Lab Project (macOS Native & xv6)

A modular Operating System shell, concurrency laboratory, and xv6 kernel-syscall reference built in C on macOS (Darwin/XNU).

---

## Chapter 9: Putting the Story Together — How Everything Connects

Every command inside `MiniOS>` demonstrates a distinct layer of how an Operating System manages hardware, execution, and isolation:

1. **Inspecting Kernel State (`system` $\rightarrow$ `src/monitor.c`)**:
   User programs cannot read raw hardware counters directly. On Linux, the kernel exposes virtual files under `/proc`; on macOS, MiniOS queries the XNU/Darwin kernel via BSD `sysctl` (`CTL_KERN / KERN_BOOTTIME`, `hw.memsize`) and Mach microkernel IPC (`host_statistics64`) to report live uptime and physical RAM page allocations.
2. **Process Creation & Isolation (`run <cmd>` $\rightarrow$ `src/process.c`)**:
   Running an external program requires three steps: `fork()` clones the MiniOS shell into a separate virtual address space ("two kitchens" via Copy-On-Write), `execvp()` replaces the child's memory image with the requested binary, and `waitpid()` blocks the parent to reap the child's exit status without leaving zombie processes.
3. **Inter-Process Communication (`submit <job>` $\rightarrow$ `src/ipc.c`)**:
   Because forked processes cannot touch each other's variables, `pipe(fd)` allocates a unidirectional kernel FIFO buffer. The child worker writes its result to `fd[1]` and closes the descriptor to send `EOF`, allowing the parent manager to read the output safely from `fd[0]`.
4. **Lightweight Concurrency (`workers <n>` $\rightarrow$ `src/workers.c`)**:
   When tasks need to share data without pipe serialization overhead, `pthread_create()` spawns multiple execution flows ("many workers in one shared room"). Each thread gets its own call stack and registers while sharing global variables and heap memory inside the same PID.
5. **Synchronization & Critical Sections (`race` $\rightarrow$ `src/sync.c`)**:
   Shared memory introduces race conditions because `counter++` is a non-atomic `LOAD-ADD-STORE` sequence. MiniOS demonstrates lost updates under unsynchronized execution and resolves them using `pthread_mutex_t` mutual exclusion locks (and POSIX named semaphores `sem_open` on macOS).
6. **Crossing the Hardware Privilege Boundary (`xv6/`)**:
   Every system call above (`fork`, `read`, `write`, `pipe`) transitions the CPU from unprivileged **User Mode** into privileged **Kernel Mode** via a hardware trap (`ecall` in RISC-V xv6), indexing into the kernel's `syscalls[]` dispatch table before returning via `sret`.

---

## Chapter 10: Complete Project Directory Tree

```text
MiniOS/
├── Makefile                            # Clang build orchestration (-Wall -Wextra -pthread)
├── README.md                           # Architecture story, build guide, and directory map
├── .gitignore                          # Excludes compiled binaries and *.o build artifacts
├── include/
│   └── minios.h                        # Unified header declarations across Chapters 3–7
├── src/
│   ├── main.c                          # Entry point launching the interactive shell loop
│   ├── shell.c                         # Command tokenizer & dispatcher (system, run, submit, workers, race)
│   ├── monitor.c                       # Ch 3: macOS sysctl & Mach host_statistics64 telemetry
│   ├── process.c                       # Ch 4: fork() -> execvp() -> waitpid() process runner
│   ├── ipc.c                           # Ch 5: Unidirectional pipe() worker communication
│   ├── workers.c                       # Ch 6: POSIX multithreaded worker pool (pthread)
│   └── sync.c                          # Ch 7: Unsynchronized race vs. pthread_mutex_t demo
├── examples/
│   ├── 01_uptime_proc.c                # Ch 3.1: Linux /proc/uptime reference implementation
│   ├── 01_uptime_macos.c               # Ch 3.1: macOS sysctl kern.boottime implementation
│   ├── 01_experiment_uptime_delta.c    # Ch 3.2: Elapsed uptime delta measurement
│   ├── 01_experiment_meminfo.c         # Ch 3.2: Physical RAM & Mach VM page breakdown
│   ├── 01_experiment_proc_status.c     # Ch 3.2: Process inspection via Darwin libproc
│   ├── 02_fork_basics.c                # Ch 4.1: Parent vs. child PID branching
│   ├── 03_fork_memory.c                # Ch 4.3: Virtual address space isolation ("Two Kitchens")
│   ├── 04_exec_wait.c                  # Ch 4.4: Image replacement with execlp() & waitpid()
│   ├── 05_pipe_ipc.c                   # Ch 5.1: Standalone parent-child pipe messaging
│   ├── 06_threads_intro.c              # Ch 6.1: Stack vs. global memory addresses across threads
│   ├── 06_thread_return.c              # Ch 6.3: Passing structs & returning heap values via pthread_join
│   ├── 07_race_condition.c             # Ch 7.1: Non-atomic counter corruption under concurrency
│   ├── 08_semaphore_mutex.c            # Ch 7.2: Mutex & macOS named semaphore (sem_open) fix
│   └── 09_producer_consumer.c          # Ch 7.3: Bounded circular buffer with mutex + condvars
├── tests/
│   └── verify_all.sh                   # Automated end-to-end verification script for all commands
└── xv6/
    ├── kernel_syscall_patch.c          # Ch 8: Kernel syscall table & sys_getreadcount() handler
    ├── user_syscall_test.c             # Ch 8: User-space ecall stub & verification program
    └── README_CH8.md                   # Ch 8: User-to-Kernel trap execution walkthrough