#	timedate.asm
.PB main
	.PV buffer -26

	#allocate local storage on stack
	PUSHQ FP
	MOV FP,SP
	LQI R7,26
	SUB SP,SP,R7

	#get the current UTC time
	LQI R1,0
	INT 4
	MOV R20,R2
	
	LBI R1,20
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#decompose into time/date components
	MOV R2,R20
	LBI R1,1
	INT 4
	
	MOV R9,R3
	MOV R10,R4
	MOV R11,R5
	MOV R12,R6
	MOV R13,R7
	MOV R14,R8
	
	MOV R2,R9
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0
	
	MOV R2,R10
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	MOV R2,R11
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	MOV R2,R12
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	MOV R2,R13
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	MOV R2,R14
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#get the eqivalent local time
	MOV R2,R20
	LBI R1,2
	INT 4
	
	MOV R9,R3
	MOV R10,R4
	MOV R11,R5
	MOV R12,R6
	MOV R13,R7
	MOV R14,R8
	
	MOV R2,R9
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0
	
	MOV R2,R10
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	MOV R2,R11
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	MOV R2,R12
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	MOV R2,R13
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	MOV R2,R14
	LBI R1,20
	INT 0
	LBI R2,32
	LBI R1,16
	INT 0

	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#create a new time
	LBI R1,3
	LBI R2,0
	LBI R3,10
	LBI R4,23
	LBI R5,1
	LBI R6,8
	LBI R7,101
	INT 4
	MOV R16,R8
	
	#get string rep of current time
	MOV R2,R20
	LAI R3,FP,buffer
	LBI R1,4
	INT 4
	
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
	
	#get string rep of new time
	MOV R2,R16
	LAI R3,FP,buffer
	LBI R1,4
	INT 4
	
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
	
	#compare previous date with today's
	MOV R2,R20
	MOV R3,R16
	LBI R1,5
	INT 4
	
	MOV R2,R4
	LBI R1,20
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