#ifndef INTPROCESSMANAGEMENT_H
#define INTPROCESSMANAGEMENT_H

#include <process.h>
#include "Helpers\Win32Pipe.h"

namespace DoZerg{
	template<class __Res>class IntProcessManagement
	{
		typedef typename __Res::U8 U8;
	public:
		~IntProcessManagement(){}
		explicit IntProcessManagement(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			switch(res.R1){
				case 0:{
					STARTUPINFO st_info;
					PROCESS_INFORMATION pro_info;
					GetStartupInfo(&st_info);
					if(CreateProcess(
						NULL,
						res.RAMPosition<CHAR>(res.R2),
						NULL,NULL,FALSE,0,NULL,NULL,
						&st_info,
						&pro_info)
					== FALSE){
						res.R4 = 0;
						res.R5 = 1;
					}else{
						res.R4 = U8(pro_info.hProcess);
						res.R5 = 0;
					}break;}
				case 1:{
					BOOL ret;
					DWORD code = STILL_ACTIVE;
					do{
						ret = GetExitCodeProcess(HANDLE(res.R2),&code);
					}while(code == STILL_ACTIVE);
					if(ret == FALSE){
						res.R3 = 0;
						res.R4 = 1;
					}else{
						res.R3 = code;
						res.R4 = 0;
					}break;}
				case 2:{
					int ret = system(res.RAMPosition<char>(res.R2));
					if(ret == -1){
						res.R3 = 0;
						res.R4 = 1;
					}else{
						res.R3 = ret;
						res.R4 = 0;
					}break;}
				case 3:{
					Win32Pipe pipe(res.RAMPosition<char>(res.R2),"rb");
					if(pipe.IsOpen()){
						res.R5 = pipe.Read(res.RAMPosition<char>(res.R3),1,size_t(res.R4));
						res.RAM[res.R3 + res.R5] = '\0';
						res.R6 = 0;
					}else{
						res.R5 = 0;
						res.R6 = 1;
					}break;}
				default:{
					res.errFile<<BeginError
						<<"IntProcessManagement: INT 2, R1="<<res.R1<<" function not handled"
						<<EndError;
					return false;
				}
			}
			return true;
		}
	private:
		bool suc_;
	};
}//namespace DoZerg

#endif