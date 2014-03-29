#ifndef RAWMEMORY_H
#define RAWMEMORY_H

#include <cassert>

namespace DoZerg{
	template<class __Platform>class RawMemory
	{
	public:
		typedef typename __Platform::U1 U1;
		typedef typename __Platform::U8 U8;
	public:
		RawMemory():mem_(0),size_(0){}
		~RawMemory(){}
		bool Allocate(U8 size){
			assert(mem_ == 0 && "RawMemory::Allocate");
			mem_ = new(std::nothrow) U1[std::size_t(size)];
			if(!mem_)
				return false;
			size_ = size;
			return true;
		}
		void Deallocate(){
			if(mem_ == 0)
				return;
			delete [] mem_;
			mem_ = 0;
			size_ = 0;
		}
		template<typename __Type>
			__Type * Pointer(U8 index) const{
				assert(index < size_ && "RawMemory::__Type * Pointer()");
				return reinterpret_cast<__Type *>(mem_ + index);
			}
		U1 * Pointer(U8 index) const{
			assert(index < size_ && "RawMemory::U1 * Pointer()");
			return mem_ + index;
		}
		U1 operator [](U8 index) const{
			assert(index < size_ && "RawMemory::U1 operator [](U8 index) const");
			return mem_[index];
		}
		U1 & operator [](U8 index){
			assert(index < size_ && "RawMemory::U1 & operator [](U8 index)");
			return mem_[index];
		}
		template<typename __Type>
			__Type Oprand(U8 index) const{
				assert(index < size_ && "RawMemory::__Type Oprand(U8 index) const");
				return *reinterpret_cast<__Type *>(mem_ + index);
			}
		template<typename __Type>
			__Type & Oprand(U8 index){
				assert(index < size_ && "RawMemory::__Type & Oprand(U8 index)");
				return *reinterpret_cast<__Type *>(mem_ + index);
			}
	private:
		U1 * mem_;
		U8 size_;
	};
}//namespace DoZerg

#endif