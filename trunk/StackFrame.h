#ifndef STACKFRAME_H
#define STACKFRAME_H

namespace DoZerg{
	template<class __Platform>class StackFrame
	{
	public:
		typedef typename __Platform::U4 U4;
		typedef typename __Platform::U8 U8;
		typedef typename __Platform::S8 S8;
	public:
		static const TotalBytes = 20;
	public:
		~StackFrame(){}
		StackFrame(){}
		template<class __FileIO>
			explicit StackFrame(const __FileIO & file){Read(file);}
		StackFrame(U8 txt,S8 offset,U4 ln)
			:text(txt)
			,fpOffset(offset)
			,line(ln)
		{}
		U8 Text() const{return text;}
		S8 Offset() const{return fpOffset;}
		U4 Line() const{return line;}
		template<class __StrTbl,class __OutStream>
			void Print(const __StrTbl & strTbl,__OutStream & out) const{
				out<<"id	= "<<strTbl[text].c_str()
					<<"\n\t\t\tfpOffset	= "<<fpOffset
					<<"\n\t\t\tline	= "<<line<<'\n';
			}
		template<class __FileIO>
			const StackFrame & Read(const __FileIO & file){
				file.GetVar(text);
				file.GetVar(fpOffset);
				file.GetVar(line);
				return *this;
			}
		template<class __FileIO>
			void Write(const __FileIO & file) const{
				file.PutVar(text);
				file.PutVar(fpOffset);
				file.PutVar(line);
			}
	private:
		U8 text;		//index into StringTable of where identifier begins
		S8 fpOffset;	//+n or -n from $FP
		U4 line;		//source code line containing declaration
	};
	template<class __Platform>struct ReturnValue:public StackFrame<__Platform>
	{
		static const SymType = 2;
		~ReturnValue(){}
		ReturnValue(){}
		ReturnValue(U8 txt,S8 offset,U4 ln):StackFrame<__Platform>(txt,offset,ln){}
		template<class __FileIO>
			explicit ReturnValue(const __FileIO & file):StackFrame<__Platform>(file){}
	};
	template<class __Platform>struct Argument:public StackFrame<__Platform>
	{
		static const SymType = 3;
		~Argument(){}
		Argument(){}
		Argument(U8 txt,S8 offset,U4 ln):StackFrame<__Platform>(txt,offset,ln){}
		template<class __FileIO>
			explicit Argument(const __FileIO & file):StackFrame<__Platform>(file){}
	};
	template<class __Platform>struct LocalVariable:public StackFrame<__Platform>
	{
		static const SymType = 4;
		~LocalVariable(){}
		LocalVariable(){}
		LocalVariable(U8 txt,S8 offset,U4 ln):StackFrame<__Platform>(txt,offset,ln){}
		template<class __FileIO>
			explicit LocalVariable(const __FileIO & file):StackFrame<__Platform>(file){}
	};

}//namespace DoZerg

#endif