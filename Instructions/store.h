#ifndef STORE_H
#define STORE_H

#include "InsBase.h"

namespace DoZerg{
	//store byte	SB $R1,$R2	把$R1里的字节数存到$R2给出的地址	IRR
	template<class __Res,class __Pass>struct SB
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSB\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSB\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.LeftRiType<typename __Res::S1>(2) = typename __Res::S1(res.OpRi(1));
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
	//store word	SW $R1,$R2	把$R1里的2字节数存到$R2给出的地址
	template<class __Res,class __Pass>struct SW
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSW\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSW\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.LeftRiType<typename __Res::S2>(2) = typename __Res::S2(res.OpRi(1));
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
	//store word	SW $R1,$R2	把$R1里的2字节数存到$R2给出的地址
	template<class __Res,class __Pass>struct SD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSD\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSD\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.LeftRiType<typename __Res::S4>(2) = typename __Res::S4(res.OpRi(1));
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
	//store qword	SQ $R1,$R2	把$R1里的8字节数存到$R2给出的地址
	template<class __Res,class __Pass>struct SQ
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSQ\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSQ\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.LeftRiType<typename __Res::S8>(2) = typename __Res::S8(res.OpRi(1));
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
	//store float	SF1 $F,$R	把$F里的浮点数存到$R给出的地址		IFR
	template<class __Res,class __Pass>struct SF1
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSF1\tat address ",cur);
			return res.Reformat_IFR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSF1\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.LeftRiType<typename __Res::F4>(2) = res.OpRf(1);
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
	//store double	SF2 $D,$R	把$D里的双精度浮点数存到$R给出的地址
	template<class __Res,class __Pass>struct SF2
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tSF2\tat address ",cur);
			return res.Reformat_IDR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tSF2\tat address ",res.IP);
			if(!res.CheckAddress(res.OpRi(2),res.IP + 2))
				return false;
			res.LeftRiType<typename __Res::F8>(2) = res.OpRd(1);
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