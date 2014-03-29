#ifndef INTFILEMANAGEMENT_H
#define INTFILEMANAGEMENT_H

#include <cstring>
#include <fcntl.h>
#include <direct.h>
#include "Helpers\Win32File.h"
#include "Helpers\Win32Pipe.h"

namespace DoZerg{
	template<class __Res>class IntFileManagement
	{
	public:
		~IntFileManagement(){}
		explicit IntFileManagement(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			switch(res.R1){
				case 0:{		//check if exist
					res.R3 = (_access(res.RAMPosition<char>(res.R2),0) == 0 ? 1 : 0);
					break;}
				case 1:{		//directory
					DWORD ret = GetFileAttributes(res.RAMPosition<char>(res.R2));
					res.R3 = (ret == INVALID_FILE_ATTRIBUTES ? 2 : ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0 ? 1 : 0));
					break;}
				case 2:{		//read
					res.R3 = (_access(res.RAMPosition<char>(res.R2),4) == 0 ? 1 : 0);
					break;}
				case 3:{		//write
					res.R3 = (_access(res.RAMPosition<char>(res.R2),2) == 0 ? 1 : 0);
					break;}
				case 4:{		//file size
					res.R3 = 0;
					res.R4 = 1;
					Win32File file(res.RAMPosition<char>(res.R2),_O_RDONLY);
					if(file.IsOpen()){
						long size = file.FileSize();
						if(size != -1L){
							res.R3 = size;
							res.R4 = 0;
						}
					}break;}
				case 5:{		//file stat
					res.R3 = 0;
					res.R4 = 1;
					Win32File file(res.RAMPosition<char>(res.R2),_O_RDONLY);
					if(file.IsOpen()){
						Win32File::__Stat fields;
						if(file.GetFileStat(fields)){
							res.R3 = fields.st_mtime;
							res.R4 = 0;
						}
					}break;}
				case 6:{
					res.R3 = (remove(res.RAMPosition<char>(res.R2)) == -1 ? 1 : 0);
					break;}
				case 7:{
					res.R3 = (_mkdir(res.RAMPosition<char>(res.R2)) == -1 ? 1 : 0);
					break;}
				case 8:{
					res.R3 = (_rmdir(res.RAMPosition<char>(res.R2)) == -1 ? 1 : 0);
					break;}
				case 9:{
					char full[_MAX_PATH];
					if(_fullpath(full,res.RAMPosition<char>(res.R2),_MAX_PATH) == 0){
						res.R4 = 1;
						res.R5 = 0;
					}else{
						std::strcpy(res.RAMPosition<char>(res.R3),full);
						res.R4 = 0;
						res.R5 = std::strlen(full); + 1;
					}break;}
				case 10:{
					res.R2 = _MAX_PATH;
					break;}
				case 11:{		//execute dir /b
					res.R5 = 0;
					res.R6 = 1;
					char buffer[16 * 1024] = "dir ";
					//max windows filename is 215 chars in length
					char * filename = res.RAMPosition<char>(res.R2);
					if(std::strlen(filename) <= 215){
						std::strcat(buffer,filename);
						std::strcat(buffer," /b");
						Win32Pipe pipe(buffer,"rb");
						if(pipe.IsOpen()){
							res.R5 = pipe.Read(buffer,1,16 * 1024);
							if(res.R3 != 1)
								std::strcpy(filename,buffer);
							res.R6 = 0;
						}
					}break;}
				case 12:{
					res.R4 = (rename(res.RAMPosition<char>(res.R2),res.RAMPosition<char>(res.R3)) != 0 ? 1 : 0);
					break;}
				default:{
					res.errFile<<BeginError
						<<"IntFileManagement: INT 1, R1="<<res.R1<<" function not handled"
						<<EndError;
					return false;
				}
			}
			return true;
		}
	private:
		bool suc_;
	};
}//namespace DoZerg

#endif