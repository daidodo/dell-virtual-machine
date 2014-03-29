#ifndef INTCOMMANDLINE_H
#define INTCOMMANDLINE_H

#include <cstring>

namespace DoZerg{
	template<class __Res>class IntCommandLine
	{
	public:
		~IntCommandLine(){}
		explicit IntCommandLine(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			switch(res.R1){
				case 0:{
					res.R2 = res.ArgumentCount();
					break;}
				case 1:{
					if(res.R2 >= res.ArgumentCount()){
						res.R4 = 1;
						res.R5 = 0;
					}else{
						std::strcpy(res.RAMPosition<char>(res.R3),res.GetArgument(res.R2));
						res.R4 = 0;
						res.R5 = res.ArgumentSize(res.R2);
					}break;}
				default:{
					res.errFile<<BeginError
						<<"IntCommandLine: INT 5, R1="<<res.R1<<" function not handled"
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