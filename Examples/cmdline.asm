#	cmdline.asm
.PB main
	.PV buffer -256

	#allocate local storage on stack
	PUSHQ FP
	MOV FP,SP
	LQI R7,256
	SUB SP,SP,R7

	#get the number of arguments
	LBI R1,0
	INT 5
	MOV R20,R2
	
	LBI R1,20
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#get the first arguments
	LAI R3,FP,buffer
	LBI R2,0
	LBI R1,1
	INT 5
	
	LAI R3,FP,buffer
	MOV R2,R3
	LBI R1,18
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#get the second arguments
	LAI R3,FP,buffer
	LBI R2,1
	LBI R1,1
	INT 5
	
	LAI R3,FP,buffer
	MOV R2,R3
	LBI R1,18
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#reclaim local storage
	MOV SP,FP
	POPQ FP
	
	HALT
.PE