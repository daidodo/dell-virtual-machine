#	ipc4.asm	server
.PB main
	.PV buffer -32
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
	
	#open a server socket
	MOV R2,R14
	LQI R3,30000
	LBI R1,9
	INT 10
	MOV R15,R4
	
	#accept a connection
	MOV R2,R15
	LAI R3,FP,buffer
	LBI R1,10
	INT 10
	MOV R16,R4
	
	LBI R1,22
	INT 0

	#print client address
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
	
	#receive data from client (R1 = 5)
	MOV R2,R16
	LAI R3,FP,buffer
	LBI R4,7
	LBI R1,5
	INT 10
	
	LBI R1,22
	INT 0

	#print data
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

	#close connection socket
	MOV R2,R16
	LBI R1,11
	INT 10
	
	#close server socket
	MOV R2,R15
	LBI R1,12
	INT 10
	
	#reclaim local storage
	MOV SP,FP
	POPQ FP
	
	HALT
.PE