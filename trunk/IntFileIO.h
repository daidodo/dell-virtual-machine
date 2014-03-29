#ifndef INTFILEIO_H
#define INTFILEIO_H

#include <cstdio>

namespace DoZerg{
	template<class __Res>class IntFileIO
	{
		typedef typename __Res::U8 U8;
	public:
		~IntFileIO(){}
		explicit IntFileIO(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			typedef std::FILE * __FilePtr;
			switch(res.R1){
				case 0:{
					const int num = 5;
					const char modes[num][4] = {"rb","wb","ab","wb+","ab+"};
					if(res.R3 >= num)
						res.R4 = 0;
					else{
						__FilePtr fp = std::fopen(res.RAMPosition<char>(res.R2),modes[res.R3]);
						res.R4 = (fp == 0 ? 0 : U8(fp));
					}break;}
				case 1:{
					res.R3 = std::fclose(reinterpret_cast<__FilePtr>(res.R2));
					break;}
				case 2:{
					res.R3 = std::fflush(reinterpret_cast<__FilePtr>(res.R2));
					break;}
				case 3:{
					res.R3 = std::fflush(stdin );
					break;}
				case 4:{
					res.R3 = std::fflush(stdout );
					break;}
				case 5:{
					res.R3 = std::fflush(stderr);
					break;}
				case 6:{
					std::rewind(reinterpret_cast<__FilePtr>(res.R2));
					break;}
				case 7:{
					long pos = std::ftell(reinterpret_cast<__FilePtr>(res.R2));
					if(pos == -1L){
						res.R3 = 0;
						res.R4 = 1;
					}else{
						res.R3 = pos;
						res.R4 = 0;
					}break;}
				case 8:{
					switch(res.R4){
				case 1:{
					res.R5 = std::fseek(reinterpret_cast<__FilePtr>(res.R2),long(res.R3),SEEK_END);
					break;}
				case 2:{
					res.R5 = std::fseek(reinterpret_cast<__FilePtr>(res.R2),long(res.R3),SEEK_CUR);
					break;}
				default:{
					res.R5 = std::fseek(reinterpret_cast<__FilePtr>(res.R2),long(res.R3),SEEK_SET);
						}
					}break;}
				case 9:{
					res.R3 = feof(reinterpret_cast<__FilePtr>(res.R2));
					break;}
				case 10:{
					res.R3 = ferror(reinterpret_cast<__FilePtr>(res.R2));
					break;}
				case 11:{
					__FilePtr fp = reinterpret_cast<__FilePtr>(res.R2);
					res.R5 = std::fread(res.RAMPosition(res.R4),1,std::size_t(res.R3),fp);
					res.R6 = ((res.R5 < res.R3 && ferror(fp)) ? 1 : 0);
					break;}
				case 12:{
					__FilePtr fp = reinterpret_cast<__FilePtr>(res.R2);
					res.R5 = std::fwrite(res.RAMPosition(res.R4),1,std::size_t(res.R3),fp);
					res.R6 = ((res.R5 < res.R3 && ferror(fp)) ? 1 : 0);
					break;}
				case 13:{
					res.R5 = std::fread(res.RAMPosition(res.R4),1,std::size_t(res.R3),stdin);
					res.R6 = ((res.R5 < res.R3 && ferror(stdin)) ? 1 : 0);
					break;}
				case 14:{
					res.R5 = std::fwrite(res.RAMPosition(res.R4),1,std::size_t(res.R3),stdout);
					res.R6 = ((res.R5 < res.R3 && ferror(stdout)) ? 1 : 0);
					break;}
				case 15:{
					res.R5 = std::fwrite(res.RAMPosition(res.R4),1,std::size_t(res.R3),stderr);
					res.R6 = ((res.R5 < res.R3 && ferror(stderr)) ? 1 : 0);
					break;}
				case 16:{
					int ret = std::printf("%c",char(res.R2));
					if(ret < 0){
						res.R3 = 0;
						res.R4 = 1;
					}else{
						res.R3 = ret;
						res.R4 = 0;
					}break;}
				case 17:{
					int ret = std::wprintf(L"%lc",wchar_t(res.R2));
					if(ret < 0){
						res.R3 = 0;
						res.R4 = 1;
					}else{
						res.R3 = ret;
						res.R4 = 0;
					}break;}
				case 18:{
					int ret = std::printf("%s",res.RAMPosition(res.R2));
					if(ret < 0){
						res.R3 = 0;
						res.R4 = 1;
					}else{
						res.R3 = ret;
						res.R4 = 0;
					}break;}
				case 19:{
					int ret = std::wprintf(L"%ls",res.RAMPosition<wchar_t>(res.R2));
					if(ret < 0){
						res.R3 = 0;
						res.R4 = 1;
					}else{
						res.R3 = ret;
						res.R4 = 0;
					}break;}
				case 20:{	//note signed vs. unsigned
					int ret = (res.R3 == 1 ? std::printf("%I64u",res.R2) : std::printf("%I64d",res.R2));
					if(ret < 0){
						res.R3 = 0;
						res.R4 = 1;
					}else{
						res.R3 = ret;
						res.R4 = 0;
					}break;}
				case 21:{
					int ret = std::printf("%le",res.D1);
					if(ret < 0){
						res.R3 = 0;
						res.R4 = 1;
					}else{
						res.R3 = ret;
						res.R4 = 0;
					}break;}
				case 22:{
					int ret = std::scanf("%s",res.RAMPosition(res.R2));
					res.R3 = ((ret == EOF || ret == 0) ? 0 : ret);
					break;}
				case 23:{
					int ret = std::wscanf(L"%ls",res.RAMPosition<wchar_t>(res.R2));
					res.R3 = ((ret == EOF || ret == 0) ? 0 : ret);
					break;}
				case 24:{
					int ret = (res.R3 == 1 ? std::scanf("%I64u",&res.R2) : std::scanf("%I64d",&res.R2));
					res.R3 = ((ret == EOF || ret == 0) ? 0 : ret);
					break;}
				case 25:{
					int ret = std::scanf("%le",&res.D1);
					res.R3 = ((ret == EOF || ret == 0) ? 0 : ret);
					break;}
				default:{
					res.errFile<<BeginError
						<<"IntFileIO: INT 0, R1="<<res.R1<<" function not handled"
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
