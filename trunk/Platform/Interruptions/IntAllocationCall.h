#ifndef INTALLOCATIONCALL_H
#define INTALLOCATIONCALL_H

#include <cstdio>
#include "HeapStorage.h"

namespace DoZerg{
	template<class __Res>class IntAllocationCall
	{
		typedef typename __Res::U1 U1;
		typedef typename __Res::U8 U8;
		typedef HeapStorage<U1,U8> __Heap;
		static __Heap heap_;
	public:
		~IntAllocationCall(){}
		explicit IntAllocationCall(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			switch(res.R1){
				case 0:{
					U8 ret = heap_.Allocate(res.R2);
					res.R3 = (ret == __Heap::BadLocation ? 0 : ret + res.HS);
					break;}
				case 1:{
					if(!heap_.Deallocate(res.R2 - res.HS)){
						res.errFile<<BeginError
							<<"IntAllocationCall: INT 7, R1=1, address R2="<<res.R2<<" out of bounds"
							<<EndError;
						return false;
					}break;}
				case 2:{
					heap_.Print(std::cout,res.HS);
					break;}
				case 3:{
					heap_.Initialize(res.RAMPosition(res.HS),res.HE - res.HS + 1);
					break;}
				default:{
					res.errFile<<BeginError
						<<"IntAllocationCall: INT 7, R1="<<res.R1<<" function not handled"
						<<EndError;
					return false;
				}
			}
			return true;
		}
	private:
		bool suc_;
	};
	template<class __Res>
		typename IntAllocationCall<__Res>::__Heap
		IntAllocationCall<__Res>::heap_;
}//namespace DoZerg

#endif