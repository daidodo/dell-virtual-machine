#	filemgmt.asm
.PB main
	.PV filename1 -16
	.PV filename2 -32
	.PV dirname -40
	.PV buffer -512

	#allocate local storage on stack
	PUSHQ FP
	MOV FP,SP
	LQI R7,512
	SUB SP,SP,R7
	
	#populate local dirName = C:\_a
	LAI R10,FP,dirname
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

	LBI R1,'_'
	SB R1,R10
	ADD R10,R10,R2

	LBI R1,'a'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,0
	SB R1,R10
	
	#populate local filename1 = C:\_a\t.txt
	LAI R10,FP,filename1
	MOV R11,R10
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

	LBI R1,'_'
	SB R1,R10
	ADD R10,R10,R2

	LBI R1,'a'
	SB R1,R10
	ADD R10,R10,R2

	LBI R1,'\'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'t'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'.'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'t'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'x'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'t'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,0
	SB R1,R10
	
	#populate local filename2 = C:\_a\w.txt
	LAI R10,FP,filename2
	MOV R12,R10
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

	LBI R1,'_'
	SB R1,R10
	ADD R10,R10,R2

	LBI R1,'a'
	SB R1,R10
	ADD R10,R10,R2

	LBI R1,'\'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'w'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'.'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'t'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'x'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'t'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,0
	SB R1,R10
	
	#print dirname
	MOV R2,R14
	LBI R1,18
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#print filename1
	MOV R2,R11
	LBI R1,18
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#print filename2
	MOV R2,R12
	LBI R1,18
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#check to see if filename1 exists and print return code
	LBI R1,0
	MOV R2,R11
	INT 1
	
	MOV R2,R3
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0
	
	#check to see if filename1 is a directory
	MOV R2,R11
	LBI R1,1
	INT 1
	
	MOV R2,R3
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#check to see if can read filename1
	MOV R2,R11
	LBI R1,2
	INT 1
	
	MOV R2,R3
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#check to see if can write to filename1
	MOV R2,R11
	LBI R1,3
	INT 1
	
	MOV R2,R3
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#get size of filename1 in bytes
	MOV R2,R11
	LBI R1,4
	INT 1
	
	MOV R2,R3
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#get time of last modification of filename1
	MOV R2,R11
	LBI R1,5
	INT 1
	
	MOV R2,R3
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#delete filename1
	MOV R2,R11
	LBI R1,6
	INT 1
	
	MOV R2,R3
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#create a directory
	MOV R2,R12
	LBI R1,7
	INT 1
	
	MOV R2,R3
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#delete a directory
	MOV R2,R12
	LBI R1,8
	INT 1
		
	MOV R2,R3
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#obtain the full path of filename2
	LAI R3,FP,buffer
	MOV R13,R3
	MOV R2,R12
	LBI R1,9
	INT 1
	
	MOV R2,R13
	LBI R1,18
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0
	
	#get the maximum path length
	LBI R1,10
	INT 1
	
	LBI R1,20
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0
	
	#get the contents of C:\_a
	MOV R2,R14
	LBI R3,0
	MOV R4,R13
	LBI R1,11
#	INT 1
	
	MOV R2,R13
	LBI R1,18
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0
	
	#print filename1
	MOV R2,R11
	LBI R1,18
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#print filename2
	MOV R2,R12
	LBI R1,18
	INT 0
	
	LQI R2,10
	LQI R1,16
	INT 0
	LQI R2,13
	LQI R1,16
	INT 0

	#rename w.txt to t.txt
	MOV R2,R12
	MOV R3,R11
	LBI R1,12
	INT 1
	
	#reclaim local storage
	MOV SP,FP
	POPQ FP
	
	HALT
.PE