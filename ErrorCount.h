#ifndef FATALERROR_H
#define FATALERROR_H

#include <iostream>

namespace DoZerg{
	template<class __Platform>class ErrorCount
	{
	public:
		typedef typename __Platform::U8 U8;
		typedef typename __Platform::S8 S8;
	private:
		static int errCount;
	public:
		static int CheckError(bool print = true){
			if(print)
				std::cout<<"TOTAL "<<errCount<<" ERRORS\n";
			return errCount;
		}
		~ErrorCount(){}
		ErrorCount(bool err = true){
			if(err){
				++errCount;
				std::cout<<"ERROR: ";
			}
		}
		const ErrorCount & operator <<(U8 val) const{
			__Platform::pU8(val);
			return *this;
		}
		const ErrorCount & operator <<(S8 val) const{
			__Platform::pS8(val);
			return *this;
		}
		template<class __Type>
			const ErrorCount & operator <<(const __Type & msg) const{
				std::cout<<msg;
				return *this;
			}
	};
	template<class __Platform>
		int ErrorCount<__Platform>::errCount = 0;
}//namespace DoZerg

#endif