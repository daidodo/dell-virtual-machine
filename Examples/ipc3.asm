#	ipc3.asm	client
.PB main
	.PV message -32
	.PV ipaddr -64
	
	#allocate local storage on statck
	PUSHQ FP
	MOV FP,SP
	LQI R7,64
	SUB SP,SP,R7
	
	#set the ipaddress
	LAI R10,FP,ipaddr
	MOV R14,R10
	LBI R2,1
	
	LBI R1,'1'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'2'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'7'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'.'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'0'
	SB R1,R10
	ADD R10,R10,R2
	
#	LBI R1,'6'
#	SB R1,R10
#	ADD R10,R10,R2
	
#	LBI R1,'8'
#	SB R1,R10
#	ADD R10,R10,R2
	
	LBI R1,'.'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'0'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'.'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'1'
	SB R1,R10
	ADD R10,R10,R2
	
#	LBI R1,'4'
#	SB R1,R10
#	ADD R10,R10,R2
	
#	LBI R1,'4'
#	SB R1,R10
#	ADD R10,R10,R2
	
	LBI R1,0
	SB R1,R10
	
	#print out ipaddress
	MOV R2,R14
	LBI R1,18
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#set the message  = hello!
	LAI R10,FP,message
	MOV R15,R10
	LBI R2,1
	
	LBI R1,'h'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'e'
	SB R1,R10
	ADD R10,R10,R2
	
	LBI R1,'l'
	SB R1,R10
	ADD R10,R10,R2

	LBI R1,'l'
	SB R1,R10
	ADD R10,R10,R2

	LBI R1,'o'
	SB R1,R10
	ADD R10,R10,R2

	LBI R1,'!'
	SB R1,R10
	ADD R10,R10,R2

	LBI R1,0
	SB R1,R10
	
	#create the client socket
	MOV R2,R14
	LQI R3,30000
	LBI R1,2
	INT 10
	MOV R16,R4
	
	MOV R2,R5
	LBI R1,20
	INT 0

	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	LBI R1,22
	INT 0

	#send some data
	MOV R2,R16
	MOV R3,R15
	LBI R4,7
	LBI R1,4
	INT 10
	
	MOV R2,R5
	LBI R1,20
	INT 0

	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#shut the client socket down
	MOV R2,R16
	LBI R1,3
	INT 10
	
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