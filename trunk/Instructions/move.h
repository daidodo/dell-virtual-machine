#ifndef MOVE_H
#define MOVE_H

#include "InsBase.h"

namespace DoZerg{
	//MOV $R1,$R2	把$R2里的内容复制到$R1	IRR
	template<class __Res,class __Pass>struct MOV
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tMOV\tat address ",cur);
			return res.Reformat_IRR(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tMOV\tat address ",res.IP);
			res.OpRi(1) = res.OpRi(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(1),typename __Res::S8(res.OpRi(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRiName(2),typename __Res::S8(res.OpRi(2)));
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
	//MOVF $F1,$F2	把$F2里的内容复制到$F1	IFF
	template<class __Res,class __Pass>struct MOVF
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tMOVF\tat address ",cur);
			return res.Reformat_IFF(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tMOVF\tat address ",res.IP);
			res.OpRf(1) = res.OpRf(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(1),typename __Res::S8(res.OpRf(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRfName(2),typename __Res::S8(res.OpRf(2)));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IFF(line,ins);
		}
	};
	//MOVD $D1,$D2	把$D2里的内容复制到$D1
	template<class __Res,class __Pass>struct MOVD
	{
		static bool Reformat( __Res & res,typename __Res::U8 & cur){
			VM_DBG_INS_P8("Reformat():\nopcode:\t\tMOVD\tat address ",cur);
			return res.Reformat_IDD(cur);
		}
		static bool Handle(__Res & res){
			VM_DBG_INS_P8("Handle():\nopcode:\t\tMOVD\tat address ",res.IP);
			res.OpRd(1) = res.OpRd(2);
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(1),typename __Res::S8(res.OpRd(1)));
			VM_DBG_INS1_P8("\t\t%s = ",res.OpRdName(2),typename __Res::S8(res.OpRd(2)));
			res.IP += 3;
			return true;
		}
		static bool Pass1(typename __Pass::U8 & bytePos,const typename __Pass::__Line & line){
			bytePos += 3;
			return line.Size() == 3;
		}
		static bool Pass2(__Pass & pass,const typename __Pass::__Line & line,int ins){
			return pass.Process_IDD(line,ins);
		}
	};
}//namespace DoZerg

#endif