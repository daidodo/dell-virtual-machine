#ifndef LOAD_H
#define LOAD_H

#include "InsBase.h"

namespace DoZerg{
	//HALT	结束程序	I
	template<class __Res,class __Pass>struct HALT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tHALT\tat address ",cur);
			return res.Reformat_I(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tHALT\tat address ",res.IP);
			++res.IP;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 1;
			return line.Size() == 1;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_I(line,ins);
		}
	};
	//load byte immediate	LBI $R,byte	把一个单字节数存入$R	IRb
	template<class __Res,class __Pass>struct LBI
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLBI\tat address ",cur);
			return res.Reformat_IRb(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLBI\tat address ",res.IP);
			res.OpRi(1) = res.RightType<typename __Res::S1>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRb(line,ins);
		}
	};
	//load word immediate	LWI $R,word	把一个2字节数存入$R	IRw
	template<class __Res,class __Pass>struct LWI
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLWI\tat address ",cur);
			return res.Reformat_IRi<typename __Res::S2>(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLWI\tat address ",res.IP);
			res.OpRi(1) = res.RightType<typename __Res::S2>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRw(line,ins);
		}
	};
	//load dword immediate	LDI $R,dword	把一个4字节数存入$R	IRd
	template<class __Res,class __Pass>struct LDI
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLDI\tat address ",cur);
			return res.Reformat_IRi<typename __Res::S4>(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLDI\tat address ",res.IP);
			res.OpRi(1) = res.RightType<typename __Res::S4>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			res.IP += 6;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 6;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRd(line,ins);
		}
	};
	//load qword immediate	LQW $R,qword	把一个8字节数存入$R	IRq
	template<class __Res,class __Pass>struct LQI
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLQI\tat address ",cur);
			return res.Reformat_IRi<typename __Res::S8>(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLQI\tat address ",res.IP);
			res.OpRi(1) = res.RightType<typename __Res::S8>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			res.IP += 10;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 10;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRq(line,ins);
		}
	};
	//load float immediate	LF1I $R,float	把一个浮点数存入$R	IFf
	template<class __Res,class __Pass>struct LF1I
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLF1I\tat address ",cur);
			return res.Reformat_IFf(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLF1I\tat address ",res.IP);
			res.OpRf(1) = res.RightType<typename __Res::F4>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			res.IP += 6;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 6;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IFf(line,ins);
		}
	};
	//load double immediate	LF2W $R,double	把一个双精度浮点数存入$R	IDf
	template<class __Res,class __Pass>struct LF2I
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLF2I\tat address ",cur);
			return res.Reformat_IDf(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLF2I\tat address ",res.IP);
			res.OpRd(1) = res.RightType<typename __Res::F8>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			res.IP += 10;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 10;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IDf(line,ins);
		}
	};
	//load address directly	LAD $R,address	把一个地址存入$R（直接寻址）	IRq
	template<class __Res,class __Pass>struct LAD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLAD\tat address ",cur);
			return res.Reformat_IRa(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLAD\tat address ",res.IP);
			res.OpRi(1) = res.RightType<typename __Res::U8>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::U8(res.OpRi(1)));
			res.IP += 10;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 10;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_LAD(line,ins);
		}
	};
	//load address indirectly	LAI $R1,$R2,qword	把qword和$R2的和存入$R1（间接寻址）
	template<class __Res,class __Pass>struct LAI
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLAI\tat address ",cur);
			return res.Reformat_IRRo(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLAI\tat address ",res.IP);
			res.OpRi(1) = res.OpRi(2) + res.RightType<typename __Res::S8>(3);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::U8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),typename __Res::S8(res.OpRi(2)));
			if(!res.CheckAddress(res.OpRi(1),res.IP + 1))
				return false;
			res.IP += 11;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 11;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_LAI(line,ins);
		}
	};
	//load byte	LB $R1,$R2	以$R2里的值为地址把一个字节数存入$R1	IRR
	template<class __Res,class __Pass>struct LB
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLB\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLB\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.OpRi(1) = res.RightRiType<typename __Res::S1>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),res.OpRi(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRR(line,ins);
		}
	};
	//load word	LW $R1,$R2	以$R2里的值为地址把一个2字节数存入$R1
	template<class __Res,class __Pass>struct LW
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLW\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLW\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.OpRi(1) = res.RightRiType<typename __Res::S2>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),res.OpRi(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRR(line,ins);
		}
	};
	//load dword	LD $R1,$R2	以$R2里的值为地址把一个4字节数存入$R1
	template<class __Res,class __Pass>struct LD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLD\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLD\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.OpRi(1) = res.RightRiType<typename __Res::S4>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),res.OpRi(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRR(line,ins);
		}
	};
	//load qword	LQ $R1,$R2	以$R2里的值为地址把一个8字节数存入$R1
	template<class __Res,class __Pass>struct LQ
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLQ\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLQ\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.OpRi(1) = res.RightRiType<typename __Res::S8>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),res.OpRi(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRR(line,ins);
		}
	};
	//load float	LF1 $F,$R	以$R里的值为地址把一个浮点数存入$F		IFR
	template<class __Res,class __Pass>struct LF1
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLF1\tat address ",cur);
			return res.Reformat_IFR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLF1\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.OpRf(1) = res.RightRiType<typename __Res::F4>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),res.OpRi(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IFR(line,ins);
		}
	};
	//load double	LF2 $D,$R	以$R里的值为地址把一个双精度浮点数存入$D
	template<class __Res,class __Pass>struct LF2
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tLF2\tat address ",cur);
			return res.Reformat_IDR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tLF2\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.OpRd(1) = res.RightRiType<typename __Res::F8>(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),res.OpRi(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IDR(line,ins);
		}
	};
}//namespace DoZerg

#endif