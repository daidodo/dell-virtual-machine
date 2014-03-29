#ifndef INSBASE_H
#define INSBASE_H

namespace DoZerg{
#ifdef VM_DBG_INS
#define VM_DBG_INS0(str)				std::cout<<str;
#define VM_DBG_INS1(str,arg1)			std::printf(str,arg1);
#define VM_DBG_INS2(str,arg1,arg2)		std::printf(str,arg1,arg2);
#define	VM_DBG_INS_P8(str,arg1)			std::cout<<str<<arg1<<'\n';
#define	VM_DBG_INS1_P8(str,arg1,arg2)	VM_DBG_INS1(str,arg1);std::cout<<arg2<<'\n';
#else
#define VM_DBG_INS0(str)
#define VM_DBG_INS1(str,arg1)
#define VM_DBG_INS2(str,arg1,arg2)
#define	VM_DBG_INS_P8(str,arg1)
#define	VM_DBG_INS1_P8(str,arg1,arg2)
#endif
}//namespace DoZerg

#endif