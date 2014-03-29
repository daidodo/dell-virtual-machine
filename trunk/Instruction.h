#ifndef INSTRUCTION_H
#define INSTRUCTION_H

namespace DoZerg{
	template<
		class __Res,
		class __Pass
	>struct Instruction
	{
		typedef bool (*__ReformatFptr)(__Res &,typename __Res::U8 &);
		typedef bool (*__HandleFptr)(__Res &);
		typedef bool (*__Pass1Fptr)(typename __Pass::U8 &,const typename __Pass::__Line &);
		typedef bool (*__Pass2Fptr)(__Pass &,const typename __Pass::__Line &,int);
		~Instruction(){}
		Instruction(__ReformatFptr fp1,__HandleFptr fp2,__Pass1Fptr fp3,__Pass2Fptr fp4)
			:Reformat(fp1)
			,Handle(fp2)
			,Pass1(fp3)
			,Pass2(fp4)
		{}
		__ReformatFptr Reformat;
		__HandleFptr Handle;
		__Pass1Fptr Pass1;
		__Pass2Fptr Pass2;
	};
}//namespace DoZerg

#endif