#	alloc.asm
.PB main

	#init the heap
	LBI R1,3
	INT 7
	
	#print out heap
	LBI R1,2
	INT 7
	
	#allocate memory, place address in R20
	LQI R2,20
	LBI R1,0
	INT 7
	MOV R20,R3
	
	MOV R2,R20
	LBI R1,20
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#print out heap
	LBI R1,2
	INT 7
	
	#allocate memory, place address in R21
	LQI R2,11
	LBI R1,0
	INT 7
	MOV R21,R3
	
	MOV R2,R21
	LBI R1,20
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#print out heap
	LBI R1,2
	INT 7
	
	#allocate memory, place address in R22
	LQI R2,1
	LBI R1,0
	INT 7
	MOV R22,R3
	
	MOV R2,R22
	LBI R1,20
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#print out heap
	LBI R1,2
	INT 7
	
	#dealloc memory
	MOV R2,R21
	LBI R1,1
	INT 7
	
	#print out heap
	LBI R1,2
	INT 7
	
	#dealloc memory
	MOV R2,R22
	LBI R1,1
	INT 7
	
	#print out heap
	LBI R1,2
	INT 7
	
	#dealloc memory
	MOV R2,R20
	LBI R1,1
	INT 7
	
	#print out heap
	LBI R1,2
	INT 7
	
	HALT
.PE