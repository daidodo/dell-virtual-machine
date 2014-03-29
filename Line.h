#ifndef LINE_H
#define LINE_H

#include <vector>
#include <cassert>
#include <iostream>
#include "Word.h"

namespace DoZerg{
	template<class __FileIO>class Line
	{
	public:
		typedef Word __Word;
	private:
		typedef typename __FileIO::__Platform __Platform;
		typedef typename __FileIO::U8 U8;
	public:
		~Line(){}
		explicit Line(const __FileIO & file)
			:file_(file)
			,row_(0)
		{assert(file_.IsOpen() && "Line::Line()");}
		bool Empty() const{return line_.size() == 0;}
		const __Word & operator [](int index) const{return line_[index];}
		char Seperator(int index) const{return seperator_[index];}
		int Size() const{return int(line_.size());}
		int Row() const{return row_;}
		void InitRow(){row_ = 0;}
		bool MatchSeperator(const std::string & sep) const{
			int mylen = int(seperator_.length());
			int len = int(sep.length());
			for(int i = 0,j = len < mylen ? len : mylen;i < j;++i)
				if(sep[i] != seperator_[i])
					return false;
			for(int i = len;i < mylen;++i)
				if(seperator_[i] != seperator_[len - 1])
					return false;
			return true;
		}
		void Print() const{
			std::cout<<"LINE: ";
			if(!line_.empty()){
				std::cout<<line_[0];
				for(int i = 1,j = int(line_.size());i < j;++i)
					std::cout<<seperator_[i - 1]<<line_[i];
			}
			std::cout<<'\n';
		}
		bool GetLine(){		//return false = End Of File,return true = '\n'
			++row_;
			line_.clear();
			seperator_.clear();
			int c;
			do{
				c = file_.GetChar();
			}while(c == ' ' || c == '\t');
			__Word word;
			char sep = 0;
			for(bool comments = (c == '#');c != '\n' && c != __FileIO::EndOfFile;c = file_.GetChar()){	//# means comments in DEllASM source file
				if(comments){
					word.push_back(c);
					continue;
				}
				if(c == ','){
					sep = ',';
					line_.push_back(word);
					seperator_.push_back(sep);
					word.clear();
				}else if(c == ' ' || c == '\t'){
					if(sep == 0)
						sep = ' ';
				}else if(c > 31 && c < 127){		//printable characters
					if(sep == ' '){
						line_.push_back(word);
						seperator_.push_back(sep);
						word.clear();
						word.push_back(c);
					}else
						word.push_back(c);
					sep = 0;
				}
			}
			if(sep == ',' || !word.empty())
				line_.push_back(word);
			return c == '\n';
		}
		const std::string ToString(U8 bytepos,int width,int base) const{	//to a string with fixed width:	row)[bytepos] Word sep Word ...
			char tmp[16];
			std::sprintf(tmp,"%d)[",row_);
			std::string str(tmp);
			if(base == 10)
				__Platform::spU8(tmp,bytepos);
			else if(base == 16)
				std::sprintf(tmp,"%X",int(bytepos));
			else
				assert("bad base in Line::ToString()");
			str += std::string(tmp) + "]";
			if(line_.empty())
				return str.c_str();
			str += " " + line_[0];
			for(int i = 1,j = int(line_.size());i < j;++i){
				str.push_back(seperator_[i - 1]);
				str += line_[i];
			}
			for(int i = int(str.length());i < width;++i)
				str.push_back(' ');
			return str.c_str();
		}
		const std::string ToString() const{						//to a string:	row) Word sep Word ...
			char tmp[16];
			std::sprintf(tmp,"%d)",row_);
			std::string str(tmp);
			if(line_.empty())
				return str.c_str();
			str += " " + line_[0];
			for(int i = 1,j = int(line_.size());i < j;++i){
				str.push_back(seperator_[i - 1]);
				str += line_[i];
			}
			return str.c_str();
		}
	private:
		const __FileIO & file_;
		int row_;
		std::vector<__Word> line_;		//words vector
		std::string seperator_;			//seperators vector
	};
	template<class T>
		std::ostream & operator <<(std::ostream & os,const Line<T> & line){
			os<<"LINE "<<line.Row()<<": ";
			if(!line.Empty())
				os<<line[0];
			for(int i = 1;i < line.Size();++i)
				os<<line.Seperator(i - 1)<<line[i];
			os<<'\n';
			return os;
		}
}//namespace DoZerg

#endif