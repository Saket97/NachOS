# NachOS
NachOS is an operating system simulator (Not another completely heuristic operating system). Code for this operating system is not yet complte.
I started with a very basic implementation of NachOS that implements processes as
user-level light-weight threads and very limited system call support, just
enough to be able to run user programs and print outputs to stdout. 
I have improved the simulator by implementing the following twelve new system calls:
1. SysCall_GetPID: Returns the id of the calling thread.
2. SysCall_GetPPID: Returns the id of the parent of the calling thread.
3. SysCall_Time: Returns the total ticks at present (roughly represents the current
simulated time).
4. SysCall_Sleep: Puts the calling thread to sleep for the number of ticks passed as
argument.
5. SysCall_Yield: The calling thread voluntarily gives up the CPU to the scheduler so
that some other thread can now be scheduled.
6. SysCall_Fork: Create a new thread and duplicate the address space of the calling
thread. The SysCall_Fork syscall is such that
the execution in the child starts as if it has just returned from the
syscall_wrapper_Fork() call. Also, the child pid is returned to the parent and the
child gets a zero return value.
7. SysCall_Join: This is almost same as the wait() call. However the syscall_wrapper_Join() call takes the pid of the thread to be waited on,
as in UNIX waitpid().
8. SysCall_Exec: This is same as the execv() call UNIX. 
9. SysCall_Exit: This implements the syscall_wrapper_Exit() call by destroying the calling
thread.
10. SysCall_NumInstr: Returns the number of instructions executed so far by the calling
process.
11. SysCall_GetReg: Returns the contents of the processor register, number of which is 
passed as argument.
12. SysCall_GetPA: Returns the physical address corresponding to the virtual address
passed as argument.
