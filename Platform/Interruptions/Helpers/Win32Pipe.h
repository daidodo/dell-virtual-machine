#ifndef WIN32PIPE_H
#define WIN32PIPE_H

#include <cassert>
#include <stdio.h>

namespace DoZerg{
	class Win32Pipe
	{
		typedef FILE * __FilePtr;
	public:
		Win32Pipe(const char * sFileName,const char * mode):pipe_(_popen(sFileName,mode)){}
		~Win32Pipe(){
			if(IsOpen())
				_pclose(pipe_);
		}
		bool IsOpen() const{return pipe_ != 0;}
		size_t Read(void * buf,size_t size,size_t count) const{
			assert(pipe_ != 0 && "Win32Pipe::Read()");
			return fread(buf,size,count,pipe_);
		}
	private:
		__FilePtr pipe_;
	};
}//namespace DoZerg

#endif