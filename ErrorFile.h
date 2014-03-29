#ifndef ERRORFILE_H
#define ERRORFILE_H

#include <ctime>
#include "FileIO.h"
#include "ErrorCount.h"

namespace DoZerg{
#ifdef VM_DBG_ERRFILE
#define VM_DBG_ERRFILE0(str)			std::cout<<"ErrorFile::"<<str;
#define VM_DBG_ERRFILE1(str,arg1)		std::cout<<"ErrorFile::";std::printf(str,arg1);
#define VM_DBG_ERRFILE2(str,arg1,arg2)	std::cout<<"ErrorFile::";std::printf(str,arg1,arg2);
#else
#define VM_DBG_ERRFILE0(str)
#define VM_DBG_ERRFILE1(str,arg1)
#define VM_DBG_ERRFILE2(str,arg1,arg2)
#endif
	enum ErrorFileEnum{BeginError,EndError};
	template<class __Platform>class ErrorFile:public ErrorCount<__Platform>
	{
	public:
		typedef FileIO<__Platform> __FileIO;
		typedef ErrorCount<__Platform> __ErrCount;
	private:
		typedef ErrorCount<__Platform> __MyBase;
	public:
		ErrorFile():__MyBase(false){}
		~ErrorFile(){Close();}
		const ErrorFile & operator <<(ErrorFileEnum msg) const{
			if(msg == BeginError){
				__MyBase::operator <<("ERROR: ");
				xmlBegin();
			}else{
				xmlEnd();
			}
			return *this;
		}
		template<class __Type>
			const ErrorFile & operator <<(const __Type & msg) const{
				__MyBase::operator <<(msg);
				errFile<<msg;
				return *this;
			}
		bool Setup(){
			char sFileName[50];
			strcpy(sFileName,"logs\\VM_LOG_");
			time_t now(time(0));
			tm local_time(*(localtime(&now)));
			char tmp[16];
			std::sprintf(tmp,"%lu-",local_time.tm_year + 1900);
			strcat(sFileName,tmp);
			std::sprintf(tmp,"%u-",local_time.tm_mon + 1);
			strcat(sFileName,tmp);
			std::sprintf(tmp,"%u_",local_time.tm_mday);
			strcat(sFileName,tmp);
			std::sprintf(tmp,"%u_",local_time.tm_hour);
			strcat(sFileName,tmp);
			std::sprintf(tmp,"%u_",local_time.tm_min);
			strcat(sFileName,tmp);
			std::sprintf(tmp,"%u",local_time.tm_sec);
			strcat(sFileName,tmp);
			strcat(sFileName,".XML");
			if(!errFile.Open(sFileName,"w")){
				__MyBase::operator <<("ErrorFile::Setup(): can't open ")<<sFileName<<'\n';
				return false;
			}
			errFile<<"<ERRORS>\n";
			VM_DBG_ERRFILE0("Setup(): ErrorFile open successfully\n");
			return true;
		}
		void Close(){
			if(!errFile.IsOpen())
				return;
			errFile<<"</ERRORS>";
			errFile.Close();
			VM_DBG_ERRFILE0("Close(): error file closed\n");
		}
	private:
		void xmlBegin() const{
			time_t now = ::time(0);
			errFile<<"<Message>\n\t<time>\n\t"
				<<ctime(&now)
				<<"\t</time>\n\t<content>\n\t";
		}
		void xmlEnd() const{errFile<<"\t</content>\n</Message>\n";}
	private:
		__FileIO errFile;
	};
}//namespace DoZerg

#endif