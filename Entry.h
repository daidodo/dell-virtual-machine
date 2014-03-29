#ifndef ENTRY_H
#define ENTRY_H

namespace DoZerg{
	template<class __Platform>class Entry
	{
	public:
		typedef typename __Platform::U1 U1;
		typedef typename __Platform::U4 U4;
	private:
		static const char * SymTypeStr[11];
	public:
		~Entry(){}
		Entry(U1 t,U4 i,U4 si)
			:type(t)
			,index(i)
			,subIndex(si)
		{}
		U1 Type() const{return type;}
		U4 Index() const{return index;}
		U4 SubIndex() const{return subIndex;}
		template<class __OutStream>
			void Print(__OutStream & out) const{
				out<<"\ttype	= "<<SymTypeStr[type]
					<<"(index,subIndex) = ("<<index<<','<<subIndex<<")\n";
			}
	private:
		U1 type;		//GLOBAL_VAR,PROC,PROC_RET,PROC_ARG,PROC_LOC,PROC_LBL
		U4 index;		//index to globVar,proc arrays in symbTbl
		U4 subIndex;	//subindex for PROC_RET,...,PROC_LBL
	};
	template<class __Platform>
		const char * Entry<__Platform>::SymTypeStr[11] = {"GLOBAL_VAR","PROC","PROC_RET","PROC_ARG","PROC_LOC","PROC_LBL"};
}//namespace DoZerg

#endif