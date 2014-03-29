#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "IntFileIO.h"
#include "IntFileManagement.h"
#include "IntProcessManagement.h"
#include "IntTimeDateCall.h"
#include "IntCommandLine.h"
#include "IntMemoryStatus.h"
#include "IntAllocationCall.h"
#include "IntMathCall.h"
#include "IntNativeCall.h"
#include "IntIPC.h"

namespace DoZerg{
#ifdef VM_DBG_INTERRUPT
#define VM_DBG_INTERRUPT0(str)				std::cout<<"InterruptHandleInt(): "<<str;
#define VM_DBG_INTERRUPT1(str,arg1)			std::cout<<"InterruptHandleInt(): ";std::printf(str,arg1);
#define VM_DBG_INTERRUPT2(str,arg1,arg2)	std::cout<<"InterruptHandleInt(): ";std::printf(str,arg1,arg2);
#else
#define VM_DBG_INTERRUPT0(str)
#define VM_DBG_INTERRUPT1(str,arg1)
#define VM_DBG_INTERRUPT2(str,arg1,arg2)
#endif
	struct Interrupt
	{
		static bool IntOn;
		~Interrupt(){}
		Interrupt(){}
		template<class __Res>
			static bool HandleInt(__Res & res,int vec){
				VM_DBG_INTERRUPT1("received vector (%u)\n",vec);
				if(!IntOn){
					VM_DBG_INTERRUPT0("interrupts are disabled\n");
					return true;
				}
				switch(vec){
					case 0:
						VM_DBG_INTERRUPT0("handling vector 0: File IO\n");
						return IntFileIO<__Res>(res);
					case 1:
						VM_DBG_INTERRUPT0("handling vector 1: File Management\n");
						return IntFileManagement<__Res>(res);
					case 2:
						VM_DBG_INTERRUPT0("handling vector 3: Process Management\n");
						return IntProcessManagement<__Res>(res);
					case 3:
						VM_DBG_INTERRUPT0("handling vector 3: debug mode = TRUE\n");
						res.DebugModeOn = true;
						return true;
					case 4:
						VM_DBG_INTERRUPT0("handling vector 4: Time Date Call\n");
						return IntTimeDateCall<__Res>(res);
					case 5:
						VM_DBG_INTERRUPT0("handling vector 5: Command Line\n");
						return IntCommandLine<__Res>(res);
					case 6:
						VM_DBG_INTERRUPT0("handling vector 6: Memory Status\n");
						return IntMemoryStatus<__Res>(res);
					case 7:
						VM_DBG_INTERRUPT0("handling vector 7: Allocateion Call");
						return IntAllocationCall<__Res>(res);
					case 8:
						VM_DBG_INTERRUPT0("handling vector 8: Math Call\n");
						return IntMathCall<__Res>(res);
					case 9:
						VM_DBG_INTERRUPT0("handling vector 9: Native Call\n");
						return IntNativeCall<__Res>(res);
					case 10:
						VM_DBG_INTERRUPT0("handling vector 10: IPC\n");
						return IntIPC<__Res>(res);
					default:{
						VM_DBG_INTERRUPT1("vector not handled (%u)\n",vec);
					}
				}
				return false;
			}
	};
	bool Interrupt::IntOn = true;
}//namespace DoZerg

#endif