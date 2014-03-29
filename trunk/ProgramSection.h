#ifndef PROGRAMSECTOIN_H
#define PROGRAMSECTOIN_H

namespace DoZerg{
	class ProgramSection
	{
	public:
		enum Sectioins{			//Codes allowed:
			OutsideGlob,		//.GX,.PB
			InsideProcRet,		//.PR,.PA,.PV,ins,.PL,.PE
			InsideProcArg,		//.PA,.PV,ins,.PL,.PE
			InsideProcVar,		//.PV,ins,.PL,.PE
			InsideProcIns,		//ins,.PL,.PE
			OutsideProc,		//.PB
		};
	public:
		ProgramSection():sec_(OutsideGlob){}
		ProgramSection(Sectioins s):sec_(s){}
		~ProgramSection(){}
		void operator =(Sectioins s){sec_ = s;}
		bool operator ==(Sectioins s) const{return sec_ == s;}
		bool operator !=(Sectioins s) const{return sec_ != s;}
		bool Outside() const{return sec_ == OutsideGlob || sec_ == OutsideProc;}
		bool Inside() const{return !Outside();}
		bool Arguments() const{return sec_ == InsideProcRet || sec_ == InsideProcArg;}
		bool Variable() const{return sec_ == InsideProcRet || sec_ == InsideProcArg || sec_ == InsideProcVar;}
	private:
		Sectioins sec_;
	};
}//namespace DoZerg

#endif