#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "StackFrame.h"
#include "Label.h"

namespace DoZerg{
	template<class __Platform>class Procedure
	{
	public:
		typedef typename __Platform::U1 U1;
		typedef typename __Platform::U4 U4;
		typedef typename __Platform::U8 U8;
		typedef ReturnValue<__Platform> __Return;
		typedef Argument<__Platform> __Argument;
		typedef LocalVariable<__Platform> __LocalVar;
		typedef Label<__Platform> __Label;
	public:
		static const SymType = 1;
	public:
		~Procedure(){}
		Procedure(U8 txt,U8 addr,U4 l)
			:text(txt)
			,address(addr)
			,line(l)
			,nRet(0)
		{}
		template<class __FileIO>
			explicit Procedure(const __FileIO & file){Read(file);}
		U8 Text() const{return text;}
		U8 Address() const{return address;}
		U4 Line() const{return line;}
		U1 Ret() const{return nRet;}
		const __Return & Return() const{return ret;}
		const __Argument & Argument(U4 i) const{return arg[i];}
		const __LocalVar & Local(U4 i) const{return local[i];}
		const __Label & Lbl(U4 i) const{return label[i];}
		U4 ArgSize() const{return U4(arg.size());}
		U4 LocalSize() const{return U4(local.size());}
		U4 LabelSize() const{return U4(label.size());}
		U4 AddLabel(const __Label & lbl){
			label.push_back(lbl);
			return U4(label.size()) - 1;
		}
		void SetReturn(const __Return & stackframe){
			ret = stackframe;
			nRet = 1;
		}
		U4 AddArgument(const __Argument & stackframe){
			arg.push_back(stackframe);
			return U4(arg.size()) - 1;
		}
		U4 AddVariable(const __LocalVar & stackframe){
			local.push_back(stackframe);
			return U4(local.size()) - 1;
		}
		template<class __StrTbl,class __OutStream>
			void Print(const __StrTbl & strTbl,__OutStream & out) const{
				out<<"Procedure:\n"
					<<"\t\tidentifier	= "<<strTbl[text].c_str();
				out<<"\n\t\taddress	= "<<address;
				out<<"\n\t\tline	= "<<line<<'\n';
				if(nRet){
					out<<"\t\tRET:\n\t\t\t";
					ret.Print(strTbl,out);
				}
				out<<"\t\tARGS:\n";
				for(U4 i = 0,j = U4(arg.size());i < j;++i){
					out<<"\t\t"<<i<<'\t';
					arg[i].Print(strTbl,out);
				}
				out<<"\t\tLOCALS:\n";
				for(U4 i = 0,j = U4(local.size());i < j;++i){
					out<<"\t\t"<<i<<'\t';
					local[i].Print(strTbl,out);
				}
				out<<"\t\tLABELS:\n";
				for(U4 i = 0,j = U4(label.size());i < j;++i){
					out<<"\t\t"<<i<<'\t';
					label[i].Print(strTbl,out);
				}
				out<<"\tEnd of Procedure\n";
			}
		template<class __FileIO>
			const Procedure & Read(const __FileIO & file){
				file.GetVar(text);
				file.GetVar(address);
				file.GetVar(line);
				if(file.GetVar(nRet))
					ret.Read(file);
				U4 size;
				//read arguments
				arg.reserve(file.GetVar(size));
				for(U4 i = 0;i < size;++i)
					arg.push_back(__Argument(file));
				//read locals
				local.reserve(file.GetVar(size));
				for(U4 i = 0;i < size;++i)
					local.push_back(__LocalVar(file));
				//read labels
				label.reserve(file.GetVar(size));
				for(U4 i = 0;i < size;++i)
					label.push_back(__Label(file));
				return *this;
			}
		template<class __FileIO>
			void Write(const __FileIO & file) const{
				file.PutVar(text);		//8
				file.PutVar(address);	//8
				file.PutVar(line);		//4
				file.PutVar(nRet);		//1
				if(nRet)
					ret.Write(file);
				file.PutVar(ArgSize());		//4
				for(U4 i = 0;i < ArgSize();++i)
					arg[i].Write(file);
				file.PutVar(LocalSize());	//4
				for(U4 i = 0;i < LocalSize();++i)
					local[i].Write(file);
				file.PutVar(LabelSize());	//4
				for(U4 i = 0;i < LabelSize();++i)
					label[i].Write(file);
			}
		U8 TotalBytes() const{
			return 33 + (ArgSize() + LocalSize() + U4(nRet)) * __Argument::TotalBytes
				+ LabelSize() * __Label::TotalBytes;
		}
	private:
		U8 text;		//index into StringTable of where identifier begins
		U8 address;		//address of procedure
		U4 line;		//source code line containing declaration
		U1 nRet;		//0 = void return,1 = returns a value
		__Return ret;
		std::vector<__Argument> arg;
		std::vector<__LocalVar> local;
		std::vector<__Label> label;
	};
}//namespace DoZerg

#endif