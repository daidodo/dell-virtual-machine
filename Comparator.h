#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <map>

namespace DoZerg{
	template<class __StrTbl>class Comparator
		:public std::binary_function<std::string,typename __StrTbl::U8,bool>
	{
	public:
		typedef typename __StrTbl::U8 U8;
		typedef std::binary_function<std::string,U8,bool> __MyBase;
		typedef typename __MyBase::first_argument_type first_argument_type;
	public:
		~Comparator(){}
		explicit Comparator(const __StrTbl & st):StrTbl_(st){}
		template<typename T1,typename T2>
			bool operator ()(const  T1 & left,const  T2 & right) const{
				return DataTrans(left) < DataTrans(right);
			}
		const __StrTbl & StrTable() const{
			return StrTbl_;
		}
	private:
		const  std::string & DataTrans(const std::string & val) const{return val;}
		const  std::string & DataTrans(U8 index) const{return StrTbl_[index];}
	private:
		const __StrTbl & StrTbl_;
	};
}//namespace DoZerg

#endif