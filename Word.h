#ifndef WORD_H
#define WORD_H

#include <string>

namespace DoZerg{
	class Word:public std::string
	{
	public:
		Word(){}
		~Word(){}
		Word(const std::string & str):std::string(str){}
		bool MatchIdentifier() const{		//check if a word is a identifier
			if(empty())
				return false;
			const_iterator i = begin();
			if(!IsCharacter(*i) && !IsSpecialCharacter(*i))
				return false;
			for(++i;i != end();++i){
				if(!IsCharacter(*i) && !IsSpecialCharacter(*i) && !IsDigit(*i))
					return false;
			}
			return true;
		}
		template<typename __Number>
			bool MatchSignedInteger(__Number & val) const{	//check if a word is a signed integer
				bool sign = true;
				__Number ret = 0;
				if(!IsNumber(ret,sign))
					return false;
				val = sign ? ret : -ret;
				return true;
			}
		template<typename __Number>
			bool MatchUnsignedInteger(__Number & val) const{	//check if a word is a unsigned integer
				bool sign = true;
				__Number ret = 0;
				if(!IsNumber(ret,sign) || !sign)
					return false;
				val = ret;
				return true;
			}
		template<typename __Number>
			bool MatchIntegerValue(__Number & val) const{	//check if a word is a constant integer value
				if(MatchSignedInteger(val))
					return true;
				if(size() != 3 || (*this)[0] != 39 || (*this)[2] != 39)		//ASCII(') = 39
					return false;
				val = (*this)[1];
				return true;
			}
		template<typename __FloatNumber>
			bool MatchFloatValue(__FloatNumber & val) const{	//check if a word is a constant float value
				val = __FloatNumber(::atof(c_str()));
				return true;
			}
	private:
		bool IsCharacter(char c) const{return (c >='a' && c <= 'z') || (c >='A' && c <= 'Z');}
		bool IsSpecialCharacter(char c) const{return c == '.' || c == '?' || c == '@' || c == '_';}
		bool IsNZDigit(char c) const{return c >= '1' && c <= '9';}	//None-Zero digits
		bool IsDigit(char c) const{return c >= '0' && c <= '9';}
		template<typename __Number>
			bool IsNumber(__Number & ret,bool & sign) const{
				int base = 0;
				for(const_iterator v = begin();v != end() && base != -1; ++v){
					switch(base){
						case 0:
							if('+' == *v || '-' == *v){
								base = 1;
								if('-' == *v)
									sign = false;
							}else if('0' == *v)
								base = 2;
							else if(*v >= '1' && *v <= '9'){
								base = 10;
								ret = *v - '0'; 
							}else
								base = -1;
							break;
						case 1:		//'+' or '-'
							if('0' == *v)
								base = 2;
							else if(*v >= '1' && *v <= '9'){
								base = 10;
								ret = *v - '0';
							}else
								base = -1;		//only '+' or '-'
							break;
						case 2:		//'0'	OK
							if('x' == *v || 'X' == *v)
								base = 3;
							else if(*v >= '0' && *v <= '9'){
								base = 10;
								ret = *v - '0';
							}else
								base = -1;
							break;
						case 3:		//"0x" or "0X"
							if((*v > '0' && *v <= '9')||(*v >= 'a' && *v <= 'f')||(*v >= 'A' && *v <= 'F')){
								base = 16;
								if(*v > '0' && *v <= '9')
									ret = *v - '0';
								else if(*v >= 'a' && *v <= 'f')
									ret = *v - 'a' + 10;
								else
									ret = *v - 'A' + 10;
							}else
								base = -1;
							break;
						case 10:		//	OK
							if(*v >= '0' && *v <= '9')
								ret = ret * 10 + *v - '0';
							else
								base = -1;
							break;
						case 16:		//	OK
							if((*v >= '0' && *v <= '9')||(*v >= 'a' && *v <= 'f')||(*v >= 'A' && *v <= 'F')){
								if(*v > '0' && *v <= '9')
									ret = ret * 16 + *v - '0';
								else if(*v >= 'a' && *v <= 'f')
									ret = ret * 16 + *v - 'a' + 10;
								else
									ret = ret * 16 + *v - 'A' + 10;
							}else
								base = -1;
							break;
					}
				}
				return (base == 2 || base == 10 || base == 16);
			}
	};
}//namespace DoZerg

#endif