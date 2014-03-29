#ifndef INTNATIVECALL_H
#define INTNATIVECALL_H

#include <windows.h>

namespace DoZerg{
	template<class __Res>class IntNativeCall
	{
		typedef typename __Res::U8 U8;
	public:
		~IntNativeCall(){}
		explicit IntNativeCall(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			typedef void (*__GateWayFn)(char *,char *);
			switch(res.R1){
				case 0:{
					HINSTANCE handle = LoadLibrary(res.RAMPosition<char>(res.R2));
					res.R3 = U8(handle);
					res.R4 = (handle == 0 ? 1 : 0);
					break;}
				case 1:{
					__GateWayFn gateway = reinterpret_cast<__GateWayFn>(GetProcAddress(HINSTANCE(res.R2),"gateway"));
					if(gateway != 0){
						res.R5 = 0;
						gateway(res.RAMPosition<char>(res.R3),res.RAMPosition<char>(res.R4));
					}else
						res.R5 = 1;
					break;}
				case 2:{
					res.R3 = (FreeLibrary(HINSTANCE(res.R2)) == 0 ? 1 : 0);
					break;}
				default:{
					res.errFile<<BeginError
						<<"IntNativeCall: INT 9, R1="<<res.R1<<" function not handled"
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