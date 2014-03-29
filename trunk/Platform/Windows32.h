#ifndef WINDOWS32_H
#define WINDOWS32_H

#include <windows.h>
#include <algorithm>
#include "Interruptions\Interrupt.h"

namespace DoZerg{
	struct Windows32Platform
	{
	private:
		typedef Windows32Platform __Myt;
	public:
		//data type definitions
		typedef unsigned char		U1;
		typedef unsigned short		U2;
		typedef unsigned long		U4;
		typedef unsigned __int64	U8;
		typedef signed char			S1;
		typedef signed short		S2;
		typedef signed long			S4;
		typedef signed __int64		S8;
		typedef float				F4;
		typedef double				F8;
		//functions
		static int pU8(U8 v){return std::printf("%I64u",v);}	//print U8,S8 to std out
		static int pS8(S8 v){return std::printf("%I64d",v);}
		static void spU8(char * str,U8 v){std::sprintf(str,"%I64u",v);}	//print U8,S8 to string
		static void fpU8(std::FILE * fp,U8 v){std::fprintf(fp,"%I64u",v);}	//print U8,S8 to file
		static void fpS8(std::FILE * fp,S8 v){std::fprintf(fp,"%I64d",v);}
		//降序－升序格式转换函数
		template<typename __Type>
			static const __Type & Bytecode2Type(U1 * bytes){
				const int size = sizeof(__Type);
				for(int i = (size >> 1) - 1;i >= 0;--i)
					std::swap(bytes[i],bytes[size - 1 -i]);
				return *reinterpret_cast<__Type *>(bytes);
			}
		//升序－降序格式转换函数
		template<typename __Type>
			static void Type2Bytecode(__Type val,U1 * result){
				S1 * tmp = reinterpret_cast<S1*>(&val);
				const size = sizeof(__Type);
				for(int i = 0;i < size;++i)
					result[i] = tmp[size - i - 1];
			}
		//得到当前可用物理内存
		static U8 GetAvailableMemory(){
			MEMORYSTATUS mem_status;
			mem_status.dwLength = sizeof(MEMORYSTATUS);
			GlobalMemoryStatus(&mem_status);
			return mem_status.dwAvailPhys;
		}
		//得到文件大小
		static U8 GetFileSize(const char * sFileName){
			WIN32_FILE_ATTRIBUTE_DATA fdata;
			if(GetFileAttributesEx(sFileName,GetFileExInfoStandard,&fdata))
				return fdata.nFileSizeLow;
			else
				return 0;
		}
		//中断处理函数
		template<class __Res>
			static bool HandleInterrupt(__Res & res,int vec){return Interrupt::HandleInt(res,vec);}
		static void SetInterruptOn(bool on){Interrupt::IntOn = on;}
	};
}//namespace DoZerg

#endif