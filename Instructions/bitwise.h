#ifndef BITWISE_H
#define BITWISE_H

#include "InsBase.h"

namespace DoZerg{
	//AND $R1,$R2,$R3	��$R2��$R3�ġ���λ�롱�������$R1	IRRR
	template<class __Res,class __Pass>struct AND
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tAND\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tAND\tat address ",res.IP);
			res.OpRi(1) = res.OpRi(2) & res.OpRi(3);
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
	//OR $R1,$R2,$R3	��$R2��$R3�ġ���λ�򡱽������$R1
	template<class __Res,class __Pass>struct OR
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tOR\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tOR\tat address ",res.IP);
			res.OpRi(1) = res.OpRi(2) | res.OpRi(3);
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
	//XOR $R1,$R2,$R3	��$R2��$R3�ġ���λ��򡱽������$R1
	template<class __Res,class __Pass>struct XOR
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tXOR\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tXOR\tat address ",res.IP);
			res.OpRi(1) = res.OpRi(2) ^ res.OpRi(3);
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
	//NOT $R1,$R2	��$R2�ġ���λ�ǡ��������$R1	IRR
	template<class __Res,class __Pass>struct NOT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tNOT\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tNOT\tat address ",res.IP);
			res.OpRi(1) = ~res.OpRi(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),res.OpRi(1));
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
	//bit test	BT $R1,$R2,$R3	���$R2�еĵ�$R3λ��1�����$R1����Ϊ1�����򣬰�$R1����Ϊ0	IRRR
	template<class __Res,class __Pass>struct BT
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tBT\tat address ",cur);
			return res.Reformat_IRRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tBT\tat address ",res.IP);
			res.OpRi(1) = (((typename __Res::U8(1) << (res.OpRi(3) % 64)) & res.OpRi(2)) != 0 ? 1 : 0);
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
	//bit set	BS $R1,$R2	��$R1�еĵ�$R2λ������λ����1��	IRR
	template<class __Res,class __Pass>struct BS
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tBS\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tBS\tat address ",res.IP);
			res.OpRi(1) |= (typename __Res::U8(1) << (res.OpRi(2) % 64));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),res.OpRi(1));
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
}//namespace DoZerg

#endif