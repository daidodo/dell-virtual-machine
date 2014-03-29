#	native.asm
.PB main
	.PV filename -20
	.PV input -128
	.PV output -512
	
	#allocate local storage
	PUSHQ FP
	MOV FP,SP
	LQI R7,512
	SUB SP,SP,R7
	
	#populate name of shared lib = C:\Dllexample.dll
	LAI R10,FP,filename
	MOV R14,R10
	LBI R2,1
	
	LBI R1,'C'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,':'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'\'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'D'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'l'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'l'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'e'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'x'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'a'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'m'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'p'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'l'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'e'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'.'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'d'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'l'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'l'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,0
	SB R1,R10
	
	#print out filename
	MOV R2,R14
	LBI R1,18
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#populate XML input = <IN>proc1</IN>
	LAI R10,FP,input
	MOV R11,R10
	LBI R2,1
	
	LBI R1,'<'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'I'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'N'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'>'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'p'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'r'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'o'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'c'
	SB R1,R10
	ADD R10,R10,R2
	
#change proc1 or proc2 here
	LBI R1,'1'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'<'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'/'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'I'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'N'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'>'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,0
	SB R1,R10

	#print out input
	MOV R2,R11
	LBI R1,18
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#load the library, put handle in R20
	MOV R2,R14
	LBI R1,0
	INT 9
	MOV R20,R3
	
	MOV R2,R4
	LBI R1,20
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#execute native call
	LAI R4,FP,output
	MOV R3,R11
	MOV R2,R20
	LBI R1,1
	INT 9
	
	LAI R2,FP,output
	LBI R1,18
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#free library
	MOV R2,R20
	LBI R1,2
	INT 9
	
	MOV R2,R3
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