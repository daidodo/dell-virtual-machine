#ifndef WIN32FILE_H
#define WIN32FILE_H

#include <io.h>
#include <sys\stat.h>

namespace DoZerg{
	class Win32File
	{
	public:
		typedef struct _stat __Stat;
	public:
		Win32File(const char * sFileName,int mode):handle_(_open(sFileName,mode)){}
		~Win32File(){
			if(IsOpen())
				_close(handle_);
		}
		bool IsOpen() const{return handle_ != -1;}
		long FileSize() const{return _filelength(handle_);}
		bool GetFileStat(__Stat & field){return _fstat(handle_,&field) != -1;}
	private:
		int handle_;
	};
}//namespace DoZerg

#endif
