#ifndef INTMATH_H
#define INTMATH_H

#include "InsBase.h"

namespace DoZerg{
	//ADD $R1,$R2,$R3	$R1 = $R2 + $R3	IRRR
	template<class __Res,class __Pass>struct ADD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tADD\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tADD\tat address ",res.IP);
			res.OpRi(1) = typename __Res::S8(res.OpRi(2)) + typename __Res::S8(res.OpRi(3));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),typename __Res::S8(res.OpRi(2)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(3),typename __Res::S8(res.OpRi(3)));
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
	//SUB $R1,$R2,$R3	$R1 = $R2 - $R3	IRRR
	template<class __Res,class __Pass>struct SUB
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSUB\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSUB\tat address ",res.IP);
			res.OpRi(1) = typename __Res::S8(res.OpRi(2)) - typename __Res::S8(res.OpRi(3));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),typename __Res::S8(res.OpRi(2)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(3),typename __Res::S8(res.OpRi(3)));
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
	//MULT $R1,$R2,$R3	$R1 = $R2 * $R3	IRRR
	template<class __Res,class __Pass>struct MULT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tMULT\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tMULT\tat address ",res.IP);
			res.OpRi(1) = typename __Res::S8(res.OpRi(2)) * typename __Res::S8(res.OpRi(3));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),typename __Res::S8(res.OpRi(2)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(3),typename __Res::S8(res.OpRi(3)));
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
	//DIV $R1,$R2,$R3,$R4	$R1 = $R3 / $R4ºÍ$R2 = $R3 % $R4	IRRRR
	template<class __Res,class __Pass>struct DIV
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tDIV\tat address ",cur);
			return res.Reformat_IRRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tDIV\tat address ",res.IP);
			bool ret = true;
			if(res.OpRi(4) == 0){
				res.OpRi(1) = res.OpRi(2) = -1;
				ret = false;
			}else{
				res.OpRi(1) = typename __Res::S8(res.OpRi(3)) / typename __Res::S8(res.OpRi(4));
				res.OpRi(2) = typename __Res::S8(res.OpRi(3)) % typename __Res::S8(res.OpRi(4));
			}
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),typename __Res::S8(res.OpRi(2)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(3),typename __Res::S8(res.OpRi(3)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(4),typename __Res::S8(res.OpRi(4)));
			if(!ret)
				return false;
			res.IP += 5;
			return ret;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 5;
			return line.Size() == 5;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IRRRR(line,ins);
		}
	};

}//namespace DoZerg

#endif