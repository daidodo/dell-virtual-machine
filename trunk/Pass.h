#ifndef PASS_H
#define PASS_H

#include "Line.h"
#include "ProgramSection.h"
#include "FileIO.h"

namespace DoZerg{
#ifdef VM_DBG_PASS
#define VM_DBG_PASS0(str)			std::cout<<"Pass::"<<str;
#define VM_DBG_PASS1(str,arg1)		std::cout<<"Pass::";std::printf(str,arg1);
#define VM_DBG_PASS2(str,arg1,arg2)	std::cout<<"Pass::";std::printf(str,arg1,arg2);
#else
#define VM_DBG_PASS0(str)
#define VM_DBG_PASS1(str,arg1)
#define VM_DBG_PASS2(str,arg1,arg2)
#endif
	template<
		class __Platform,
		class __CmdLine,
		class __Res
	>class Pass
	{
	public:
		typedef typename __Platform::U8 U8;
	private:
		typedef typename __Platform::U1 U1;
		typedef typename __Platform::U4 U4;
		typedef typename __Platform::S1 S1;
		typedef typename __Platform::S2 S2;
		typedef typename __Platform::S4 S4;
		typedef typename __Platform::S8 S8;
		typedef typename __Platform::F4 F4;
		typedef typename __Platform::F8 F8;
		typedef typename __CmdLine::__ErrCount __ErrCount;
		typedef FileIO<__Platform> __FIO;
		typedef typename __Res::__Header __Header;
		typedef typename __Res::__SymTbl __SymTbl;
		typedef typename __SymTbl::__GVar __GVar;
	public:
		typedef Line<__FIO> __Line;
	private:
		static const LineWidth = 40;
		static const ListCodeBase = 10;		//写入list file的代码基数(10 or 16),WriteTempListFile()
		enum PassEnum{First,Second};
	public:
		~Pass(){}
		explicit Pass(const __CmdLine & cmdline,__Res & res)
			:cmdline_(cmdline)
			,res_(res)
			,symTbl_(res.SymTbl())
			,bytePos_(0)
			,listFile_(true)	//don't report error if it is not opened 
		{}
		template<class __InsSet>
			void Scan(const __InsSet & ins){
				VM_DBG_PASS0("Scan(): 1) FIRST PASS\n");
				__FIO infile(cmdline_.InputFileName());
				if(!infile.IsOpen()){
					__ErrCount()<<"cannot open source file "<<cmdline_.InputFileName()<<'\n';
					return;
				}
				bytePos_ = 0;
				__Line line(infile);
				bool not_end;
				do{
					not_end = line.GetLine();
					ParseLine(First,ins,line);
				}while(not_end);
				if(!section_.Outside())
					__ErrCount()<<"no .PE for procedure,after "<<line;
				if(__ErrCount::CheckError(false) != 0)
					return;
				VM_DBG_PASS0("Scan(): 2) SECOND PASS\n");
				if(!tmpFile_.Open(cmdline_.TempFileName(),"wb")){
					__ErrCount()<<"cannot open tmp file "<<cmdline_.TempFileName()<<'\n';
					return;
				}
				if(cmdline_.Listing() && !listFile_.Open(cmdline_.ListFileName(),"w")){
					__ErrCount()<<"cannot open list file "<<cmdline_.ListFileName()<<'\n';
					return;
				}
				bytePos_ = 0;
				infile.Rewind();		//rewind to start of the file
				line.InitRow();
				do{
					not_end = line.GetLine();
					ParseLine(Second,ins,line);
				}while(not_end);
				GenerateSymbolSummary();
				tmpFile_.Close();
				listFile_.Close();
			}
			bool Build(){
				VM_DBG_PASS0("Build(): 1) open output file\n");
				__FIO outFile(cmdline_.OutputFileName(),"wb");
				if(!outFile.IsOpen()){
					__ErrCount()<<"cannot open output file "<<cmdline_.OutputFileName()<<'\n';
					return false;
				}
				VM_DBG_PASS0("Build(): 2) write header info\n");
				U8 size_byte = bytePos_;
				if(cmdline_.OmitDebugData()){
					__Header(0,0,size_byte).Write(outFile);
					VM_DBG_PASS0("Build(): 3) commit symbol tables to output file(OMITTED)\n");
				}else{
					U8 size_symtbl = symTbl_.TotalBytes();
					U8 size_strtbl = symTbl_.StrTblSize();
					__Header(size_symtbl,size_strtbl,size_byte).Write(outFile);
					VM_DBG_PASS0("Build(): 3) commit symbol tables to output file\n");
					symTbl_.Write(outFile);
				}
				VM_DBG_PASS0("Build(): 4) append bytecode in temp to output file\n");
				return AppendTempFile(outFile);
			}
		//Pass2
		//Pass instructions
		bool Process_I(const __Line & line,int ins){
			WriteListFile(line);
			U1 encode = U1(ins);
			WriteTempListFile(&encode,1);
			++bytePos_;
			return true;
		}
		bool Process_IR(const __Line & line,int ins){
			WriteListFile(line);
			int reg = res_.FineRi(line[1]);
			if(reg == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 2;
			U1 encode[len] = {U1(ins),U1(reg)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IF(const __Line & line,int ins){
			WriteListFile(line);
			int reg = res_.FineRf(line[1]);
			if(reg == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 2;
			U1 encode[len] = {U1(ins),U1(reg)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IFD(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRf(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRd(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IFF(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRf(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRf(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			if(reg1 == reg2){
				__ErrCount()<<line[1]<<" and "<<line[2]<<" are the same float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IFFF(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRf(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRf(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg3 = res_.FineRf(line[3]);
			if(reg3 == __Res::BadReg){
				__ErrCount()<<line[3]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 4;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2),U1(reg3)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_ID(const __Line & line,int ins){
			WriteListFile(line);
			int reg = res_.FineRd(line[1]);
			if(reg == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 2;
			U1 encode[len] = {U1(ins),U1(reg)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IDD(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRd(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRd(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			if(reg1 == reg2){
				__ErrCount()<<line[1]<<" and "<<line[2]<<" are the same double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IDDD(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRd(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRd(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg3 = res_.FineRd(line[3]);
			if(reg3 == __Res::BadReg){
				__ErrCount()<<line[3]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 4;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2),U1(reg3)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IDF(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRd(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRf(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_Ib(const __Line & line,int ins){
			WriteListFile(line);
			int oprand;
			if(!line[1].MatchUnsignedInteger(oprand)){
				__ErrCount()<<line[1]<<" is not an unsigned integer constant,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 2;
			U1 encode[len] = {U1(ins),U1(oprand)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IRb(const __Line & line,int ins){return IRi<S1>(line,ins);}
		bool Process_IRw(const __Line & line,int ins){return IRi<S2>(line,ins);}
		bool Process_IRd(const __Line & line,int ins){return IRi<S4>(line,ins);}
		bool Process_IRq(const __Line & line,int ins){return IRi<S8>(line,ins);}
		bool Process_LAD(const __Line & line,int ins){
			WriteListFile(line);
			int reg = res_.FineRi(line[1]);
			if(reg == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			if(!line[2].MatchIdentifier()){
				__ErrCount()<<line[2]<<" is not a valid identifier,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			S8 oprand;
			__SymTbl::AddressEnum ret = symTbl_.QueryAddress(line[2],oprand);
			if(ret == __SymTbl::BadId){
				__ErrCount()<<line[2]<<" is not defined,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			if(ret == __SymTbl::Indirect){
				__ErrCount()<<"invalid oprand for LAD,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 2 + sizeof(S8);
			U1 encode[len] = {U1(ins),U1(reg)};
			__Platform::Type2Bytecode(oprand,encode + 2);
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_LAI(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRi(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRi(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			if(!line[3].MatchIdentifier()){
				__ErrCount()<<line[3]<<" is not a valid identifier,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			S8 oprand;
			__SymTbl::AddressEnum ret = symTbl_.QueryAddress(line[3],oprand);
			if(ret == __SymTbl::BadId){
				__ErrCount()<<line[3]<<" is not defined,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			if(ret == __SymTbl::Direct){
				__ErrCount()<<"invalid oprand for LAI,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3 + sizeof(S8);
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			__Platform::Type2Bytecode(oprand,encode + 3);
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IFf(const __Line & line,int ins){
			WriteListFile(line);
			int reg = res_.FineRf(line[1]);
			if(reg == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			F4 oprand;
			if(!line[2].MatchFloatValue(oprand)){
				__ErrCount()<<line[2]<<" is not a float constant,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const len = 2 + sizeof(F4);
			U1 encode[len] = {U1(ins),U1(reg)};
			__Platform::Type2Bytecode(oprand,encode + 2);
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IDf(const __Line & line,int ins){
			WriteListFile(line);
			int reg = res_.FineRd(line[1]);
			if(reg == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			F8 oprand;
			if(!line[2].MatchFloatValue(oprand)){
				__ErrCount()<<line[2]<<" is not a float constant,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const len = 2 + sizeof(F8);
			U1 encode[len] = {U1(ins),U1(reg)};
			__Platform::Type2Bytecode(oprand,encode + 2);
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IRR(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRi(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRi(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			if(reg1 == reg2){
				__ErrCount()<<line[1]<<" and "<<line[2]<<" are the same integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IRRR(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRi(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRi(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg3 = res_.FineRi(line[3]);
			if(reg3 == __Res::BadReg){
				__ErrCount()<<line[3]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 4;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2),U1(reg3)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IFR(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRf(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRi(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IDR(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRd(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRi(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IRF(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRi(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRf(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid float register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IRD(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRi(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRd(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid double register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 3;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
		bool Process_IRRRR(const __Line & line,int ins){
			WriteListFile(line);
			int reg1 = res_.FineRi(line[1]);
			if(reg1 == __Res::BadReg){
				__ErrCount()<<line[1]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg2 = res_.FineRi(line[2]);
			if(reg2 == __Res::BadReg){
				__ErrCount()<<line[2]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg3 = res_.FineRi(line[3]);
			if(reg3 == __Res::BadReg){
				__ErrCount()<<line[3]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			int reg4 = res_.FineRi(line[4]);
			if(reg4 == __Res::BadReg){
				__ErrCount()<<line[4]<<" is not a valid integer register,"<<line;
				listFile_<<"ERROR\n";
				return false;
			}
			const int len = 5;
			U1 encode[len] = {U1(ins),U1(reg1),U1(reg2),U1(reg3),U1(reg4)};
			WriteTempListFile(encode,len);
			bytePos_ += len;
			return true;
		}
	private:
		template<class __InsSet>
			void ParseLine(PassEnum t,const __InsSet & ins,const __Line & line){
			/*line can be:
				i)	a directive(starts with a period,i.e.,.PE)
				ii)	an instruction(operand(s))
				iii)a comment(start with #)
			*/
#ifdef VM_DBG_PASS
				line.Print();
#endif
				if(line.Empty())
					return;
				switch(line[0][0]){
					case '.':{
						ProcessDirective(t,line);
						break;}
					case '#':{
						break;}
					default:{
						if(t == First && section_.Outside()){
							__ErrCount()<<"instructions must be in procedure,"<<line;
							return;
						}
						ProcessInstruction(t,ins,line);
					}
				}
			}
		void ProcessDirective(PassEnum t,const __Line & line){
			switch(line[0][1]){
				case 'G':{			//Global Directive
					if(t == First && section_ != ProgramSection::OutsideGlob){
						__ErrCount()<<"global directive must be in front of all functions,"<<line;
						return;
					}
					ProcessGD(t,line);
					break;}
				case 'P':{			//Procedure Directive
					ProcessPD(t,line);
					break;}
				default:
					__ErrCount()<<line[0]<<" is not a directive,"<<line;
			}
		}
		template<class __InsSet>
			void ProcessInstruction(PassEnum t,const __InsSet & ins,const __Line & line){
				if(t == Second){
					ins.Pass2(*this,line);
					return ;
				}
				if(!line.MatchSeperator(" ") && !line.MatchSeperator(" ,")){
					__ErrCount()<<"instruction with bad seperators,"<<line;
					return;
				}
				__InsSet::PassReturnEnum ret = ins.Pass1(line,bytePos_);
				if(ret == __InsSet::r_BadCommand)
					__ErrCount()<<"bad command "<<line[0]<<','<<line;
				else if(ret == __InsSet::r_Fail)
					__ErrCount()<<"instruction format error,"<<line;
			}
		void ProcessGD(PassEnum t,const __Line & line){
			/*
			FORM:	.GX identifier [integer]
				the [integer] modifier is optional
				so we get 2 correct ways:
				i)	.GX identifier
				ii)	.GX identifier integer
			*/
			if(t == Second){
				listFile_<<line.ToString().c_str()<<'\n';
				return;
			}
			if(line[0].size() != 3 || line.Size() < 2 || line.Size() > 3 || !line.MatchSeperator(" ")){
				__ErrCount()<<"bad global declaration,"<<line;
				return;
			}
			U1 dtype;
			switch(line[0][2]){
				case 'B':{
					dtype = __GVar::SZ_BYTE;
					break;}
				case 'W':{
					dtype = __GVar::SZ_WORD;
					break;}
				case 'D':{
					dtype = __GVar::SZ_DWORD;
					break;}
				case 'Q':{
					dtype = __GVar::SZ_QWORD;
					break;}
				default:{
					__ErrCount()<<line[0]<<" is invalid global data type,"<<line;
					return;
						}
			}
			if(!line[1].MatchIdentifier()){
				__ErrCount()<<line[1]<<" is not a identifier,"<<line;
				return;
			}
			U4 len = 1;
			if(line.Size() == 3 && !line[2].MatchUnsignedInteger(len)){
				__ErrCount()<<line[2]<<" is not a unsigned integer,"<<line;
				return;
			}
			if(symTbl_.Has(line[1])){
				__ErrCount()<<line[1]<<" re-defined,"<<line;
				return;
			}
			VM_DBG_PASS0("ProcessGD(): adding .GX to symbol table\n");
			symTbl_.AddGlobalVariable(line[1],dtype,len,line.Row());
		}
		void ProcessPD(PassEnum t,const __Line & line){
			/*
			FORM:	
				.PB identifier
				.PR identifier [+]integer
				.PA identifier [+]integer
				.PV identifier -integer
				.PL identifier
				.PE
			*/
			if(t == Second){
				if(line[0][2] == 'B')
					listFile_<<"#-------Procedure------------\n";
				listFile_<<line.ToString().c_str()<<'\n';
				if(line[0][2] == 'E')
					listFile_<<"#------End Procedure---------\n";
				return;
			}
			int len = line.Size();
			if(len > 3 || line[0].size() != 3 || !line.MatchSeperator(" ")){
				__ErrCount()<<"bad procedure directive,"<<line;
				return;
			}
			if(len == 1){		//.PE
				if(line[0][2] != 'E'){
					__ErrCount()<<"bad procedure directive,"<<line;
					return;
				}
				if(section_.Outside()){
					__ErrCount()<<".PE must partner with .PB,"<<line;
					return;
				}
				VM_DBG_PASS0("ProcessGD(): .PE found\n");
				section_ = ProgramSection::OutsideProc;
			}else if(len == 2){	//.PB,.PL
				if(line[0][2] != 'B' && line[0][2] != 'L'){
					__ErrCount()<<".PE must partner with .PB,"<<line;
					return;
				}
				if(!line[1].MatchIdentifier()){
					__ErrCount()<<line[1]<<" is not a identifier,"<<line;
					return;
				}
				if(symTbl_.Has(line[1])){
					__ErrCount()<<line[1]<<" re-defined,"<<line;
					return;
				}
				if(line[0][2] == 'B'){
					if(section_.Inside()){
						__ErrCount()<<"cannot nest procedures,"<<line;
						return;
					}
					VM_DBG_PASS0("ProcessGD(): adding .PB to symbol table\n");
					symTbl_.AddProcedure(line[1],bytePos_,line.Row());
					section_ = ProgramSection::InsideProcRet;
				}else{
					if(section_.Outside()){
						__ErrCount()<<"labels must be in procedures,"<<line;
						return;
					}
					VM_DBG_PASS0("ProcessPD(): adding .PL to symbol table\n");
					symTbl_.AddProcLabel(line[1],bytePos_,line.Row());
					listFile_<<line.ToString().c_str()<<'\n';
					section_ = ProgramSection::InsideProcIns;
				}
			}else{				//.PR,.PA,.PV
				if(line[0][2] != 'R' && line[0][2] != 'A' && line[0][2] != 'V'){
					__ErrCount()<<"bad procedure directive,"<<line;
					return;
				}
				if(!line[1].MatchIdentifier()){
					__ErrCount()<<line[1]<<" is not a identifier,"<<line;
					return;
				}
				if(symTbl_.Has(line[1])){
					__ErrCount()<<line[1]<<" re-defined,"<<line;
					return;
				}
				S4 offset;
				if(!line[2].MatchSignedInteger(offset)){
					__ErrCount()<<line[2]<<" is not a insteger,"<<line;
					return;
				}
				if(line[0][2] == 'R'){
					if(section_ != ProgramSection::InsideProcRet){
						__ErrCount()<<"return declaration must be after procedures declaration,"<<line;
						return;
					}
					VM_DBG_PASS0("ProcessPD(): adding .PR to symbol table\n");
					symTbl_.AddProcRet(line[1],offset,line.Row());
					section_ = ProgramSection::InsideProcArg;
				}else if(line[0][2] == 'A'){
					if(!section_.Arguments()){
						__ErrCount()<<"argument declarations must be after return/procedures declaration,"<<line;
						return;
					}
					VM_DBG_PASS0("ProcessPD(): adding .PA to symbol table\n");
					symTbl_.AddProcArg(line[1],offset,line.Row());
					section_ = ProgramSection::InsideProcArg;
				}else{
					if(!section_.Variable()){
						__ErrCount()<<"variable declarations must be after argument/return/procedures declaration,"<<line;
						return;
					}
					VM_DBG_PASS0("ProcessPD(): adding .PV to symbol table\n");
					symTbl_.AddProcVar(line[1],offset,line.Row());
					section_ = ProgramSection::InsideProcVar;
				}
			}
		}
		void WriteListFile(const __Line & line) const{		//write a instruction to list file
			listFile_<<line.ToString(bytePos_,LineWidth,ListCodeBase).c_str();
		}
		void WriteTempListFile(const U1 str[],int length) const{
			for(int i = 0;i < length;++i)
				tmpFile_<<str[i];
			if(!cmdline_.Listing())
				return;
			char tmp[12];
			for(int i = 0;i < length;++i){
				listFile_<<'['<<ToUppercase(::itoa(U4(str[i]),tmp,ListCodeBase),12)<<']';
			}
			listFile_<<'\n';
		}
		char * ToUppercase(char * str,int len) const{
			for(int i = 0;i < len;++i)
				if(str[i] >= 'a' && str[i] <= 'z')
					str[i] -= 32;
			return str;
		}
		template<typename __IntConst>
			bool IRi(const __Line & line,int ins){	//ins $Ri integer
				WriteListFile(line);
				int reg = res_.FineRi(line[1]);
				if(reg == __Res::BadReg){
					__ErrCount()<<line[1]<<" is not a valid integer register,"<<line;
					listFile_<<"ERROR\n";
					return false;
				}
				__IntConst oprand;
				if(!line[2].MatchIntegerValue(oprand)){
					__ErrCount()<<line[2]<<" is not a integer constant,"<<line;
					listFile_<<"ERROR\n";
					return false;
				}
				const int len = 2 + sizeof(__IntConst);
				U1 encode[len] = {U1(ins),U1(reg)};
				__Platform::Type2Bytecode(oprand,encode + 2);
				WriteTempListFile(encode,len);
				bytePos_ += len;
				return true;
			}
		void GenerateSymbolSummary() const{
			listFile_<<"\n--------------Symbol Summary-----------\n";
			res_.PrintSymbol(listFile_,bytePos_);
		}
		bool AppendTempFile(const __FIO & outfile){
			if(!tmpFile_.ReOpen()){
				__ErrCount()<<"cannot open temp file "<<tmpFile_.FileName()<<'\n';
				return false;
			}
			for(int ch = tmpFile_.GetChar();ch != __FIO::EndOfFile;ch = tmpFile_.GetChar())
				outfile<<char(ch);
			tmpFile_.Close();
			return true;
		}
	private:
		const __CmdLine & cmdline_;
		__Res & res_;
		__SymTbl & symTbl_;
		U8 bytePos_;				//current index of bytecode being generated
		ProgramSection section_;
		__FIO listFile_,tmpFile_;
	};
}//namespace DoZerg

#endif