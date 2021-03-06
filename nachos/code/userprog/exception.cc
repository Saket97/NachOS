// exception.cc 
//  Entry point into the Nachos kernel from user programs.
//  There are two kinds of things that can cause control to
//  transfer back to here from user code:
//
//  syscall -- The user code explicitly requests to call a procedure
//  in the Nachos kernel.  Right now, the only function we support is
//  "Halt".
//
//  exceptions -- The user code does something that the CPU can't handle.
//  For instance, accessing memory that doesn't exist, arithmetic errors,
//  etc.  
//
//  Interrupts (which can also cause control to transfer from user
//  code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "console.h"
#include "synch.h"

//----------------------------------------------------------------------
// ExceptionHandler
//  Entry point into the Nachos kernel.  Called when a user program
//  is executing, and either does a syscall, or generates an addressing
//  or arithmetic exception.
//
//  For system calls, the following is the calling convention:
//
//  system call code -- r2
//      arg1 -- r4
//      arg2 -- r5
//      arg3 -- r6
//      arg4 -- r7
//
//  The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//  "which" is the kind of exception.  The list of possible exceptions 
//  are in machine.h.
//----------------------------------------------------------------------
static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

static void ConvertIntToHex (unsigned v, Console *console)
{
   unsigned x;
   if (v == 0) return;
   ConvertIntToHex (v/16, console);
   x = v % 16;
   if (x < 10) {
      writeDone->P() ;
      console->PutChar('0'+x);
  }
  else {
      writeDone->P() ;
      console->PutChar('a'+x-10);
  }
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    int memval, vaddr, printval, tempval, exp;
    unsigned printvalus;        // Used for printing in hex
    if (!initializedConsoleSemaphores) {
        writeDone = new Semaphore("write done", 1);
        readAvail = new Semaphore("read avail", 0);
        initializedConsoleSemaphores = true;
    }
    Console *console = new Console(NULL, NULL, ReadAvail, WriteDone, 0);;

    if ((which == SyscallException) && (type == SysCall_Halt)) {
        DEBUG('a', "Shutdown, initiated by user program.\n");
        interrupt->Halt();
    }
    else if ((which == SyscallException) && (type == SysCall_PrintInt)) {
        printval = machine->ReadRegister(4);
        if (printval == 0) {
            writeDone->P() ;
            console->PutChar('0');
        }
        else {
            if (printval < 0) {
                writeDone->P() ;
                console->PutChar('-');
                printval = -printval;
            }
            tempval = printval;
            exp=1;
            while (tempval != 0) {
                tempval = tempval/10;
                exp = exp*10;
            }
            exp = exp/10;
            while (exp > 0) {
                writeDone->P() ;
                console->PutChar('0'+(printval/exp));
                printval = printval % exp;
                exp = exp/10;
            }
        }
       // Advance program counters.
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    }
    else if ((which == SyscallException) && (type == SysCall_PrintChar)) {
        writeDone->P() ;
        console->PutChar(machine->ReadRegister(4));   // echo it!
       // Advance program counters.
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    }
    else if ((which == SyscallException) && (type == SysCall_PrintString)) {
        vaddr = machine->ReadRegister(4);
        machine->ReadMem(vaddr, 1, &memval);
        while ((*(char*)&memval) != '\0') {
            writeDone->P() ;
            console->PutChar(*(char*)&memval);
            vaddr++;
            machine->ReadMem(vaddr, 1, &memval);
        }
       // Advance program counters.
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    }
    else if ((which == SyscallException) && (type == SysCall_PrintIntHex)) {
        printvalus = (unsigned)machine->ReadRegister(4);
        writeDone->P() ;
        console->PutChar('0');
        writeDone->P() ;
        console->PutChar('x');
        if (printvalus == 0) {
            writeDone->P() ;
            console->PutChar('0');
        }
        else {
            ConvertIntToHex (printvalus, console);
        }
           // Advance program counters.
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    } else if ((which==SyscallException) && (type == SysCall_GetReg)) {
        if (machine->ReadRegister(4) <= 31 &&  machine->ReadRegister(4) >= 0)
        {
            machine->WriteRegister(2, machine->ReadRegister(4));
            machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
            machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
            machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
        }
    } else if ((which==SyscallException) && (type == SysCall_GetPA)) {
        int phyaddr;
        ExceptionType e = machine->Translate(machine->ReadRegister(4), &phyaddr, 1, false);
        if (e == NoException)
        {
          machine->WriteRegister(2, phyaddr);
        }
        else
            machine->WriteRegister(2,-1);
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    } else if ((which == SyscallException) && (type == SysCall_GetPID)) {
        machine->WriteRegister(2,currentThread->pid);
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    } else if ((which == SyscallException) && (type == SysCall_GetPPID)){
        machine->WriteRegister(2,currentThread->ppid);
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    } else if ((which == SyscallException) && (type == SysCall_Time)) {
        machine->WriteRegister(2,stats->totalTicks);
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    } else if ((which == SyscallException) && (type == SysCall_Yield)) {
        currentThread->YieldCPU(); // TODO: Check whether to update pc before or after
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    } else if ((which == SyscallException) && (type == SysCall_Sleep)){
        unsigned int off = machine->ReadRegister(4);
        NachOSThread *prevThread = currentThread;
        IntStatus oldLevel = interrupt->SetLevel(IntOff);
        unsigned int wakeup_time = (unsigned int) stats->totalTicks + off;
        listOfSleepNodes->SortedInsert((void *) prevThread, wakeup_time);
        currentThread->PutThreadToSleep();
        (void) interrupt->SetLevel(oldLevel);
        machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    } else if ((which == SyscallException) && (type == SysCall_Exit)){
	int exitcode = machine->ReadRegister(4);
	Log *log = (Log *)threadLog->SortedRemove(currentThread->pid);
        log->exitCode = exitcode;
	log->exitCalled = 1;
	threadLog->SortedInsert((void *)log, currentThread->pid)
	ListElement *ptr = listOfSleepNodes->first;
	ListElement *prev = listOfSleepNodes->first;
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	while (ptr != NULL){
		Thread *thread= (Thread *)ptr->item;
		if(thread->pid == currentThread->ppid && ptr->key < 0){
			prev->next = ptr->next;
			delete ptr;
			scheduler->MoveThreadToReadyQueue(thread);
			break;
		}
		prev = ptr;
		if(ptr != NULL)
			ptr = ptr->next;
	}
	(void) interrupt->SetLevel(oldLevel);
    } else if ((which == SyscallException) && (type == SysCall_Join)) {
	int childPid = machine->ReadRegister(4);
	int FLAG = 0;
	node *ptr = currentThread->child;
	while(ptr != NULL){
	if(ptr->threadId == childPid){
		FLAG=1;
		break;}
	ptr = ptr->next;
	}
	if(FLAG == 0)
		machine->WriteRegister(2,-1);
	else{
	Log *log = (Log *)threadLog->SortedRemove(childPid);
	threadLog->SortedInsert((void *)log,childPid);
	if(log->exitCalled != 1){
		NachOSThread *prevThread = currentThread;
		IntStatus oldLevel = interrupt->SetLevel(IntOff);
		listOfSleepNodes->SortedInsert((void *)prevThread, -1);
		currentThread->PutThreadToSleep();
		(void) interrupt->SetLevel(oldLevel);
		log = (Log *)threadLog->SortedRemove(childPid);
		threadLog->SortedInsert((void *)log,childPid);
	}
	machine->WriteRegister(2,log->exitCode);	
	}
	machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
        machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
        machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
    } else if ((which == SyscallException) && (type == SysCall_Fork)){
        NachOSThread *newThread = new NachOSThread("newThread");
        new_space = new ProcessAddrSpace();
        newThread->space = new_space;
        for(int ptr_for_phy_addr = 0; ptr_for_phy_addr < currentThread->space->numVirtualPages; i++){
            machine->mainMemory[new_space->KernelPageTable[ptr_for_phy_addr]] = machine->mainMemory[currentThread->space->KernelPageTable[ptr_for_phy_addr]];
        }
        




    }
    else
    {
        printf("Unexpected user mode exception %d %d\n", which, type);
        ASSERT(FALSE);
    }
}
    
