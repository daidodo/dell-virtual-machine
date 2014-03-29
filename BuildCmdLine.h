#ifndef BUILDCMDLINE_H
#define BUILDCMDLINE_H

#include <string>
#include <vector>
#include <iostream>

namespace DoZerg{
#ifdef VM_DBG_BLDCMDLINE
#define VM_DBG_BLDCMDLINE0(str)				std::cout<<"BuildCmdLine::HandleArguments(): "<<str;
#define VM_DBG_BLDCMDLINE1(str,arg1)		std::cout<<"BuildCmdLine::HandleArguments(): ";std::printf(str,arg1);
#define VM_DBG_BLDCMDLINE2(str,arg1,arg2)	std::cout<<"BuildCmdLine::HandleArguments(): ";std::printf(str,arg1,arg2);
#else
#define VM_DBG_BLDCMDLINE0(str)
#define VM_DBG_BLDCMDLINE1(str,arg1)
#define VM_DBG_BLDCMDLINE2(str,arg1,arg2)
#endif
	template<class __ErrCount>class BuildCmdLine
	{
	public:
		typedef std::vector<std::string> __Arguments;
		typedef __ErrCount __ErrCount;
	private:
		static const char IN_EXT[5];				//file extensions
		static const char TMP_EXT[5];
		static const char LST_EXT[5];
		static const char OUT_EXT[5];
		static const std::string DEFAULT_NAME;		//default name of file
	public:
		~BuildCmdLine(){}
		BuildCmdLine():omitDebugData(0),listing(0){}
		const char * InputFileName() const{return inputFile_.c_str();}
		const char * ListFileName() const{return listFile_.c_str();}
		const char * TempFileName() const{return tempFile_.c_str();}
		const char * OutputFileName() const{return outputFile_.c_str();}
		int Listing() const{return listing;}
		int OmitDebugData() const{return omitDebugData;}
		bool HandleArguments(const __Arguments & argv){
			if(argv.empty()){
				__ErrCount()<<"No Arguments\n";
				PrintUsage();
				return false;
			}
			//get input file name "filename.ASM"
			int inFileLen = int(argv[0].length());
			if(inFileLen > 4 && argv[0][inFileLen - 4] == '.' &&
				(argv[0][inFileLen - 3] == 'A' || argv[0][inFileLen - 3] == 'a') &&
				(argv[0][inFileLen - 2] == 'S' || argv[0][inFileLen - 2] == 's') &&
				(argv[0][inFileLen - 1] == 'M' || argv[0][inFileLen - 1] == 'm')){
				inputFile_ = argv[0];
			}else{
				__ErrCount()<<"expecting .ASM file\n";
				PrintUsage();
				return false;
			}
			//process options
			bool OutFileSet = false;
			for(int i = 1,j = int(argv.size());i < j;++i){
				if(argv[i][0] != '-'){
					__ErrCount()<<"bad option,need \'-\' before "<<argv[i]<<'\n';
					PrintUsage();
					return false;
				}
				switch(argv[i][1]){
					case 'd':
					case 'D':{
						if(argv[i][2] != '\0'){
							__ErrCount()<<"bad \'-d\' in "<<argv[i]<<'\n';
							PrintUsage();
							return false;
						}
						omitDebugData = true;
						VM_DBG_BLDCMDLINE0("Omit Debug Data is TRUE\n");
						break;}
					case 'l':
					case 'L':{
						if(argv[i][2] != '\0'){
							__ErrCount()<<"bad \'-l\' in "<<argv[i]<<'\n';
							PrintUsage();
							return false;
						}
						listing = 1;
						VM_DBG_BLDCMDLINE0("List File set to TRUE\n");
						break;}
					case 'o':
					case 'O':{		//output file must look like "file.RUN"
						if(argv[i][2] != '='){
							__ErrCount()<<"bad \'-o=XXX\' in "<<argv[i]<<'\n';
							PrintUsage();
							return false;
						}else{
							int len = int(argv[i].length());
							if(len > 7/*(-o=file.RUN)4 + 3*/ && argv[i][len - 4] == '.' &&
								(argv[i][len - 3] == 'R' || argv[i][len - 3] == 'r') &&
								(argv[i][len - 2] == 'U' || argv[i][len - 2] == 'u') &&
								(argv[i][len - 1] == 'N' || argv[i][len - 1] == 'n')){
									outputFile_ = argv[i].substr(3);
								}else{
									__ErrCount()<<"expecting .RUN file but "<<argv[i]<<'\n';
									PrintUsage();
									return false;
								}
						}
						OutFileSet = true;
						break;}
					case '?':{
						PrintUsage();
						return false;}
					default:{
						__ErrCount()<<"bad option: "<<argv[i]<<'\n';
						PrintUsage();
						return false;
							}
				}
			}
			if(!OutFileSet)
				outputFile_ = inputFile_.substr(0,inFileLen - 4) + OUT_EXT;
			inFileLen = int(outputFile_.length());
			listFile_ = outputFile_.substr(0,inFileLen - 4) + LST_EXT;
			tempFile_ = outputFile_.substr(0,inFileLen - 4) + TMP_EXT;
			//summary
			VM_DBG_BLDCMDLINE0("Command line summary\n");
			VM_DBG_BLDCMDLINE1("\tinput file = %s\n",inputFile_.c_str());
			VM_DBG_BLDCMDLINE1("\toutput file = %s\n",outputFile_.c_str());
			VM_DBG_BLDCMDLINE1("\ttemp file = %s\n",tempFile_.c_str());
			VM_DBG_BLDCMDLINE1("\tlist file = %s\n",listFile_.c_str());
			if(listing){
				VM_DBG_BLDCMDLINE0("\tlisting is ON\n");
			}else{
				VM_DBG_BLDCMDLINE0("\tlisting is OFF\n");
			}
			if(omitDebugData){
				VM_DBG_BLDCMDLINE0("\tomit Debug Data\n");
			}else{
				VM_DBG_BLDCMDLINE0("\tDebug Data included\n");
			}
			return true;
		}
	private:
		void PrintUsage() const{
			std::cout<<"\n\tDEllASM file.ASM [optoins]\n\n"
				<<"\tOptions:\n\n"
				<<"\t-d\t\tomit debug information\n"
				<<"\t-l\t\tproduce a listing file\n"
				<<"\t-o=XXX\t\tname of output file(compilation unit)\n"
				<<"\t-?\t\tprint help\n"
				<<"\n\tthere are no spaces between option characters\n\n";
		}
	private:
		std::string inputFile_,listFile_,tempFile_,outputFile_;
		int omitDebugData;		//true if omit debug data
		int listing;			//true if desire a listing
	};
	template<class __ErrCount>
		const char BuildCmdLine<__ErrCount>::IN_EXT[5] = ".ASM";
	template<class __ErrCount>
		const char BuildCmdLine<__ErrCount>::TMP_EXT[5] = ".TMP";
	template<class __ErrCount>
		const char BuildCmdLine<__ErrCount>::LST_EXT[5] = ".LST";
	template<class __ErrCount>
		const char BuildCmdLine<__ErrCount>::OUT_EXT[5] = ".RUN";
	template<class __ErrCount>
		const std::string BuildCmdLine<__ErrCount>::DEFAULT_NAME = "Un-Named";
}//namespace DoZerg

#endif