# 1 "start.s"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "start.s"
# 10 "start.s"
# 1 "../userprog/syscall.h" 1
# 16 "../userprog/syscall.h"
# 1 "../threads/copyright.h" 1
# 17 "../userprog/syscall.h" 2
# 11 "start.s" 2

        .text
        .align 2
# 24 "start.s"
 .globl __start
 .ent __start
__start:
 jal main
 move $4,$0
 jal syscall_wrapper_Exit
 .end __start
# 45 "start.s"
 .globl syscall_wrapper_Halt
 .ent syscall_wrapper_Halt
syscall_wrapper_Halt:
 addiu $2,$0,0
 syscall
 j $31
 .end syscall_wrapper_Halt

 .globl syscall_wrapper_Exit
 .ent syscall_wrapper_Exit
syscall_wrapper_Exit:
 addiu $2,$0,1
 syscall
 j $31
 .end syscall_wrapper_Exit

 .globl syscall_wrapper_Exec
 .ent syscall_wrapper_Exec
syscall_wrapper_Exec:
 addiu $2,$0,2
 syscall
 j $31
 .end syscall_wrapper_Exec

 .globl syscall_wrapper_Join
 .ent syscall_wrapper_Join
syscall_wrapper_Join:
 addiu $2,$0,3
 syscall
 j $31
 .end syscall_wrapper_Join

 .globl syscall_wrapper_Create
 .ent syscall_wrapper_Create
syscall_wrapper_Create:
 addiu $2,$0,4
 syscall
 j $31
 .end syscall_wrapper_Create

 .globl syscall_wrapper_Open
 .ent syscall_wrapper_Open
syscall_wrapper_Open:
 addiu $2,$0,5
 syscall
 j $31
 .end syscall_wrapper_Open

 .globl syscall_wrapper_Read
 .ent syscall_wrapper_Read
syscall_wrapper_Read:
 addiu $2,$0,6
 syscall
 j $31
 .end syscall_wrapper_Read

 .globl syscall_wrapper_Write
 .ent syscall_wrapper_Write
syscall_wrapper_Write:
 addiu $2,$0,7
 syscall
 j $31
 .end syscall_wrapper_Write

 .globl syscall_wrapper_Close
 .ent syscall_wrapper_Close
syscall_wrapper_Close:
 addiu $2,$0,8
 syscall
 j $31
 .end syscall_wrapper_Close

 .globl syscall_wrapper_Fork
 .ent syscall_wrapper_Fork
syscall_wrapper_Fork:
 addiu $2,$0,9
 syscall
 j $31
 .end syscall_wrapper_Fork

 .globl syscall_wrapper_Yield
 .ent syscall_wrapper_Yield
syscall_wrapper_Yield:
 addiu $2,$0,10
 syscall
 j $31
 .end syscall_wrapper_Yield

 .globl syscall_wrapper_PrintInt
 .ent syscall_wrapper_PrintInt
syscall_wrapper_PrintInt:
        addiu $2,$0,11
        syscall
        j $31
 .end syscall_wrapper_PrintInt

 .globl syscall_wrapper_PrintChar
 .ent syscall_wrapper_PrintChar
syscall_wrapper_PrintChar:
        addiu $2,$0,12
        syscall
        j $31
 .end syscall_wrapper_PrintChar

 .globl syscall_wrapper_PrintString
 .ent syscall_wrapper_PrintString
syscall_wrapper_PrintString:
        addiu $2,$0,13
        syscall
        j $31
 .end syscall_wrapper_PrintString

 .globl syscall_wrapper_GetReg
 .ent system_callGetReg
syscall_wrapper_GetReg:
 addiu $2,$0,14
 syscall
 j $31
 .end syscall_wrapper_GetReg

 .globl syscall_wrapper_GetPA
 .ent syscall_wrapper_GetPA
syscall_wrapper_GetPA:
 addiu $2,$0,15
 syscall
 j $31
 .end syscall_wrapper_GetPA

 .globl syscall_wrapper_GetPID
 .ent syscall_wrapper_GetPID
syscall_wrapper_GetPID:
 addiu $2,$0,16
 syscall
 j $31
 .end syscall_wrapper_GetPID

 .globl syscall_wrapper_GetPPID
 .ent syscall_wrapper_GetPPID
syscall_wrapper_GetPPID:
 addiu $2,$0,17
 syscall
 j $31
 .end syscall_wrapper_GetPPID

 .globl syscall_wrapper_Sleep
 .ent syscall_wrapper_Sleep
syscall_wrapper_Sleep:
 addiu $2,$0,18
 syscall
 j $31
 .end syscall_wrapper_Sleep

 .globl syscall_wrapper_GetTime
 .ent syscall_wrapper_GetTime
syscall_wrapper_GetTime:
 addiu $2,$0,19
 syscall
 j $31
 .end syscall_wrapper_GetTime

 .globl syscall_wrapper_GetNumInstr
 .ent syscall_wrapper_GetNumInstr
syscall_wrapper_GetNumInstr:
 addiu $2,$0,50
 syscall
 j $31
 .end syscall_wrapper_GetNumInstr

 .globl syscall_wrapper_PrintIntHex
 .ent syscall_wrapper_PrintIntHex
syscall_wrapper_PrintIntHex:
 addiu $2,$0,20
 syscall
 j $31
 .end syscall_wrapper_PrintIntHex


        .globl __main
        .ent __main
__main:
        j $31
        .end __main
