#ifndef	STRINGTABLE_H
#define STRINGTABLE_H

#include <string>
#include <vector>

namespace DoZerg{
	template<class __Platform>class StringTable
	{
	public:
		typedef __Platform __Platform;
		typedef typename __Platform::U8 U8;
	private:
		typedef std::vector<std::string> __Container;
		typedef __Container::size_type __Size;
		typedef __Container::const_iterator __CIter;
	public:
		StringTable(){}
		~StringTable(){}
		U8 AddString(const std::string & str){
			container_.push_back(str);
			return container_.size() - 1;
		}
		template<class __OutStream>
			void Print(__OutStream & out) const{
				out<<"String Table:\n";
				for(__CIter i = container_.begin();i != container_.end();++i)
					out<<'\t'<<i->c_str()<<'\n';
				out<<"End of String Table\n";
			}
		const std::string & operator [](U8 index) const{
			return container_[__Size(index)];
		}
		U8 TotalBytes() const{
			U8 size = 8;
			for(__CIter i = container_.begin();i != container_.end();++i)
				size += i->size() + 1;	//'\0'
			return size;
		}
		U8 Size() const{
			return container_.size();
		}
		template<class __FileIO>
			void Read(const __FileIO & file){
				std::string str;
				U8 size;
				container_.reserve(__Size(file.GetVar(size)));
				for(U8 i = 0;i < size;){
					if(int c = file.GetChar()){
						str.push_back(c);
					}else if(!str.empty()){
						container_.push_back(str);
						str.clear();
						++i;
					}
				}
			}
		template<class __FileIO>
			void Write(const __FileIO & file) const{
				file.PutVar(Size());
				for(__CIter i = container_.begin();i != container_.end();++i)
					file<<i->c_str()<<'\0';
			}
	private:
		__Container container_;
	};
}//namespace DoZerg

#endif