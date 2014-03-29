#ifndef DBLMATH_H
#define DBLMATH_H

#include "InsBase.h"

namespace DoZerg{
	//DADD $D1,$D2,$D3	$D1 = $D2 + $D3	IDDD
	template<class __Res,class __Pass>struct DADD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tDADD\tat address ",cur);
			return res.Reformat_IDDD(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tDADD\tat address ",res.IP);
			res.OpRd(1) = res.OpRd(2) + res.OpRd(3);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(2),res.OpRd(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(3),res.OpRd(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IDDD(line,ins);
		}
	};
	//DSUB $D1,$D2,$D3	$D1 = $D2$ - D3
	template<class __Res,class __Pass>struct DSUB
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tDSUB\tat address ",cur);
			return res.Reformat_IDDD(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tDSUB\tat address ",res.IP);
			res.OpRd(1) = res.OpRd(2) - res.OpRd(3);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(2),res.OpRd(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(3),res.OpRd(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IDDD(line,ins);
		}
	};
	//DMULT $D1,$D2,$D3	$D1 = $D2$ * D3
	template<class __Res,class __Pass>struct DMULT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tDMULT\tat address ",cur);
			return res.Reformat_IDDD(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tDMULT\tat address ",res.IP);
			res.OpRd(1) = res.OpRd(2) * res.OpRd(3);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(2),res.OpRd(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(3),res.OpRd(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IDDD(line,ins);
		}
	};
	//DDIV $D1,$D2,$D3	$D1 = $D2$ / D3
	template<class __Res,class __Pass>struct DDIV
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tDDIV\tat address ",cur);
			return res.Reformat_IDDD(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tDDIV\tat address ",res.IP);
			res.OpRd(1) = res.OpRd(2) / res.OpRd(3);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(2),res.OpRd(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(3),res.OpRd(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IDDD(line,ins);
		}
	};
	//DSLT $D1,$D2,$D3	如果$D2小于$D3，则把$D1设置为1；否则把$D1设置为0
	template<class __Res,class __Pass>struct DSLT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tDSLT\tat address ",cur);
			return res.Reformat_IDDD(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tDSLT\tat address ",res.IP);
			res.OpRd(1) = res.OpRd(2) < res.OpRd(3) ? 1. : 2.;
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),res.OpRd(1));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(2),res.OpRd(2));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(3),res.OpRd(3));
			res.IP += 4;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 4;
			return line.Size() == 4;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IDDD(line,ins);
		}
	};
}//namespace VirtualMachine

#endif