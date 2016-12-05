# DEllVM
Assembly language compiler and virtual machine

* Create a new assembly language, and a new executable file format;
* Compile the assembly language sources, and build an executable file;
* The executable runs in the virtual machine environment;
* Using XML file to record error information;
* Implement software interruptions such as memory allocation and management, file IO, sockets and networks, mathematical functions and float point operations, process management, date and time functions, etc;

This is an example of the new assembly language (you can find it in Examples/):
```
#ipc1.asm

.PB main .PV buffer -40 .PV mutex -64

#allocate local storage
PUSHQ FP
MOV FP,SP
LQI R7,64
SUB SP,SP,R7

#set name of mutex and store address in R14
LAI R10,FP,mutex
MOV R14,R10
LBI R2,1

LBI R1,'m'
SB R1,R10
ADD R10,R10,R2

LBI R1,'i'
SB R1,R10
ADD R10,R10,R2

LBI R1,'n'
SB R1,R10
ADD R10,R10,R2

LBI R1,'e'
SB R1,R10
ADD R10,R10,R2

LBI R1,0
SB R1,R10

#obtain a mutex
MOV R2,R14
LBI R1,0
INT 10
MOV R15,R3

#print string to signal that mutex obtained
MOV R2,R14
LBI R1,18
INT 0

LBI R2,10
LBI R1,16
INT 0
LBI R2,13
LBI R1,16
INT 0

#read and print string
LAI R2,FP,buffer
LBI R1,22
INT 0

LBI R1,18
INT 0

LBI R2,10
LBI R1,16
INT 0
LBI R2,13
LBI R1,16
INT 0

#release mutex
MOV R2,R15
LBI R1,1
INT 10

#reclaim local storage
MOV SP,FP
POPQ FP

HALT
.PE
```
