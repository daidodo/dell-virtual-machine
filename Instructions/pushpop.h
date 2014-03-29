#ifndef PUSHPOP_H
#define PUSHPOP_H

#include "InsBase.h"

namespace DoZerg{
	//PUSHB $R	把$R里的单字节数压入堆栈	IR
	template<class __Res,class __Pass>struct PUSHB
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPUSHB\tat address ",cur);
			return res.Reformat_IR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPUSHB\tat address ",res.IP);
			if(!res.PushStack<typename __Res::S1>(res.OpRi(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",typename __Res::S8(res.TopStack<typename __Res::S1>()));
			res.IP += 2;
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
	//PUSHW $R	把$R里的4字节数压入堆栈	IR
	template<class __Res,class __Pass>struct PUSHW
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPUSHW\tat address ",cur);
			return res.Reformat_IR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPUSHW\tat address ",res.IP);
			if(!res.PushStack<typename __Res::S2>(res.OpRi(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",typename __Res::S8(res.TopStack<typename __Res::S2>()));
			res.IP += 2;
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
	//PUSHD $R	把$R里的2字节数压入堆栈	IR
	template<class __Res,class __Pass>struct PUSHD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPUSHD\tat address ",cur);
			return res.Reformat_IR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPUSHD\tat address ",res.IP);
			if(!res.PushStack<typename __Res::S4>(res.OpRi(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",typename __Res::S8(res.TopStack<typename __Res::S4>()));
			res.IP += 2;
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
	//PUSHQ $R	把$R里的8字节数压入堆栈	IR
	template<class __Res,class __Pass>struct PUSHQ
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPUSHQ\tat address ",cur);
			return res.Reformat_IR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPUSHQ\tat address ",res.IP);
			if(!res.PushStack<typename __Res::S8>(res.OpRi(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",typename __Res::S8(res.TopStack<typename __Res::S8>()));
			res.IP += 2;
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
	//PUSHF1 $F	把$F里的浮点数压入堆栈	IF
	template<class __Res,class __Pass>struct PUSHF1
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPUSHF1\tat address ",cur);
			return res.Reformat_IF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPUSHF1\tat address ",res.IP);
			if(!res.PushStack<typename __Res::F4>(res.OpRf(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",res.TopStack<typename __Res::F4>());
			res.IP += 2;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 2;
			return line.Size() == 2;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IF(line,ins);
		}
	};
	//PUSHF2 $D	把$D里的双精度浮点数压入堆栈 ID
	template<class __Res,class __Pass>struct PUSHF2
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPUSHF2\tat address ",cur);
			return res.Reformat_ID(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPUSHF2\tat address ",res.IP);
			if(!res.PushStack<typename __Res::F8>(res.OpRd(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",res.TopStack<typename __Res::F8>());
			res.IP += 2;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 2;
			return line.Size() == 2;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_ID(line,ins);
		}
	};
	//POPB $R	从堆栈里弹出一个单字节数并放入$R	IR
	template<class __Res,class __Pass>struct POPB
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPOPB\tat address ",cur);
			return res.Reformat_IR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPOPB\tat address ",res.IP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",typename __Res::S8(res.TopStack<typename __Res::S1>()));
			if(!res.PopStack<typename __Res::S1>(res.OpRi(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			res.IP += 2;
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
	//POPW $R	从堆栈里弹出一个2字节数并放入$R	IR
	template<class __Res,class __Pass>struct POPW
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPOPW\tat address ",cur);
			return res.Reformat_IR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPOPW\tat address ",res.IP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",typename __Res::S8(res.TopStack<typename __Res::S2>()));
			if(!res.PopStack<typename __Res::S2>(res.OpRi(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			res.IP += 2;
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
	//POPD $R	从堆栈里弹出一个4字节数并放入$R
	template<class __Res,class __Pass>struct POPD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPOPD\tat address ",cur);
			return res.Reformat_IR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPOPD\tat address ",res.IP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",typename __Res::S8(res.TopStack<typename __Res::S4>()));
			if(!res.PopStack<typename __Res::S4>(res.OpRi(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			res.IP += 2;
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
	//POPQ $R	从堆栈里弹出一个8字节数并放入$R
	template<class __Res,class __Pass>struct POPQ
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPOPQ\tat address ",cur);
			return res.Reformat_IR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPOPQ\tat address ",res.IP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",typename __Res::S8(res.TopStack<typename __Res::S8>()));
			if(!res.PopStack<typename __Res::S8>(res.OpRi(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			res.IP += 2;
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
	//POPF1 $F	从堆栈里弹出一个浮点数并放入$F	IF
	template<class __Res,class __Pass>struct POPF1
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPOPF1\tat address ",cur);
			return res.Reformat_IF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPOPF1\tat address ",res.IP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",res.TopStack<typename __Res::F4>());
			if(!res.PopStack<typename __Res::F4>(res.OpRf(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			res.IP += 2;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 2;
			return line.Size() == 2;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IF(line,ins);
		}
	};
	//POPF2 $D	从堆栈里弹出一个双精度浮点数并放入$D	ID
	template<class __Res,class __Pass>struct POPF2
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tPOPF2\tat address ",cur);
			return res.Reformat_ID(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tPOPF2\tat address ",res.IP);
			VM_DBG_INS_P8("\t\tRAM[SP] = ",res.TopStack<typename __Res::F8>());
			if(!res.PopStack<typename __Res::F8>(res.OpRd(1)))
				return false;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			VM_DBG_INS_P8("\t\tSP = ",res.SP);
			res.IP += 2;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 2;
			return line.Size() == 2;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_ID(line,ins);
		}
	};
}//namespace DoZerg

#endif