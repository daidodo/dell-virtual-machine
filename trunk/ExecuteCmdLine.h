#ifndef EXECUTECMDLINE_H
#define EXECUTECMDLINE_H

#include <vector>
#include <string>
#include <iostream>

namespace DoZerg{
#ifdef VM_DBG_EXECMDLINE
#define VM_DBG_EXECMDLINE0(str)				std::cout<<"ExecuteCmdLine::HandleArguments(): "<<str;
#define VM_DBG_EXECMDLINE1(str,arg1)		std::cout<<"ExecuteCmdLine::HandleArguments(): ";std::printf(str,arg1);
#define VM_DBG_EXECMDLINE2(str,arg1,arg2)	std::cout<<"ExecuteCmdLine::HandleArguments(): ";std::printf(str,arg1,arg2);
#else
#define VM_DBG_EXECMDLINE0(str)
#define VM_DBG_EXECMDLINE1(str,arg1)
#define VM_DBG_EXECMDLINE2(str,arg1,arg2)
#endif
	template<class __Platform>class ExecuteCmdLine
	{
	public:
		typedef std::vector<std::string> __Arguments;
		typedef typename __Platform::U8 U8;
	private:
		static const U8 DefaultHeapSize = 64 * 1024;		//数据堆缺省长度Bytes
		static const U8 DefaultStackSize = DefaultHeapSize;	//堆栈缺省长度Bytes
	public:
		~ExecuteCmdLine(){}
		explicit ExecuteCmdLine()
			:heapRequest(DefaultHeapSize)
			,stackRequest(DefaultStackSize)
			,debug(0)
		{}
		const char * FileName() const{return binaryFile.c_str();}
		U8 HeapSize() const{return heapRequest;}
		U8 StackSize() const{return stackRequest;}
		int Debug() const{return debug;}
		const __Arguments & ProgramArgs() const{return programArgs_;}
		template<class __ErrFile>
		bool HandleArguments(const __ErrFile & errFile,const __Arguments & argv){
			__Arguments::size_type argc = argv.size();
			if(argc == 0){		//only 1 argument
				errFile<<BeginError
					<<"no arguments\n"
					<<EndError;
				PrintUsage();
				return false;
			}
			/*
			handle options:
			-h=XXX,-s=YYY,-d,-?
			argv[i][0] = '-'
			argv[i][1] = letter
			argv[i][2] = '='
			argv[i][3] = number
			*/
			__Arguments::size_type i;
			for(i = 0;i < argc && argv[i][0] == '-';++i){
				switch(argv[i][1]){
					case 'h':
					case 'H':{
						if(argv[i][2] != '='){
							errFile<<BeginError
								<<"incorrect \'-h\' switch\n"
								<<EndError;
							PrintUsage();
							return false;
						}
						char * stop;
						U8 t = ::strtol(&argv[i][3],&stop,10);
						if(t <= 0){
							errFile<<BeginError
								<<"bad heap request ("
								<<argv[i].c_str()
								<<")\nrequire positive integer\n"
								<<EndError;
							PrintUsage();
							return false;
						}
						heapRequest = t;
						VM_DBG_EXECMDLINE0("heap size set\n");
						break;}
					case 's':
					case 'S':{
						if(argv[i][2] != '='){
							errFile<<BeginError
								<<"incorrect \'-s\' switch\n"
								<<EndError;
							PrintUsage();
							return false;
						}
						char * stop;
						U8 t = ::strtol(&argv[i][3],&stop,10);
						if(t <= 0){
							errFile<<BeginError<<"bad stack request ("
								<<argv[i].c_str()
								<<")\nrequire positive integer\n"
								<<EndError;
							PrintUsage();
							return false;
						}
						stackRequest = t;
						VM_DBG_EXECMDLINE0("stack size set\n");
						break;}
					case 'd':
					case 'D':{
						debug = 1;
						VM_DBG_EXECMDLINE0("debug switch toggled\n");
						break;}
					case '?':{
						PrintUsage();
						return false;}
					default:{
						errFile<<BeginError<<"\'-"
							<<argv[i][1]
							<<"\' is invalid\n"
							<<EndError;
						PrintUsage();
						return false;
					}
				}
			}
			//set filename,check for .RUN extension
			if(i == argc){
				errFile<<BeginError
					<<"executable missing\n"
					<<EndError;
				PrintUsage();
				return false;
			}
			std::string::size_type len = argv[i].length();
			if(	len >= 4 &&
				argv[i][len - 4] == '.' &&
				argv[i][len - 3] == 'R' &&
				argv[i][len - 2] == 'U' &&
				argv[i][len - 1] == 'N')
			{
				binaryFile = argv[i];
				++i;
			}else{
				errFile<<BeginError
					<<"missing .RUN suffix\n"
					<<EndError;
				PrintUsage();
				return false;
			}
			for(;i < argc;++i)
				programArgs_.push_back(argv[i]);
			//summarize command line
			VM_DBG_EXECMDLINE0("Command line summary:\n\n");
			VM_DBG_EXECMDLINE1("\theapRequest:%lu KB\n",heapRequest);
			VM_DBG_EXECMDLINE1("\tstackRequest:%lu KB\n",stackRequest);
			if(debug == 1){
				VM_DBG_EXECMDLINE0("\tdebugging enabled\n");
			}else{
				VM_DBG_EXECMDLINE0("\tdebugging mode off\n");
			}
			VM_DBG_EXECMDLINE1("\tflie to load=%s\n\n",binaryFile.c_str());
#ifdef VM_DBG_EXECMDLINE
			i = programArgs_.size();
			for(__Arguments::size_type j = 0;j < i;++j)
				std::cout<<"arg["<<int(j)<<"] = "<<programArgs_[j].c_str()<<'\n';
#endif
			return true;
		}
	private:
		void PrintUsage() const{	//显示帮助
			std::cout<<"\n\tDEllVM [options] file.RUN [arguments]\n\n"
				<<"\tvirtual machine options:\n\n"
				<<"\t-h=XXX\tmemory to allocate for heap in KB\n"
				<<"\t-s=XXX\tmemory to allocate for stack in KB\n"
				<<"\t-d\tenable debug mode\n"
				<<"\t-?\tprint help\n"
				<<"\tthere are no space in the -h and -s options!\n\n";
		}
	private:
		U8 heapRequest;				//RAM to allocate for heap
		U8 stackRequest;			//RAM to allocate for stack
		int debug;					//if 1,start in debug mode
		std::string binaryFile;		//name of the bytecode file to execute
		__Arguments programArgs_;	//program arguments for bytecode
	};
}//namespace DoZerg

#endif