#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <map>
#include <vector>
#include <string>
#include <cassert>
#include "Instruction.h"
#include "Instructions\load.h"
#include "Instructions\store.h"
#include "Instructions\pushpop.h"
#include "Instructions\move.h"
#include "Instructions\jump.h"
#include "Instructions\bitwise.h"
#include "Instructions\shift.h"
#include "Instructions\intmath.h"
#include "Instructions\fltmath.h"
#include "Instructions\dblmath.h"

namespace DoZerg{
#ifdef VM_DBG_INSSET
#define VM_DBG_INSSET0(str)				std::cout<<"InstructionSet::Initialize(): "<<str;
#define VM_DBG_INSSET1(str,arg1)		std::cout<<"InstructionSet::Initialize(): ";std::printf(str,arg1);
#define VM_DBG_INSSET2(str,arg1,arg2)	std::cout<<"InstructionSet::Initialize(): ";std::printf(str,arg1,arg2);
#else
#define VM_DBG_INSSET0(str)
#define VM_DBG_INSSET1(str,arg1)
#define VM_DBG_INSSET2(str,arg1,arg2)
#endif
	template<
		class __Res,
		class __Pass
	>class InstructionSet
	{
	public:
		typedef Instruction<__Res,__Pass> __InsType;
		typedef typename __Pass::__Line __Line;
	private:
		typedef typename __Line::__Word __Word;
		typedef std::map<__Word,int> __Map;
		typedef std::pair<typename __Map::iterator,bool> __MapRet;
	public:
		enum ReformatReturnEnum{r_SECCESS,r_FAIL,r_BADOPCODE};			//Reformat() return value
		enum HandleReturnEnum{r_HALT,r_NORMAL,r_ERROR,r_BadCode};	//Handle() return value
		enum PassReturnEnum{r_Success,r_Fail,r_BadCommand};				//Pass() return value
	public:
		~InstructionSet(){}
		InstructionSet():insHALT_(0){}
		void Initialize(){
			//HALT: InsHALT = 0
			insHALT_ = RegIns<HALT>("HALT");
			//DATA TRANSFER
			RegIns<LBI>("LBI");
			RegIns<LWI>("LWI");
			RegIns<LDI>("LDI");
			RegIns<LQI>("LQI");
			RegIns<LF1I>("LF1I");
			RegIns<LF2I>("LF2I");
			RegIns<LAD>("LAD");
			RegIns<LAI>("LAI");
			RegIns<LB>("LB");
			RegIns<LW>("LW");
			RegIns<LD>("LD");
			RegIns<LQ>("LQ");
			RegIns<LF1>("LF1");
			RegIns<LF2>("LF2");
			RegIns<SB>("SB");
			RegIns<SW>("SW");
			RegIns<SD>("SD");
			RegIns<SQ>("SQ");
			RegIns<SF1>("SF1");
			RegIns<SF2>("SF2");
			RegIns<PUSHB>("PUSHB");
			RegIns<PUSHW>("PUSHW");
			RegIns<PUSHD>("PUSHD");
			RegIns<PUSHQ>("PUSHQ");
			RegIns<PUSHF1>("PUSHF1");
			RegIns<PUSHF2>("PUSHF2");
			RegIns<POPB>("POPB");
			RegIns<POPW>("POPW");
			RegIns<POPD>("POPD");
			RegIns<POPQ>("POPQ");
			RegIns<POPF1>("POPF1");
			RegIns<POPF2>("POPF2");
			RegIns<MOV>("MOV");
			RegIns<MOVF>("MOVF");
			RegIns<MOVD>("MOVD");
			//PROGRAM FLOW CONTROL
			RegIns<JMP>("JMP");
			RegIns<JE>("JE");
			RegIns<JNE>("JNE");
			RegIns<SLT>("SLT");
			RegIns<INT>("INT");
			RegIns<DI>("DI");
			RegIns<EI>("EI");
			RegIns<NOP>("NOP");
			//BITWISE
			RegIns<AND>("AND");
			RegIns<OR>("OR");
			RegIns<XOR>("XOR");
			RegIns<NOT>("NOT");
			RegIns<BT>("BT");
			RegIns<BS>("BS");
			//SHIFT
			RegIns<SRA>("SRA");
			RegIns<SRL>("SRL");
			RegIns<SL>("SL");
			//INTEGER ARITHMETIC
			RegIns<ADD>("ADD");
			RegIns<SUB>("SUB");
			RegIns<MULT>("MULT");
			RegIns<DIV>("DIV");
			//CONVERSION
			RegIns<CAST_IF>("CAST_IF");
			RegIns<CAST_ID>("CAST_ID");
			RegIns<CAST_FI>("CAST_FI");
			RegIns<CAST_FD>("CAST_FD");
			RegIns<CAST_DI>("CAST_DI");
			RegIns<CAST_DF>("CAST_DF");
			//FLOATING-POINT MATH
			RegIns<FADD>("FADD");
			RegIns<FSUB>("FSUB");
			RegIns<FMULT>("FMULT");
			RegIns<FDIV>("FDIV");
			RegIns<FSLT>("FSLT");
			RegIns<DADD>("DADD");
			RegIns<DSUB>("DSUB");
			RegIns<DMULT>("DMULT");
			RegIns<DDIV>("DDIV");
			RegIns<DSLT>("DSLT");
		}
		ReformatReturnEnum Reformat(__Res & res,typename __Res::U8 & cur) const{
			int opcode = res.RAM[cur];
			if(!Valid(opcode))
				return r_BADOPCODE;
			if(!insVec_[opcode].Reformat(res,cur))
				return r_FAIL;
			return r_SECCESS;
		}
		HandleReturnEnum Handle(__Res & res) const{
			int opcode = res.OpCode();
			if(!Valid(opcode))
				return r_BadCode;
			if(IsHalt(opcode)){
#ifdef VM_DBG_INS
				insVec_[insHALT_].Handle(res);
#endif
				return r_HALT;
			}
			return insVec_[opcode].Handle(res) ? r_NORMAL : r_ERROR;
		}
		PassReturnEnum Pass1(const __Line & line,typename __Pass::U8 & bytePos) const{
			__Map::const_iterator where = insDic_.find(line[0]);
			if(where == insDic_.end())
				return r_BadCommand;
			return insVec_[where->second].Pass1(bytePos,line) ? r_Success : r_Fail;
		}
		PassReturnEnum Pass2(__Pass & pass,const __Line & line) const{
			__Map::const_iterator where = insDic_.find(line[0]);
			assert(where != insDic_.end() && "InstructionSet::Pass2()");
			if(!insVec_[where->second].Pass2(pass,line,where->second))
				return r_Fail;
			return r_Success;
		}
	private:
		bool IsHalt(int opcode) const{return opcode == insHALT_;}
		bool Valid(int index) const{return index < int(insVec_.size());}
	private:
		template<template<class,class>class __IType>
			int RegIns(const std::string & name){
				typedef __IType<__Res,__Pass> __I;
				int opcode = int(insVec_.size());
				__MapRet ret = insDic_.insert(std::make_pair(name,opcode));
				assert(ret.second && "InstructionSet::RegIns(): instruction is already registed");
				insVec_.push_back(
					__InsType(
						__I::Reformat,
						__I::Handle,
						__I::Pass1,
						__I::Pass2
						));
				VM_DBG_INSSET2("instruction %s registed,opcode = %d\n",name.c_str(),opcode);
				return opcode;
			}
	private:
		int insHALT_;						//HALT instruction opcode
		std::vector<__InsType> insVec_;		//instruction vector
		__Map insDic_;						//instruction dictionary
	};
}//namespace DoZerg

#endif