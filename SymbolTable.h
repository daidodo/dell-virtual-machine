#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Dictionary.h"
#include "GlobalVariable.h"
#include "Procedure.h"
#include "StringTable.h"

namespace DoZerg{
	template<class __Platform>class SymbolTable
	{
	public:
		typedef typename __Platform::U1 U1;
		typedef typename __Platform::U4 U4;
		typedef typename __Platform::U8 U8;
		typedef typename __Platform::S8 S8;
		typedef GlobalVariable<__Platform> __GVar;
		typedef Procedure<__Platform> __Proc;
		typedef typename __Proc::__Return __Ret;
		typedef typename __Proc::__Argument __Arg;
		typedef typename __Proc::__LocalVar __Loc;
		typedef typename __Proc::__Label __Lab;
	private:
		typedef StringTable<__Platform> __StrTbl;
		typedef Dictionary<__StrTbl> __Dic;
	public:
		typedef typename __Dic::const_iterator __CIter;
	public:
		enum AddressEnum{BadId,Direct,Indirect};
	public:
		SymbolTable():dic_(strTbl_),currentProc_(0){}
		~SymbolTable(){}
		U4 GlobVarSize() const{return U4(globVar_.size());}
		U4 ProcSize() const{return U4(proc_.size());}
		U8 StrTblSize() const{return strTbl_.TotalBytes();}
		AddressEnum QueryAddress(const std::string & id,S8 & addr){
			__CIter it = dic_.Query(id);
			if(it == dic_.end())
				return BadId;
			AddressEnum ret;
			int type = it->second.Type();
			U4 index = it->second.Index();
			U4 subIndex = it->second.SubIndex();
			switch(type){
				case __GVar::SymType:{
					addr = -globVar_[index].Offset();
					ret = Indirect;
					break;}
				case __Proc::SymType:{
					addr = proc_[index].Address();
					ret = Direct;
					break;}
				case __Ret::SymType:{
					addr = proc_[index].Return().Offset();
					ret = Indirect;
					break;}
				case __Arg::SymType:{
					addr = proc_[index].Argument(subIndex).Offset();
					ret = Indirect;
					break;}
				case __Loc::SymType:{
					addr = proc_[index].Local(subIndex).Offset();
					ret = Indirect;
					break;}
				case __Lab::SymType:{
					addr = proc_[index].Lbl(subIndex).Address();
					ret = Direct;
					break;}
				default:{
				}
			}
			return ret;
		}
		bool Has(const std::string id){return dic_.Has(id);}
		void AddGlobalVariable(const std::string & id,U1 dtype,U8 length,U4 line){
			__GVar gv(strTbl_.AddString(id),dtype,length,line);
			dic_.AddEntry(gv,AddGlobalVariable(gv),0);
		}
		void AddProcedure(const std::string & id,U8 bytePos,U4 line){
			__Proc pr(strTbl_.AddString(id),bytePos,line);
			dic_.AddEntry(pr,currentProc_ = AddProcedure(pr),0);
		}
		void AddProcLabel(const std::string & id,U8 bytePos,U4 line){
			__Lab lbl(strTbl_.AddString(id),bytePos,line);
			dic_.AddEntry(lbl,currentProc_,proc_[currentProc_].AddLabel(lbl));
		}
		void AddProcRet(const std::string & id,S8 offset,U4 line){
			__Ret sf(strTbl_.AddString(id),offset,line);
			proc_[currentProc_].SetReturn(sf);
			dic_.AddEntry(sf,currentProc_,0);
		}
		void AddProcArg(const std::string & id,S8 offset,U4 line){
			__Arg sf(strTbl_.AddString(id),offset,line);
			dic_.AddEntry(sf,currentProc_,proc_[currentProc_].AddArgument(sf));
		}
		void AddProcVar(const std::string & id,S8 offset,U4 line){
			__Loc sf(strTbl_.AddString(id),offset,line);
			dic_.AddEntry(sf,currentProc_,proc_[currentProc_].AddVariable(sf));
		}
		template<class __OutStream>
			void Print(__OutStream & out) const{
				strTbl_.Print(out);
				out<<"Symbol Table:\n";
				for(U4 i = 0,j = U4(globVar_.size());i < j;++i){
					out<<'\t'<<i<<") ";
					globVar_[i].Print(strTbl_,out);
				}
				for(U4 i = 0,j = U4(proc_.size());i < j;++i){
					out<<'\t'<<i<<") ";
					proc_[i].Print(strTbl_,out);
				}
				out<<"End of Symbol Table\n";
				dic_.Print(out);
			}
		template<class __FileIO>
			const SymbolTable & Read(const __FileIO & file){
				U4 size;
				//read GlobalVariable
				globVar_.reserve(file.GetVar(size));
				for(U4 i = 0;i < size;++i)
					globVar_.push_back(__GVar(file));
				//read Procedure
				proc_.reserve(file.GetVar(size));
				for(U4 i = 0;i < size;++i)
					proc_.push_back(__Proc(file));
				//read string table
				strTbl_.Read(file);
				return *this;
			}
		template<class __FileIO>
			void Write(const __FileIO & file) const{
				file.PutVar(GlobVarSize());
				for(U4 i = 0;i < GlobVarSize();++i)
					globVar_[i].Write(file);
				file.PutVar(ProcSize());
				for(U4 i = 0;i < ProcSize();++i)
					proc_[i].Write(file);
				//write string table
				strTbl_.Write(file);
			}
		U8 TotalBytes() const{
			U8 size =  8 + GlobVarSize() * __GVar::TotalBytes;
			for(U4 i = 0;i < ProcSize();++i)
				size += proc_[i].TotalBytes();
			return size;
		}
	private:
		U4 AddGlobalVariable(const __GVar & gv){
			globVar_.push_back(gv);
			return U4(globVar_.size()) - 1;
		}
		U4 AddProcedure(const __Proc & pr){
			proc_.push_back(pr);
			return U4(proc_.size()) - 1;
		}
	private:
		std::vector<__GVar> globVar_;
		std::vector<__Proc> proc_;
		__StrTbl strTbl_;
		__Dic dic_;
		U4 currentProc_;		//index into symTbl of current procedure
	};
}//namespace DoZerg

#endif