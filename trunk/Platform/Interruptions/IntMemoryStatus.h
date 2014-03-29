#ifndef INTMEMORYSTATUS_H
#define INTMEMORYSTATUS_H

#include <cstdio>
#include <windows.h>

namespace DoZerg{
	template<class __Res>class IntMemoryStatus
	{
	public:
		~IntMemoryStatus(){}
		explicit IntMemoryStatus(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			switch(res.R1){
				case 0:{
					res.R2 = res.BE + 1;
					res.R3 = res.HE - res.HS + 1;
					res.R4 = res.TOP - res.SS + 1;
					res.R5 = res.TOP + 1;
					break;}
				case 1:{
					MEMORYSTATUS ms;
					GlobalMemoryStatus(&ms);
					res.R2 = ms.dwTotalPhys;
					res.R3 = ms.dwTotalPhys - ms.dwAvailPhys;
					res.R4 = ms.dwTotalPageFile;
					res.R5 = ms.dwTotalPageFile - ms.dwAvailPageFile;
					break;}
				default:{
					res.errFile<<BeginError
						<<"IntMemoryStatus: INT 6, R1="<<res.R1<<" function not handled"
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