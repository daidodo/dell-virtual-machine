#ifndef LABEL_H
#define LABEL_H

namespace DoZerg{
	template<class __Platform>class Label
	{
	public:
		typedef typename __Platform::U4 U4;
		typedef typename __Platform::U8 U8;
	public:
		static const SymType = 5;
		static const TotalBytes = 20;
	public:
		~Label(){}
		template<class __FileIO>
			explicit Label(const __FileIO & file){Read(file);}
		Label(U8 txt,U8 addr,U4 ln)
			:text(txt)
			,address(addr)
			,line(ln)
		{}
		U8 Text() const{return text;}
		U8 Address() const{return address;}
		U4 Line() const{return line;}
		template<class __StrTbl,class __OutStream>
			void Print(const __StrTbl & strTbl,__OutStream & out) const{
				out<<"id	= "<<strTbl[text].c_str()
					<<"\n\t\t\taddress	= "<<address
					<<"\n\t\t\tline	= "<<line<<'\n';
			}
		template<class __FileIO>
			const Label & Read(const __FileIO & file){
				file.GetVar(text);
				file.GetVar(address);
				file.GetVar(line);
				return *this;
			}
		template<class __FileIO>
			void Write(const __FileIO & file) const{
				file.PutVar(text);
				file.PutVar(address);
				file.PutVar(line);
			}
	private:
		U8 text;		//index into StringTable of where identifier begins
		U8 address;		//address of label
		U4 line;		//source code line containing declaration
	};
}//namespace DoZerg

#endif