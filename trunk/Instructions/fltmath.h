#ifndef FLTMATH_H
#define FLTMATH_H

#include "InsBase.h"

namespace DoZerg{
	//CAST_IF $R,$F	把$F中的浮点数转换为整数存到$R里	IRF
	template<class __Res,class __Pass>struct CAST_IF
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tCAST_IF\tat address ",cur);
			return res.Reformat_IRF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tCAST_IF\tat address ",res.IP);
			res.OpRi(1) = typename __Res::S8(res.OpRf(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),res.OpRi(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(2),res.OpRf(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRF(line,ins);
		}
	};
	//CAST_ID $R,$D	把$D中的双精度浮点数转换为整数存到$R里	IRD
	template<class __Res,class __Pass>struct CAST_ID
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tCAST_ID\tat address ",cur);
			return res.Reformat_IRD(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tCAST_ID\tat address ",res.IP);
			res.OpRi(1) = typename __Res::S8(res.OpRd(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),res.OpRi(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(2),res.OpRd(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRD(line,ins);
		}
	};
	//CAST_FI $F,$R	把$R中的整数数转换为浮点数存到$F里	IFR
	template<class __Res,class __Pass>struct CAST_FI
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tCAST_FI\tat address ",cur);
			return res.Reformat_IFR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tCAST_FI\tat address ",res.IP);
			res.OpRf(1) = typename __Res::F4(res.OpRi(2));
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
	//CAST_FD $F,$D	把$D中的双精度浮点数转换为浮点数存到$F里	IFD
	template<class __Res,class __Pass>struct CAST_FD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tCAST_FD\tat address ",cur);
			return res.Reformat_IFD(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tCAST_FD\tat address ",res.IP);
			res.OpRf(1) = typename __Res::F4(res.OpRd(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(2),res.OpRd(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IFD(line,ins);
		}
	};
	//CAST_DI $D,$R	把$R中的整数转换为双精度浮点数存到$D里	IDR
	template<class __Res,class __Pass>struct CAST_DI
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tCAST_DI\tat address ",cur);
			return res.Reformat_IDR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tCAST_DI\tat address ",res.IP);
			res.OpRd(1) = typename __Res::F8(res.OpRi(2));
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
	//CAST_DF $D,$F	把$F中的浮点数转换为双精度浮点数存到$D里	IDF
	template<class __Res,class __Pass>struct CAST_DF
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tCAST_DF\tat address ",cur);
			return res.Reformat_IDF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tCAST_DF\tat address ",res.IP);
			res.OpRd(1) = typename __Res::F8(res.OpRf(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(2),res.OpRf(2));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IDF(line,ins);
		}
	};
	//FADD $F1,$F2,$F3	$F1 = $F2 + $F3	IFFF
	template<class __Res,class __Pass>struct FADD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tFADD\tat address ",cur);
			return res.Reformat_IFFF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tFADD\tat address ",res.IP);
			res.OpRf(1) = res.OpRf(2) + res.OpRf(3);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(2),res.OpRf(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(3),res.OpRf(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IFFF(line,ins);
		}
	};
	//FSUB $F1,$F2,$F3	$F1 = $F2 - $F3	IFFF
	template<class __Res,class __Pass>struct FSUB
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tFSUB\tat address ",cur);
			return res.Reformat_IFFF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tFSUB\tat address ",res.IP);
			res.OpRf(1) = res.OpRf(2) - res.OpRf(3);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(2),res.OpRf(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(3),res.OpRf(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IFFF(line,ins);
		}
	};
	//FMULT $F1,$F2,$F3	$F1 = $F2 * $F3	IFFF
	template<class __Res,class __Pass>struct FMULT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tFMULT\tat address ",cur);
			return res.Reformat_IFFF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tFMULT\tat address ",res.IP);
			res.OpRf(1) = res.OpRf(2) * res.OpRf(3);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(2),res.OpRf(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(3),res.OpRf(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IFFF(line,ins);
		}
	};
	//FDIV $F1,$F2,$F3	$F1 = $F2 / $F3	IFFF
	template<class __Res,class __Pass>struct FDIV
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tFDIV\tat address ",cur);
			return res.Reformat_IFFF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tFDIV\tat address ",res.IP);
			res.OpRf(1) = res.OpRf(2) / res.OpRf(3);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(2),res.OpRf(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(3),res.OpRf(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IFFF(line,ins);
		}
	};
	//FSLT $F1,$F2,$F3	如果$F2小于$F3，则把$F1设置为1；否则把$F1设置为0	IFFF
	template<class __Res,class __Pass>struct FSLT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tFSLT\tat address ",cur);
			return res.Reformat_IFFF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tFSLT\tat address ",res.IP);
			res.OpRf(1) = res.OpRf(2) < res.OpRf(3) ? 1.F : 2.F;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),res.OpRf(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(2),res.OpRf(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(3),res.OpRf(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IFFF(line,ins);
		}
	};
}//namespace DoZerg

#endif