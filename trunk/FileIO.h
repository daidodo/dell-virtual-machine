#ifndef FILEIO_H
#define FILEIO_H

#include <cstdio>
#include <string>
#include <cassert>

namespace DoZerg{
	template<class __Platform>class FileIO
	{
	public:
		typedef __Platform __Platform;
		typedef typename __Platform::U1 U1;
		typedef typename __Platform::U4 U4;
		typedef typename __Platform::U8 U8;
		typedef typename __Platform::S4 S4;
		typedef typename __Platform::S8 S8;
	public:
		static const EndOfFile = -1;
	public:
		~FileIO(){Close();}
		FileIO(bool mute = false)
			:fp_(0)
			,mute_(mute)
		{}
		explicit FileIO(const char sFileName[],const char option[] = "rb",bool mute = false)
			:file_(sFileName)
			,fp_(std::fopen(sFileName,option))
			,mute_(mute)
		{}
		bool IsOpen() const{return fp_ != 0;}
		const char * FileName() const{return file_.c_str();}
		bool Open(const char sFileName[],const char option[] = "rb"){
			assert(fp_ == 0 && "FileIO::Open()");
			file_ = sFileName;
			return (fp_ = std::fopen(sFileName,option)) != 0;
		}
		bool ReOpen(const char option[] = "rb"){
			assert(fp_ == 0 && "FileIO::ReOpen()");
			assert(file_.length() > 0 && "FileIO::ReOpen()");
			return (fp_ = std::fopen(file_.c_str(),option)) != 0;
		}
		void Close(){
			if(fp_){
				fclose(fp_);
				fp_ = 0;
			}
		}
		const FileIO & operator <<(const char * str) const{
			if(mute_ && fp_ == 0)
				return *this;
			assert(fp_ != 0 && "FileIO::operator <<(const char * str)");
			std::fprintf(fp_,"%s",str);
			return *this;
		}
		const FileIO & operator <<(const unsigned char * str) const{
			if(mute_ && fp_ == 0)
				return *this;
			assert(fp_ != 0 && "FileIO::operator <<(const unsigned char * str)");
			std::fprintf(fp_,"%s",str);
			return *this;
		}
		const FileIO & operator <<(char c) const{
			if(mute_ && fp_ == 0)
				return *this;
			assert(fp_ != 0 && "FileIO::operator <<(const char c)");
			std::fprintf(fp_,"%c",c);
			return *this;
		}
		const FileIO & operator <<(unsigned char c) const{
			if(mute_ && fp_ == 0)
				return *this;
			assert(fp_ != 0 && "FileIO::operator <<(const unsigned char c)");
			std::fprintf(fp_,"%c",c);
			return *this;
		}
		const FileIO & operator <<(int i) const{
			if(mute_ && fp_ == 0)
				return *this;
			assert(fp_ != 0 && "FileIO::operator <<(int i)");
			std::fprintf(fp_,"%d",i);
			return *this;
		}
		const FileIO & operator <<(U4 i) const{
			if(mute_ && fp_ == 0)
				return *this;
			assert(fp_ != 0 && "FileIO::operator <<(U4 i)");
			std::fprintf(fp_,"%u",i);
			return *this;
		}
		const FileIO & operator <<(S4 i) const{
			if(mute_ && fp_ == 0)
				return *this;
			assert(fp_ != 0 && "FileIO::operator <<(S4 i)");
			std::fprintf(fp_,"%d",i);
			return *this;
		}
		const FileIO & operator <<(U8 i) const{
			if(mute_ && fp_ == 0)
				return *this;
			assert(fp_ != 0 && "FileIO::operator <<(U8 i)");
			__Platform::fpU8(fp_,i);
			return *this;
		}
		const FileIO & operator <<(S8 i) const{
			if(mute_ && fp_ == 0)
				return *this;
			assert(fp_ != 0 && "FileIO::operator <<(S8 i)");
			__Platform::fpS8(fp_,i);
			return *this;
		}
		void Rewind() const{		//rewind to start of the file
			assert(fp_ != 0 && "FileIO::Rewind()");
			std::rewind(fp_);
		}
		int Seek(U8 offset) const{
			assert(fp_ != 0 && "FileIO::FileSeek()");
			return std::fseek(fp_,long(offset),SEEK_SET);		//long() is a warning because of U4 size limitation
		}
		int GetChar() const{
			assert(fp_ != 0 && "FileIO::FileGetChar()");
			return std::fgetc(fp_);
		}
		template<typename __Type>
			void PutVar(__Type val) const{
				assert(fp_ != 0 && "FileIO::PutVar()");
				const len = sizeof(__Type);
				U1 encode[len];
				__Platform::Type2Bytecode(val,encode);
				for(int i = 0;i < len;++i)
					std::fprintf(fp_,"%c",encode[i]);
			}
		template<typename __Type>
			__Type GetVar(__Type & val) const{
				assert(fp_ != 0 && "FileIO::GetVar()");
				const len = sizeof(__Type);
				U1 encode[len];
				for(int i = 0;i < len;++i)
					encode[i] = std::fgetc(fp_);
				return val = __Platform::Bytecode2Type<__Type>(encode);
			}
	private:
		FileIO(const FileIO &);
		FileIO & operator =(const FileIO &);
	private:
		std::FILE * fp_;
		std::string file_;
		bool mute_;
	};
}//namespace DoZerg

#endif