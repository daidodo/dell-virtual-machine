#ifndef INTTIMEDATECALL_H
#define INTTIMEDATECALL_H

#include <ctime>
#include <cstring>

namespace DoZerg{
	template<class __Res>class IntTimeDateCall
	{
		typedef typename __Res::U8 U8;
	public:
		~IntTimeDateCall(){}
		explicit IntTimeDateCall(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			typedef std::time_t __Time;
			typedef std::tm __Tm;
			switch(res.R1){
				case 0:{
					res.R2 = std::time(0);
					break;}
				case 1:{
					__Tm * ptm = std::gmtime(reinterpret_cast<const __Time *>(&res.R2));
					res.R3 = ptm->tm_sec;
					res.R4 = ptm->tm_min;
					res.R5 = ptm->tm_hour;
					res.R6 = ptm->tm_mday;
					res.R7 = ptm->tm_mon;
					res.R8 = ptm->tm_year;
					break;}
				case 2:{
					__Tm * ptm = std::localtime(reinterpret_cast<const __Time *>(&res.R2));
					res.R3 = ptm->tm_sec;
					res.R4 = ptm->tm_min;
					res.R5 = ptm->tm_hour;
					res.R6 = ptm->tm_mday;
					res.R7 = ptm->tm_mon;
					res.R8 = ptm->tm_year;
					break;}
				case 3:{
					__Tm tm;
					tm.tm_sec = int(res.R2);
					tm.tm_min = int(res.R3);
					tm.tm_hour = int(res.R4);
					tm.tm_mday = int(res.R5);
					tm.tm_mon = int(res.R6);
					tm.tm_year = int(res.R7);
					res.R8 = U8(std::mktime(&tm));
					break;}
				case 4:{
					char str[26];
					std::strcpy(str,std::ctime(reinterpret_cast<const __Time *>(&res.R2)));
					std::strcpy(res.RAMPosition<char>(res.R3),str);
					res.RAM[res.R3 + 24] = 0;
					break;}
				case 5:{
					res.R4 = U8(std::difftime(__Time(res.R3),__Time(res.R2)));
					break;}
				case 6:{
					res.R2 = GetTickCount();
					break;}
				default:{
					res.errFile<<BeginError
						<<"IntTimeDateCall: INT 3, R1="<<res.R1<<" function not handled"
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