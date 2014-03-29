#ifndef SHIFT_H
#define SHIFT_H

#include "InsBase.h"

namespace DoZerg{
	//shift right arithmetic算术右移	SRA $R1,$R2,$R3	把$R2右移$R3位，存入$R1里，用符号位进行左填充	IRRR
	template<class __Res,class __Pass>struct SRA
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSRA\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSRA\tat address ",res.IP);
			res.OpRi(1) = typename __Res::S8(res.OpRi(2)) >> typename __Res::S8(res.OpRi(3));
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
	//shift right logic逻辑右移	SRL $R1,$R2,$R3	把$R2右移$R3位，存入$R1里，用0进行左填充	IRRR
	template<class __Res,class __Pass>struct SRL
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSRL\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSRL\tat address ",res.IP);
			res.OpRi(1) = typename __Res::U8(res.OpRi(2)) >> typename __Res::U8(res.OpRi(3));
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
	//shift left左移	SL $R1,$R2,$R3	把$R2左移$R3位，存入$R1里，用0进行右填充	IRRR
	template<class __Res,class __Pass>struct SL
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSL\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSL\tat address ",res.IP);
			res.OpRi(1) = typename __Res::U8(res.OpRi(2)) << typename __Res::U8(res.OpRi(3));
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
}//namespace DoZerg

#endif