#	ipc2.asm
.PB main
	.PV hostname -32
	.PV ipaddr -64
	
	#allocate local storage on statck
	PUSHQ FP
	MOV FP,SP
	LQI R7,64
	SUB SP,SP,R7
	
	#get the local host name and print it
	LAI R2,FP,hostname
	LBI R3,32
	LBI R1,6
	INT 10
	
	LBI R1,18
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#get the IP address of local host
	LAI R2,FP,hostname
	LAI R3,FP,ipaddr
	LBI R1,7
	INT 10
	
	MOV R2,R3
	LBI R1,18
	INT 0
	
	LBI R2,10
	LBI R1,16
	INT 0
	LBI R2,13
	LBI R1,16
	INT 0
	
	#get the host name associated with this IP
	LAI R3,FP,hostname
	LAI R2,FP,ipaddr
	LBI R1,8
	INT 10
	
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