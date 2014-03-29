#ifndef JUMP_H
#define JUMP_H

#include "InsBase.h"

namespace DoZerg{
	//JMP $R	跳转到$R给出的地址	IR
	template<class __Res,class __Pass>struct JMP
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tJMP\tat address ",cur);
			return res.Reformat_IR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tJMP\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(1),res.IP + 1))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),res.OpRi(1));
			res.IP = res.OpRi(1);
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 2;
			return line.Size() == 2;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IR(line,ins);
		}
	};
	//jump if equal	JE $R1,$R2,$R3	如果$R1等于$R2，跳转到$R3给出的地址	IRRR
	template<class __Res,class __Pass>struct JE
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tJE\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tJE\tat address ",res.IP);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),res.OpRi(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),res.OpRi(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(3),res.OpRi(3));
			if(res.OpRi(1) == res.OpRi(2)){
				if(!res.CheckAddress(res.OpRi(3),res.IP + 3))
					return false;
				res.IP = res.OpRi(3);
			}else
				res.IP += 4;
			VM_DBG_INS_P8("\t\tIP = ",res.IP);
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRRR(line,ins);
		}
	};
	//jump if not equal	JNE $R1,$R2,$R3	如果$R1不等于$R2，跳转到$R3给出的地址
	template<class __Res,class __Pass>struct JNE
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tJNE\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tJNE\tat address ",res.IP);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),res.OpRi(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),res.OpRi(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(3),res.OpRi(3));
			if(res.OpRi(1) != res.OpRi(2)){
				if(!res.CheckAddress(res.OpRi(3),res.IP + 3))
					return false;
				res.IP = res.OpRi(3);
			}else
				res.IP += 4;
			VM_DBG_INS_P8("\t\tIP = ",res.IP);
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRRR(line,ins);
		}
	};
	//set if less than	SLT $R1,$R2,$R3	如果$R2小于$R3，把$R1设置为1；否则，把$R1设置为0
	template<class __Res,class __Pass>struct SLT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSLT\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSLT\tat address ",res.IP);
			res.OpRi(1) = (res.OpRi(2) < res.OpRi(3) ? 1 : 0);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),res.OpRi(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),res.OpRi(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(3),res.OpRi(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRRR(line,ins);
		}
	};
	//INT byte	执行中断，中断号由byte指定	Ib
	template<class __Res,class __Pass>struct INT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tINT\tat address ",cur);
			return res.Reformat_Ib(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tINT\tat address ",res.IP);
			if(!res.HandleInterruption(res.RightType<typename __Res::U1>(1)))
				return false;
			res.IP += 2;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 2;
			return line.Size() == 2;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_Ib(line,ins);
		}
	};
	//DI	禁止中断处理
	template<class __Res,class __Pass>struct DI
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tDI\tat address ",cur);
			return res.Reformat_I(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tDI\tat address ",res.IP);
			res.DisableInterrupt();
			++res.IP;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			++bytePos;
			return line.Size() == 1;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_I(line,ins);
		}
	};
	//EI	启用中断处理
	template<class __Res,class __Pass>struct EI
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tEI\tat address ",cur);
			return res.Reformat_I(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tEI\tat address ",res.IP);
			res.EnableInterrupt();
			++res.IP;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			++bytePos;
			return line.Size() == 1;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_I(line,ins);
		}
	};
	//NOP	空操作
	template<class __Res,class __Pass>struct NOP
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tNOP\tat address ",cur);
			return res.Reformat_I(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tNOP\tat address ",res.IP);
			++res.IP;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			++bytePos;
			return line.Size() == 1;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_I(line,ins);
		}
	};
}//namespace DoZerg

#endif