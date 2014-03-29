#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream>
#include <string>

namespace DoZerg{
	class Debugger
	{
	public:
		~Debugger(){}
		Debugger(){}
		template<class __Res>
			bool ReadCommand(__Res & res){
				std::cout<<"DEllVM Debugger>";
				std::string cmd;
				std::cin>>cmd;
				if(cmd == "?"){
					std::cout<<"? command\n";
					res.DebugModeOn = 0;
				}else{
				}
				return true;
			}
	private:
	};
}//namespace DoZerg

#endif