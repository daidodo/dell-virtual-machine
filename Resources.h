#ifndef RESOURCES_H
#define RESOURCES_H

#include "HeaderRecord.h"
#include "SymbolTable.h"
#include "FileIO.h"
#include "RawMemory.h"

namespace DoZerg{
#ifdef VM_DBG_RESOURCES
#define VM_DBG_RESOURCES0(str)				std::cout<<str;
#define VM_DBG_RESOURCES1(str,arg1)			std::printf(str,arg1);
#define VM_DBG_RESOURCES2(str,arg1,arg2)	std::cout<<str<<arg1<<arg2;
#define VM_DBG_RESOURCES_PU8(str,arg1,arg2)	std::cout<<str;__Platform::pU8(arg1);std::cout<<arg2;
#define VM_DBG_RESOURCES_PS8(str,arg1,arg2)	std::cout<<str;__Platform::pS8(arg1);std::cout<<arg2;
#else
#define VM_DBG_RESOURCES0(str)
#define VM_DBG_RESOURCES1(str,arg1)
#define VM_DBG_RESOURCES2(str,arg1,arg2)
#define VM_DBG_RESOURCES_PU8(str,arg1,arg2)
#define VM_DBG_RESOURCES_PS8(str,arg1,arg2)
#endif
	template<
		class __Platform,
		class __ErrFile
	>class Resources
	{
	public:
		typedef typename __Platform::U1 U1;
		typedef typename __Platform::U8 U8;
		typedef typename __Platform::S1 S1;
		typedef typename __Platform::S2 S2;
		typedef typename __Platform::S4 S4;
		typedef typename __Platform::S8 S8;
		typedef typename __Platform::F4 F4;
		typedef typename __Platform::F8 F8;
		typedef HeaderRecord<__Platform> __Header;
		typedef SymbolTable<__Platform> __SymTbl;
		typedef std::vector<std::string> __ProgramArg;
	private:
		typedef U8	__IntReg;
		typedef F4	__FltReg;
		typedef F8	__DblReg;
		typedef __IntReg & __IRRef;
		typedef __DblReg & __DRRef;
		typedef std::map<std::string,int> __Map;
		typedef RawMemory<__Platform> __Mem;
		typedef	Resources<__Platform,__ErrFile> __Myt;
	public:
		typedef typename __Map::const_iterator __CIter;
	private:
		static const IRegCount = 32;
		static const FRegCount = 10;
		static const DRegCount = 10;
		static const std::string RiName[IRegCount];
		static const std::string RfName[FRegCount];
		static const std::string RdName[DRegCount];
	public:
		static const BadReg = -1;
	public:
		__IntReg Ri[IRegCount];
		__FltReg Rf[FRegCount];
		__DblReg Rd[DRegCount];
		__IRRef IP,SP,FP,BE,HS,HE,SS,TOP,R1,R2,R3,R4,R5,R6,R7,R8;
		__DRRef D1,D2,D3;
		__Mem RAM;
	public:
		~Resources(){}
		explicit Resources(const __ErrFile & err)
			:IP(Ri[0]),SP(Ri[1])
			,FP(Ri[2]),BE(Ri[3])
			,HS(Ri[4]),HE(Ri[5])
			,SS(Ri[6]),TOP(Ri[7])
			,R1(Ri[8]),R2(Ri[9]),R3(Ri[10]),R4(Ri[11])
			,R5(Ri[12]),R6(Ri[13]),R7(Ri[14]),R8(Ri[15])
			,D1(Rd[0]),D2(Rd[1]),D3(Rd[2])
			,errFile(err)
		{InitRegDictionary();}
		__SymTbl & SymTbl(){return symTbl_;}
		template<class __CmdLine>
			bool Initialize(const __CmdLine & cmdLine){
				VM_DBG_RESOURCES0("Resources::Initialize(): 1) open executable file.\n");
				U8 fileSize = __Platform::GetFileSize(cmdLine.FileName());
				if(!fileSize){
					errFile<<BeginError
						<<"executable has zero size\n"
						<<EndError;
					return false;
				}
				FileIO<__Platform> file(cmdLine.FileName());
				if(!file.IsOpen()){
					errFile<<BeginError
						<<"cannot open "<<cmdLine.FileName()<<'\n'
						<<EndError;
					return false;
				}
				VM_DBG_RESOURCES0("Resources::Initialize(): 2) read file header and validate header data.\n");
				header_.Read(file);
				if(!header_.IsHecExe()){
					errFile<<BeginError
						<<"file not VM executable\n"
						<<EndError;
					return false;
				}
#ifdef VM_DBG_RESOURCES
				header_.Print(std::cout);
#endif
				if(!header_.ByteCodeSize()){
					errFile<<BeginError
						<<"no bytecode in "<<cmdLine.FileName()<<'\n'
						<<EndError;
					return false;
				}
				if(header_.SymTblSize() || header_.StrTblSize()){
					VM_DBG_RESOURCES0("Resources::Initialize(): 3) read symbols.\n");
					symTbl_.Read(file);
#ifdef VM_DBG_SYMBALS
					symTbl_.Print(std::cout);
#endif
				}else{
					VM_DBG_RESOURCES0("Resources::Initialize(): 3) read symbols(OMITTED).\n");
				}
				VM_DBG_RESOURCES0("Resources::Initialize(): 4) use header data to allocate RAM.\n");
				U8 bytecodeStart = header_.ByteCodeStart();
				U8 bytecodeEnd = header_.ByteCodeEnd();
				VM_DBG_RESOURCES2("Resources::Initialize(): bytecodeStart in file = ",bytecodeStart,"\n");
				VM_DBG_RESOURCES2("Resources::Initialize(): bytecodeEnd in file =",bytecodeEnd,"\n");
				U8 exec_bytes = header_.ByteCodeSize();
				U8 total_bytes = exec_bytes + cmdLine.HeapSize() + cmdLine.StackSize();
				U8 free_bytes = __Platform::GetAvailableMemory();
				if(total_bytes > free_bytes){
					errFile<<BeginError
						<<"not enough memory to init runtime\n"
						<<EndError;
					return false;
				}
				VM_DBG_RESOURCES2("Resources::Initialize(): available physical RAM = ",free_bytes,"\n");
				VM_DBG_RESOURCES2("Resources::Initialize(): file size = ",fileSize,"\n");
				VM_DBG_RESOURCES2("Resources::Initialize(): size of bytecode in file = ",exec_bytes,"\n");
				VM_DBG_RESOURCES2("Resources::Initialize(): heap bytes = ",cmdLine.HeapSize(),"\n");
				VM_DBG_RESOURCES2("Resources::Initialize(): stack bytes = ",cmdLine.StackSize(),"\n");
				VM_DBG_RESOURCES2("Resources::Initialize(): total required bytes = ",total_bytes,"\n");
				//allocate RAM
				if(!RAM.Allocate(total_bytes)){
					errFile<<BeginError
						<<"allocating memory for RAM with size "<<total_bytes<<" failed\n"
						<<EndError;
					return false;
				}
#ifdef VM_DBG_RESOURCES
				std::cout<<"Resources::RAM allocated ";__Platform::pU8(total_bytes);
				std::cout<<" bytes\n";
#endif
				VM_DBG_RESOURCES0("Resources::Initialize(): 5) set up registers.\n");
				//memory layout:[0 -> BE] [HS -> HE] [SS -> TOP]
				IP = 0;
				SP = total_bytes - 1;
				FP = 0;
				BE = exec_bytes - 1;
				HS = exec_bytes;
				HE = exec_bytes + cmdLine.HeapSize() - 1;
				SS = exec_bytes + cmdLine.HeapSize();
				TOP = total_bytes - 1;
				VM_DBG_RESOURCES0("Resources::Initialize(): 6) load bytecode to RAM.\n");
				file.Rewind();		//rewind to start of file
				if(file.Seek(bytecodeStart)){
					errFile<<BeginError
						<<"could not find bytecode\n"
						<<EndError;
					return false;
				}
				for(U8 i = 0,index = bytecodeStart;index <= bytecodeEnd;++index,++i){
					int read_byte = file.GetChar();
					if(read_byte == FileIO<__Platform>::EndOfFile){
						errFile<<BeginError
							<<"error reading bytecode file\n"
							<<EndError;
						return false;
					}
					RAM[i] = U1(read_byte);
				}
				VM_DBG_RESOURCES0("Resources::Initialize(): 7) set debug mode.\n");
				DebugModeOn = cmdLine.Debug();
				VM_DBG_RESOURCES0("Resources::Initialize(): 8) get program arguments.\n");
				progArgs_ = cmdLine.ProgramArgs();
				return true;
			}
		void DeallocateRAM(){
			RAM.Deallocate();
			VM_DBG_RESOURCES0("RAM deallocated\n");
		}
		bool CheckAddress(U8 addr,U8 cur) const{
			if(addr <= TOP)
				return true;
			errFile<<BeginError
				<<"invalid address literal "<<addr<<" at memory location "<<cur<<'\n'
				<<EndError;
			return false;
		}
		//used for to Handle instructions
		int OpCode() const{return RAM[IP];}			//返回当前IP的指令代码
		U8 & OpRi(int n){return Ri[RAM[IP + n]];}	//把(IP + n)的代码转换成所指的register
		F4 & OpRf(int n){return Rf[RAM[IP + n]];}
		F8 & OpRd(int n){return Rd[RAM[IP + n]];}
		const char * OpRiName(int n){return RiName[RAM[IP + n]].c_str();}
		const char * OpRfName(int n){return RfName[RAM[IP + n]].c_str();}
		const char * OpRdName(int n){return RdName[RAM[IP + n]].c_str();}
		template<typename __Type>
			__Type & LeftType(int n){				//把RAM[IP + n]的代码转换成相应的数值类型的引用(用作左值)
				return RAM.Oprand<__Type>(IP + n);
			}
		template<typename __Type>
			__Type RightType(int n) const{			//把RAM[IP + n]的代码转换成相应的数值类型的值(用作右值)
				return RAM.Oprand<__Type>(IP + n);
			}
		template<typename __Type>
			__Type & LeftRiType(int n){				//以OpRi(n)里的值为地址把代码转换成相应的数值类型的引用(用作左值)
				return RAM.Oprand<__Type>(Ri[RAM[IP + n]]);
			}
		template<typename __Type>
			__Type RightRiType(int n) const{		//以OpRi(n)里的值为地址把代码转换成相应的数值类型的值(用作右值)
				return RAM.Oprand<__Type>(Ri[RAM[IP + n]]);
			}
		template<typename __Type>
			__Type TopStack() const{return RAM.Oprand<__Type>(SP);}
		template<typename __Type,typename __TypeVal>
			bool PushStack(__TypeVal val){
				SP -= sizeof(__Type);
				if(!CheckStack())
					return false;
				RAM.Oprand<__Type>(SP) = __Type(val);
				return true;
			}
		template<typename __Type,typename __TypeVal>
			bool PopStack(__TypeVal & val){
				val = RAM.Oprand<__Type>(SP);
				SP += sizeof(__Type);
				return CheckStack();
			}
		template<typename __Type>
			__Type * RAMPosition(U8 pos) const{return RAM.Pointer<__Type>(pos);}
		U1 * RAMPosition(U8 pos) const{return RAM.Pointer(pos);}
		//处理中断
		bool HandleInterruption(int vec){return __Platform::HandleInterrupt(*this,vec);}
		void EnableInterrupt() const{__Platform::SetInterruptOn(true);}
		void DisableInterrupt() const{__Platform::SetInterruptOn(false);}
		//命令参数
		U8 ArgumentCount() const{return progArgs_.size();}		//number of arguments
		const char * GetArgument(U8 index) const{return progArgs_[__ProgramArg::size_type(index)].c_str();}
		U8 ArgumentSize(U8 index) const{return progArgs_[__ProgramArg::size_type(index)].length();}	//length of an argument
		//used for to Reformat instructions
		/*COMMENTS:				reformat the bytecode,from RAM[0] to RAM[HS - 1]
		I	instruction
		R	integer register
		F	float register
		D	double register
		a	address(U8)
		o	offset(S8)
		b	byte oprand
		w	word oprand
		d	dword oprand
		q	qword oprand
		f	float or double oprand
		*/
		template<typename __Type>
			void Bytecode2Type(U8 cur){
				__Platform::Bytecode2Type<__Type>(RAMPosition(cur));
			}
		bool Reformat_I(U8 & cur){
			const CodeBytes = 1;
			if(!CheckCodeEnd(cur,CodeBytes))
				return false;
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IR(U8 & cur){
			const CodeBytes = 2;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IF(U8 & cur){
			const CodeBytes = 2;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRf(cur + 1))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RfName[RAM[cur + 1]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IFD(U8 & cur){
			const CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRf(cur + 1) || !CheckRd(cur + 2))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RfName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RdName[RAM[cur + 2]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IFF(U8 & cur){
			const CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRf(cur + 1) || !CheckRf(cur + 2))
				return false;
			if(RAM[cur + 1] == RAM[cur + 2]){
				int reg = RAM[cur + 1];
				errFile<<BeginError
					<<"two float registers both are "<<RfName[reg].c_str()<<"("<<reg<<") at address "<<cur<<'\n'
					<<EndError;
				return false;
			}
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RfName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RfName[RAM[cur + 2]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IFFF(U8 & cur){
			const CodeBytes = 4;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRf(cur + 1) || !CheckRf(cur + 2) || !CheckRf(cur + 3))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RfName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RfName[RAM[cur + 2]].c_str());
			VM_DBG_RESOURCES1("oprand 3:\t%s\n",RfName[RAM[cur + 3]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_ID(U8 & cur){
			const CodeBytes = 2;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRd(cur + 1))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RdName[RAM[cur + 1]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IDD(U8 & cur){
			const CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRd(cur + 1) || !CheckRd(cur + 2))
				return false;
			if(RAM[cur + 1] == RAM[cur + 2]){
				int reg = RAM[cur + 1];
				errFile<<BeginError
					<<"two double registers both are "<<RfName[reg].c_str()<<"("<<reg<<") at address "<<cur<<'\n'
					<<EndError;
				return false;
			}
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RdName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RdName[RAM[cur + 2]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IDDD(U8 & cur){
			const CodeBytes = 4;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRd(cur + 1) || !CheckRd(cur + 2) || !CheckRd(cur + 3))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RdName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RdName[RAM[cur + 2]].c_str());
			VM_DBG_RESOURCES1("oprand 3:\t%s\n",RdName[RAM[cur + 3]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IDF(U8 & cur){
			const CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRd(cur + 1) || !CheckRf(cur + 2))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RdName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RfName[RAM[cur + 2]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_Ib(U8 & cur){
			const int CodeBytes = 2;
			if(!CheckCodeEnd(cur,CodeBytes))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%d\n",S1(RAM[cur + 1]));
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IRb(U8 & cur){
			const int CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%d\n",S1(RAM[cur + 2]));
			cur += CodeBytes;
			return true;
		}
		template<typename __Type>
			bool Reformat_IRi(U8 & cur){
				const int CodeBytes = 2 + sizeof(__Type);
				if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1))
					return false;
				Bytecode2Type<__Type>(cur + 2);
				VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
				VM_DBG_RESOURCES2("oprand 2:\t",RAM.Oprand<__Type>(cur + 2),"\n");
				cur += CodeBytes;
				return true;
			}
		bool Reformat_IRa(U8 & cur){
			const CodeBytes = 10;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1))
				return false;
			Bytecode2Type<U8>(cur + 2);
			if(!CheckAddress(RAM.Oprand<U8>(cur + 2),cur + 2))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES_PU8("oprand 2:\t",RAM.Oprand<U8>(cur + 2),"\n");
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IFf(U8 & cur){
			const CodeBytes = 6;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRf(cur + 1))
				return false;
			Bytecode2Type<F4>(cur + 2);
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RfName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES2("oprand 2:\t",RAM.Oprand<F4>(cur + 2),"\n");
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IDf(U8 & cur){
			const CodeBytes = 10;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRd(cur + 1))
				return false;
			Bytecode2Type<F8>(cur + 2);
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RdName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES2("oprand 2:\t",RAM.Oprand<F8>(cur + 2),"\n");
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IRRo(U8 & cur){
			const CodeBytes = 11;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1) || !CheckRi(cur + 2))
				return false;
			Bytecode2Type<S8>(cur + 3);
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RiName[RAM[cur + 2]].c_str());
			VM_DBG_RESOURCES_PS8("oprand 3:\t",RAM.Oprand<S8>(cur + 3),"\n");
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IRR(U8 & cur){
			const int CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1) || !CheckRi(cur + 2))
				return false;
			if(RAM[cur + 1] == RAM[cur + 2]){
				int reg = RAM[cur + 1];
				errFile<<BeginError
					<<"two integer registers both are "<<RiName[reg].c_str()<<"("<<reg<<") at address "<<cur<<'\n'
					<<EndError;
				return false;
			}
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RiName[RAM[cur + 2]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IRRR(U8 & cur){
			const int CodeBytes = 4;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1) || !CheckRi(cur + 2) || !CheckRi(cur + 3))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RiName[RAM[cur + 2]].c_str());
			VM_DBG_RESOURCES1("oprand 3:\t%s\n",RiName[RAM[cur + 3]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IFR(U8 & cur){
			const int CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRf(cur + 1) || !CheckRi(cur + 2))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RfName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RiName[RAM[cur + 2]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IDR(U8 & cur){
			const int CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRd(cur + 1) || !CheckRi(cur + 2))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RdName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RiName[RAM[cur + 2]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IRF(U8 & cur){
			const int CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1) || !CheckRf(cur + 2))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RfName[RAM[cur + 2]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IRD(U8 & cur){
			const int CodeBytes = 3;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1) || !CheckRd(cur + 2))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RdName[RAM[cur + 2]].c_str());
			cur += CodeBytes;
			return true;
		}
		bool Reformat_IRRRR(U8 & cur){
			const int CodeBytes = 5;
			if(!CheckCodeEnd(cur,CodeBytes) || !CheckRi(cur + 1) || !CheckRi(cur + 2) || !CheckRi(cur + 3) || !CheckRi(cur + 4))
				return false;
			VM_DBG_RESOURCES1("oprand 1:\t%s\n",RiName[RAM[cur + 1]].c_str());
			VM_DBG_RESOURCES1("oprand 2:\t%s\n",RiName[RAM[cur + 2]].c_str());
			VM_DBG_RESOURCES1("oprand 3:\t%s\n",RiName[RAM[cur + 3]].c_str());
			VM_DBG_RESOURCES1("oprand 4:\t%s\n",RiName[RAM[cur + 4]].c_str());
			cur += CodeBytes;
			return true;
		}
		//Pass
		int FineRi(const std::string & name) const{
			__CIter where = mapRi_.find(name);
			if(where == mapRi_.end())
				return BadReg;
			return where->second;
		}
		int FineRf(const std::string & name) const{
			__CIter where = mapRf_.find(name);
			if(where == mapRf_.end())
				return BadReg;
			return where->second;
		}
		int FineRd(const std::string & name) const{
			__CIter where = mapRd_.find(name);
			if(where == mapRd_.end())
				return BadReg;
			return where->second;
		}
		template<class __OutStream>
			void PrintSymbol(__OutStream & out,U8 byteSize) const{
				__Header(symTbl_.TotalBytes(),symTbl_.StrTblSize(),byteSize).Print(out);
				symTbl_.Print(out);
			}
	private:
		void InitRegDictionary(){
			for(int i = 0;i < IRegCount;++i)
				mapRi_.insert(std::make_pair(RiName[i],i));
			for(int i = 0;i < FRegCount;++i)
				mapRf_.insert(std::make_pair(RfName[i],i));
			for(int i = 0;i < DRegCount;++i)
				mapRd_.insert(std::make_pair(RdName[i],i));
		}
		bool AllocateRAM(U8 size){
			assert(RAM == 0 && "Resources::Allocate()");
			RAM = new(std::nothrow) U1[std::size_t(size)];
			return RAM != 0;
		}
		bool CheckCodeEnd(U8 cur,int off) const{
			if(cur + off <= HS)
				return true;
			errFile<<BeginError
				<<"incomplite instruction at address "<<cur<<'\n'
				<<EndError;
			return false;
		}
		bool CheckRi(U8 cur) const{
			if(RAM[cur] < IRegCount)
				return true;
			errFile<<BeginError
				<<"bad integer register "<<int(RAM[cur])<<" at address "<<cur<<'\n'
				<<EndError;
			return false;
		}
		bool CheckRf(U8 cur) const{
			if(RAM[cur] < FRegCount)
				return true;
			errFile<<BeginError
				<<"bad float register "<<int(RAM[cur])<<" at address "<<cur<<'\n'
				<<EndError;
			return false;
		}
		bool CheckRd(U8 cur) const{
			if(RAM[cur] < DRegCount)
				return true;
			errFile<<BeginError
				<<"bad double register "<<int(RAM[cur])<<" at address "<<cur<<'\n'
				<<EndError;
			return false;
		}
		bool CheckStack() const{
			if(SP <= HE){
				errFile<<BeginError
					<<"stack SP("<<SP<<") overflow into heap at address "<<IP<<'\n'
					<<EndError;
				return false;
			}else if(SP > TOP){
				errFile<<BeginError
					<<"stack SP("<<SP<<") underflow beyond TOP at address "<<IP<<'\n'
					<<EndError;
				return false;
			}
			return true;
		}
	public:
		const __ErrFile & errFile;
		int DebugModeOn;		//debug mode
	private:
		__Header header_;		//header info
		__SymTbl symTbl_;		//symbol info
		__ProgramArg progArgs_;	//program arguments
		__Map mapRi_,mapRf_,mapRd_;
	};
	template<class __Platform,class __ErrFile>
		const std::string Resources<__Platform,__ErrFile>::RiName[Resources<__Platform,__ErrFile>::IRegCount] = {
			"IP","SP","FP","BE","HS","HE","SS","TOP",
			"R1","R2","R3","R4","R5","R6","R7","R8",
			"R9","R10","R11","R12","R13","R14","R15","R16",
			"R17","R18","R19","R20","R21","R22","R23","R24"
		};
	template<class __Platform,class __ErrFile>
		const std::string Resources<__Platform,__ErrFile>::RfName[Resources<__Platform,__ErrFile>::FRegCount] = {
			"F1","F2","F3","F4","F5",
			"F6","F7","F8","F9","F10"
		};
	template<class __Platform,class __ErrFile>
		const std::string Resources<__Platform,__ErrFile>::RdName[Resources<__Platform,__ErrFile>::DRegCount] = {
			"D1","D2","D3","D4","D5",
			"D6","D7","D8","D9","D10"
		};
}//namespace DoZerg{

#endif