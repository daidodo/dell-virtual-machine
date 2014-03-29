#ifndef HEADRECORD_H
#define HEADRECORD_H

namespace DoZerg{
	template<class __Platform>class HeaderRecord
	{
	public:
		typedef typename __Platform::U2 U2;
		typedef typename __Platform::U8 U8;
	public:
		static const TotalBytes = 26;
	private:
		static const U2 ApplicationCode = 0xDEED;
		static const U2 BadAppCode = 0xBAD;
	public:
		~HeaderRecord(){}
		HeaderRecord(){}
		HeaderRecord(U8 szSymTbl,U8 szStrTbl,U8 szByteCode)
			:magic(ApplicationCode)
			,szSymTbl_(szSymTbl)
			,szStrTbl_(szStrTbl)
			,szByteCode_(szByteCode)
		{}
		U2 Magic() const{return magic;}
		U8 SymTblSize() const{return szSymTbl_;}
		U8 StrTblSize() const{return szStrTbl_;}
		U8 ByteCodeSize() const{return szByteCode_;}
		U8 ByteCodeStart() const{return TotalBytes + szSymTbl_ + szStrTbl_;}
		U8 ByteCodeEnd() const{return ByteCodeStart() + szByteCode_ - 1;}
		template<class __OutStream>
			void Print(__OutStream & out) const{
				char tmp[5];
				std::sprintf(tmp,"%X",magic);
				out<<"Header Record:\n"
					<<"\tmagic	= "<<tmp
					<<"\n\tSymTbl size	 = "<<szSymTbl_
					<<"\n\tStrTbl size	 = "<<szStrTbl_
					<<"\n\tByteCode size	 = "<<szByteCode_
					<<"\nEnd Header Record\n";
			}
		template<class __FileIO>
			const HeaderRecord & Read(const __FileIO & file){	//read header info from file
				file.GetVar(magic);
				if(magic != ApplicationCode)
					magic = BadAppCode;
				file.GetVar(szSymTbl_);
				file.GetVar(szStrTbl_);
				file.GetVar(szByteCode_);
				return *this;
			}
		template<class __FileIO>
			void Write(const __FileIO & file) const{			//write header info into file
				file.PutVar(ApplicationCode);
				file.PutVar(szSymTbl_);
				file.PutVar(szStrTbl_);
				file.PutVar(szByteCode_);
			}
		bool IsHecExe() const{
			return magic == ApplicationCode;
		}
	private:
		U2 magic;		//file type label
		U8 szSymTbl_;	//len of SymTbl
		U8 szStrTbl_;	//len of StrTbl
		U8 szByteCode_;	//len of ByteCode
	};
}//namespace DoZerg

#endif