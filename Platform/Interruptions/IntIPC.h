#ifndef INTIPC_H
#define INTIPC_H

//load "ws2_32.lib" for WSAStartup() and WSACleanup()
#pragma comment(lib, "ws2_32.lib")

#include <winsock.h>
#include <memory.h>
#include <cstring>

namespace DoZerg{
	template<class __Res>class IntIPC
	{
		typedef typename __Res::U8 U8;
	public:
		~IntIPC(){}
		explicit IntIPC(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			switch(res.R1){
				case 0:{
					res.R3 = 0;
					res.R4 = 1;
					LPCTSTR name = res.RAMPosition<char>(res.R2);
					HANDLE hmutex = CreateMutex(0,TRUE,name);
					if(hmutex == 0){
						if(GetLastError() == ERROR_ALREADY_EXISTS){
							hmutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,name);
							if(hmutex != 0){
								WaitForSingleObject(hmutex,INFINITE);
								res.R3 = U8(hmutex);
								res.R4 = 0;
							}
						}
					}else{
						WaitForSingleObject(hmutex,INFINITE);
						res.R3 = U8(hmutex);
						res.R4 = 0;
					}break;}
				case 1:{
					res.R3 = 0;
					if(ReleaseMutex(HANDLE(res.R2)) == 0 && CloseHandle(HANDLE(res.R2)) == 0)
						res.R3 = 1;
					break;}
				case 2:{
					res.R4 = 0;
					res.R5 = 1;
					if(InitSockets()){
						sockaddr_in address;
						address.sin_family = AF_INET;
						address.sin_port = htons(unsigned short(res.R3));
						address.sin_addr.s_addr = inet_addr(res.RAMPosition<char>(res.R2));
						SOCKET client = socket(AF_INET,SOCK_STREAM,0);
						//*
						if(client != INVALID_SOCKET && connect(client,reinterpret_cast<SOCKADDR *>(&address),sizeof(address)) != SOCKET_ERROR){
							res.R4 = U8(client);
							res.R5 = 0;
						/*/
						if(client != INVALID_SOCKET){
							int ret = connect(client,reinterpret_cast<SOCKADDR *>(&address),sizeof(address));
							if(ret != SOCKET_ERROR){
								res.R4 = U8(client);
								res.R5 = 0;
							}
						//*/
						}
					}break;}
				case 3:{
					res.R3 = 1;
					if(shutdown(SOCKET(res.R2),0x2) != SOCKET_ERROR && closesocket(SOCKET(res.R2)) != SOCKET_ERROR){
						res.R3 = 0;
						WSACleanup();
					}break;}
				case 4:{
					res.R5 = 0;
					char * buffer = res.RAMPosition<char>(res.R3);
					for(int i = 0,count = int(res.R4);i < count;){
						int ret = send(SOCKET(res.R2),buffer + i,count - i,0);
						if(ret = SOCKET_ERROR){
							res.R5 = 1;
							break;
						}
						i += ret;
					}break;}
				case 5:{
					res.R5 = 0;
					char * buffer = res.RAMPosition<char>(res.R3);
					for(int i = 0,count = int(res.R4);i < count;){
						int ret = recv(SOCKET(res.R2),buffer + i,count - i,0);
						if(ret = SOCKET_ERROR){
							res.R5 = 1;
							break;
						}
						i += ret;
					}break;}
				case 6:{
					res.R4 = (InitSockets() ? (gethostname(res.RAMPosition<char>(res.R2),int(res.R3)) != 0 ? 1 : 0) : 1);
					WSACleanup();
					break;}
				case 7:{
					res.R4 = 1;
					if(InitSockets()){
						HOSTENT * hp = gethostbyname(res.RAMPosition<char>(res.R2));
						if(hp != 0){
							if(hp->h_addr_list[0] != 0){
								in_addr addr;
								memcpy(&addr,hp->h_addr_list[0],sizeof(in_addr));
								std::strcpy(res.RAMPosition<char>(res.R3),inet_ntoa(addr));
							}
							for(int i = 1;hp->h_addr_list[i] != 0;++i){
								in_addr addr;
								memcpy(&addr,hp->h_addr_list[i],sizeof(in_addr));
								std::strcat(res.RAMPosition<char>(res.R3),":");
								std::strcat(res.RAMPosition<char>(res.R3),inet_ntoa(addr));
							}
							res.R4 = 0;
						}
						WSACleanup();
					}break;}
				case 8:{
					res.R4 = 1;
					if(InitSockets()){
						in_addr hostaddr;
						hostaddr.s_addr = inet_addr(res.RAMPosition<char>(res.R2));
						HOSTENT * hp = gethostbyaddr(reinterpret_cast<char *>(&hostaddr),sizeof(in_addr),AF_INET);
						if(hp != 0){
							std::strcpy(res.RAMPosition<char>(res.R3),hp->h_name);
							res.R4 = 0;
						}
						WSACleanup();
					}break;}
				case 9:{
					res.R4 = 0;
					res.R5 = 1;
					if(InitSockets()){
						sockaddr_in address;
						address.sin_family = AF_INET;
						address.sin_port = htons(unsigned short(res.R3));
						address.sin_addr.s_addr = inet_addr(res.RAMPosition<char>(res.R2));
						SOCKET server = socket(AF_INET,SOCK_STREAM,0);
						if(server != INVALID_SOCKET &&
							bind(server,reinterpret_cast<SOCKADDR *>(&address),sizeof(address)) != SOCKET_ERROR &&
							listen(server,SOMAXCONN) != SOCKET_ERROR){
							res.R4 = U8(server);
							res.R5 = 0;
						}
					}break;}
				case 10:{
					sockaddr_in client;
					int length;
					SOCKET connection = accept(SOCKET(res.R2),reinterpret_cast<SOCKADDR *>(&client),&length);
					if(connection == INVALID_SOCKET)
						res.R5 = 1;
					else{
						res.R5 = 0;
						std::strcpy(res.RAMPosition<char>(res.R3),inet_ntoa(client.sin_addr));
						res.R4 = U8(connection);
					}break;}
				case 11:{
					res.R3 = ((shutdown(SOCKET(res.R2),0x2) == SOCKET_ERROR || closesocket(SOCKET(res.R2)) == SOCKET_ERROR) ? 1 : 0);
					break;}
				case 12:{
					if(closesocket(SOCKET(res.R2)) == SOCKET_ERROR)
						res.R3 = 1;
					else{
						res.R3 = 0;
						WSACleanup();
					}break;}
				default:{
					res.errFile<<BeginError
						<<"IntIPC: INT 10, R1="<<res.R1<<" function not handled"
						<<EndError;
					return false;
				}
			}
			return true;
		}
		bool InitSockets(){
			WORD wVersionReq = MAKEWORD(2,2);
			WSADATA wsaData;
			if(WSAStartup(wVersionReq,&wsaData) != 0)
				return false;
			else if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2){
				WSACleanup();
				return false;
			}
			return true;
		}
	private:
		bool suc_;
	};
}//namespace DoZerg

#endif