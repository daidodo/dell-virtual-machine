#ifndef DELLVM_H
#define DELLVM_H

#include "Platform\Windows32.h"
#include "InstructionSet.h"
#include "Resources.h"
#include "Pass.h"
#include "ErrorFile.h"
#include "ExecuteCmdLine.h"
#include "BuildCmdLine.h"
#include "Debugger.h"

namespace DoZerg{
#ifdef VM_DBG_DELLVM
#define VM_DBG_DELLVM0(str)				std::cout<<str;
#define VM_DBG_DELLVM1(str,arg1)		std::printf(str,arg1);
#define VM_DBG_DELLVM2(str,arg1,arg2)	std::printf(str,arg1,arg2);
#define	VM_DBG_DELLVM_PU8(str,arg1)		VM_DBG_DELLVM0(str);__Platform::pU8(arg1);std::cout<<'\n';
#else
#define VM_DBG_DELLVM0(str)
#define VM_DBG_DELLVM1(str,arg1)
#define VM_DBG_DELLVM2(str,arg1,arg2)
#define	VM_DBG_DELLVM_PU8
#endif
	template<
		class __Platform,
		template<class,class>class __InsSet = InstructionSet,
		template<class,class>class __Resources = Resources,
		template<class,class,class>class __Pass = Pass,
		template<class>class __ErrHandle = ErrorFile
	>class DEllVM
	{
	private:
		typedef typename __Platform::U8 U8;
		typedef ExecuteCmdLine<__Platform>			__ExeCmd;
		typedef __ErrHandle<__Platform>				__ErrFile;
		typedef typename __ErrFile::__ErrCount		__ErrCount;
		typedef BuildCmdLine<__ErrCount>			__BldCmd;
		typedef __Resources<__Platform,__ErrFile>	__Res;
		typedef __Pass<__Platform,__BldCmd,__Res>	__Pas;
		typedef __InsSet<__Res,__Pas>				__Ins;
	public:
		typedef typename __ExeCmd::__Arguments __ExeArgs;
		typedef typename __BldCmd::__Arguments __BldArgs;
	public:
		DEllVM():res_(err_){ins_.Initialize();}
		~DEllVM(){}
		bool Execute(const __ExeArgs & args){
			VM_DBG_DELLVM0("Execute(): 1) set up error file\n");
			if(!err_.Setup())
				return false;

			VM_DBG_DELLVM0("\nExecute(): 2) handle program arguments\n");
			__ExeCmd CmdLine;
			if(!CmdLine.HandleArguments(err_,args))
				return false;

			VM_DBG_DELLVM0("\nExecute(): 3) init execution environment and load bytecode\n");
			if(!res_.Initialize(CmdLine))
				return false;

			VM_DBG_DELLVM0("\nExecute(): 4) re-format numeric operands to native and verify\n");
			if(!Reformat())
				return false;

			VM_DBG_DELLVM0("\nExecute(): 5) begin execution\n");
			if(!Handle())
				return false;

			VM_DBG_DELLVM0("\nExecute(): 6) release RAM and safe shutdown\n");
			res_.DeallocateRAM();
			err_.Close();
			return true;
		}
		bool Build(const __BldArgs & args){
			VM_DBG_DELLVM0("Build(): 1) handling arguments\n");
			__BldCmd cmdline;
			if(!cmdline.HandleArguments(args))
				return false;

			VM_DBG_DELLVM0("Build(): 2) scan,parse,populate symbol table\n");
			__Pas pass(cmdline,res_);
			pass.Scan(ins_);
			if(__ErrCount::CheckError() != 0)
				return false;

			VM_DBG_DELLVM0("Build(): 3) build compilation unit\n");
			if(!pass.Build())
				return false;

			VM_DBG_DELLVM0("Build(): 4) safe shutdown\n");
			return true;
		}
	private:
		bool Reformat(){
			VM_DBG_DELLVM0("Reformat(): initiating reformat\n");
			VM_DBG_DELLVM0("Reformat(): starting at address->0\n");
			VM_DBG_DELLVM_PU8("Reformat(): stop at address->",res_.HS);
			for(U8 current_byte = 0;current_byte < res_.HS;){
				__Ins::ReformatReturnEnum ret = ins_.Reformat(res_,current_byte);
				if(ret == __Ins::r_SECCESS)
					continue;
				if(ret == __Ins::r_BADOPCODE)
					err_<<BeginError
						<<"bad opcode "<<res_.RAM[current_byte]<<" at address "<<current_byte<<'\n'
						<<EndError;
				return false;
			}
			VM_DBG_DELLVM0("Reformat(): reformatting complete\n");
			return true;
		}
		bool Handle(){
			VM_DBG_DELLVM0("Execute(): initiating bytecode execution\n");
			for(__Ins::HandleReturnEnum ret = ins_.Handle(res_);ret != __Ins::r_HALT;ret = ins_.Handle(res_)){
				if(ret == __Ins::r_BadCode){
					err_<<BeginError
						<<"bad instruction code while handling ("<<res_.OpCode()<<") at address "<<res_.IP<<'\n'
						<<EndError;
					return false;
				}else if(ret == __Ins::r_ERROR){			//error occurs in instruction handling
					err_<<BeginError
						<<"error occurs while handling ("<<res_.OpCode()<<") at address "<<res_.IP<<'\n'
						<<EndError;
					return false;
				}
				if(res_.DebugModeOn){
					Debugger debugger;
					debugger.ReadCommand(res_);
				}
			}
			VM_DBG_DELLVM0("Execute(): instruction execution finished\n");
			return true;
		}
	private:
		__ErrFile err_;
		__Res res_;
		__Ins ins_;
	};
	typedef DEllVM<Windows32Platform> Win32DEllVM;
}//namespace DoZerg

#endif