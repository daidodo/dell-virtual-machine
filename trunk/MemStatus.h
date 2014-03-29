#ifndef MEMSTATUS_H
#define MEMSTATUS_H

namespace DoZerg{
	template<typename __Type>class MemStatus
	{
		typedef MemStatus<__Type> __Myt;
	public:
		static const __Myt Free;
		static const __Myt Reserved;
	public:
		operator __Type() const{return id_;}
	private:
		explicit MemStatus(__Type id):id_(id){}
		~MemStatus(){}
	private:
		__Type id_;
	};
	template<typename __Type>
		const MemStatus<__Type> MemStatus<__Type>::Free(0);
	template<typename __Type>
		const MemStatus<__Type> MemStatus<__Type>::Reserved(1);
}//namespace DoZerg

#endif