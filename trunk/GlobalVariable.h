#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H

namespace DoZerg{
	template<class __Platform>class GlobalVariable
	{
	public:
		typedef typename __Platform::U1 U1;
		typedef typename __Platform::U4 U4;
		typedef typename __Platform::U8 U8;
		typedef typename __Platform::S8 S8;
	public:
		static const SymType = 0;
		static const TotalBytes = 37;
		static const U1 SZ_BYTE = 1;
		static const U1 SZ_WORD = 2;
		static const U1 SZ_DWORD = 4;
		static const U1 SZ_QWORD = 8;
	private:
		static const char * SIZE_NAME[10];
		static U8 TotalOffSet;
	public:
		~GlobalVariable(){}
		GlobalVariable(U8 txt,U1 type,U8 length,U4 sourceline)
			:text(txt)
			,dType(type)
			,len(length)
			,size(len * dType)
			,offset(TotalOffSet += size)
			,line(sourceline)
		{}
		template<class __FileIO>
			explicit GlobalVariable(const __FileIO & file){Read(file);}
		U8 Text() const{return text;}
		U1 DType() const{return dType;}
		U8 Len() const{return len;}
		U8 Size() const{return size;}
		S8 Offset() const{return offset;}
		U4 Line() const{return line;}
		template<class __StrTbl,class __OutStream>
			void Print(const __StrTbl & strTbl,__OutStream & out) const{
				out<<"Global Variable:\n"
					<<"\t\tidentifier	= "<<strTbl[text].c_str()
					<<"\n\t\tdata type	= "<<SIZE_NAME[dType]
					<<"\n\t\tlength	= "<<len
					<<"\n\t\tsize	= "<<size
					<<"\n\t\toffset	= "<<(-S8(offset))
					<<"\n\t\tline	= "<<line<<'\n';
			}
		template<class __FileIO>
			const GlobalVariable & Read(const __FileIO & file){
				file.GetVar(text);
				file.GetVar(dType);
				file.GetVar(len);
				file.GetVar(size);
				file.GetVar(offset);
				file.GetVar(line);
				return *this;
			}
		template<class __FileIO>
			void Write(const __FileIO & file) const{
				file.PutVar(text);
				file.PutVar(dType);
				file.PutVar(len);
				file.PutVar(size);
				file.PutVar(offset);
				file.PutVar(line);
			}
	private:
		U8 text;		//index to StrTbl of where identifier begins
		U1 dType;		//SZ_BYTE,SZ_WORD,SZ_DWORD,SZ_QWORD
		U8 len;			//# elements if array
		U8 size;		//total byte size
		S8 offset;		//offset below $TOP,address(g) = $TOP - offset
		U4 line;		//source code line containing declaration
	};
	template<class __Platform>
		const char * GlobalVariable<__Platform>::SIZE_NAME[10] = {"","SZ_BYTE","SZ_WORD","","SZ_DWORD","","","","SZ_QWORD"};
	template<class __Platform>
		typename GlobalVariable<__Platform>::U8
		GlobalVariable<__Platform>::TotalOffSet = 0;
}//namespace DoZerg

#endif