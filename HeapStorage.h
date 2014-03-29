#ifndef HEAPSTORAGE_H
#define HEAPSTORAGE_H

#include <cassert>
#include "MemStatus.h"

namespace DoZerg{
	template<typename __Byte,typename __Size>class HeapStorage
	{
		typedef MemStatus<__Byte> __MemStat;
		static const __Size HeaderSize = 1 + 2 * sizeof(__Size);
		static const __Size MinFreeBytes = 8;
	public:
		static const __Size BadLocation = -1;	//out of range (0 - size_)
	public:
		HeapStorage():mem_(0),size_(0){}
		~HeapStorage(){}
		void Initialize(__Byte * mem,__Size size){
			mem_ = mem;
			size_ = size;
			Prev(0) = BadLocation;
			Next(0) = size_;
			SetStatus(0,__MemStat::Free);
		}
		__Size Allocate(__Size size) const{	//if failed, return BadLocation
			__Size loc = 0;
			if(!Alloc(loc,size))
				do{
					loc = Next(loc);
				}while(loc < size_ && !Alloc(loc,size));
			return (loc < size_ ? loc : BadLocation);
		}
		bool Deallocate(__Size loc) const{
			if(!CheckRange(loc))
				return false;
			if(Status(Prev(loc)) == __MemStat::Free)
				loc = Merge(Prev(loc),loc);
			if(Status(Next(loc)) == __MemStat::Free)
				loc = Merge(loc,Next(loc));
			SetStatus(loc,__MemStat::Free);
			return true;
		}
		template<class __OutStream>
			void Print(__OutStream & out,__Size offset) const{
				out<<"--HEAP--\n"
					<<"start at address: "<<offset
					<<"\ntotal size: "<<size_
					<<"\n\tPrev\tCurrent\tNext\tStatus\t\tSize\n";
				for(__Size loc = 0;loc < size_;loc = Next(loc)){
					out<<"[\t";
					if(Prev(loc) == BadLocation)
						out<<"begin";
					else
						out<<(Prev(loc) + offset);
					out<<",\t"<<(loc + offset)<<",\t";
					if(Next(loc) == size_)
						out<<"end";
					else
						out<<(Next(loc) + offset);
					out<<",\t"<<(Status(loc) == __MemStat::Free ? "Free\t" : "Reserved")
						<<"]\t"<<Size(loc)<<"\n";
				}
				out<<"--END--\n";
			}
	private:
		bool CheckRange(__Size index) const{return (index >= 0 && index < size_);}
		__Size & Prev(__Size index) const{		//previous chunk's index
			assert(CheckRange(index) && "HeapStorage::Prev()");
			return *reinterpret_cast<__Size *>(mem_ + index);
		}
		__Size & Next(__Size index) const{		//next chunk's index
			assert(CheckRange(index) && "HeapStorage::Next()");
			return *reinterpret_cast<__Size *>(mem_ + index + sizeof(__Size));
		}
		__Byte Status(__Size index) const{	//chunk status
			if(!CheckRange(index))
				return __MemStat::Reserved;
			return mem_[index + sizeof(__Size) * 2];
		}
		void SetStatus(__Size index,const __MemStat & stat) const{
			assert(CheckRange(index) && "HeapStorage::SetStatus()");
			mem_[index + sizeof(__Size) * 2] = stat;
		}
		__Size Size(__Size index) const{		//chunk size
			return (CheckRange(index) ? Next(index) - index - HeaderSize : 0);
		}
		bool Alloc(__Size loc,__Size size) const{
			if(!CheckRange(loc) || Status(loc) == __MemStat::Reserved || Size(loc) < size)
				return false;
			if(Size(loc) >= size + HeaderSize + MinFreeBytes){
				__Size new_loc = loc + size + HeaderSize;
				Prev(new_loc) = loc;
				Next(new_loc) = Next(loc);
				SetStatus(new_loc,__MemStat::Free);
				Next(loc) = new_loc;
			}
			SetStatus(loc,__MemStat::Reserved);
			return true;
		}
		__Size Merge(__Size loc1,__Size loc2) const{		//merge two chunks,loc1 < loc2
			assert(loc1 < loc2 && "HeapStorage::Merge()");
			if(!CheckRange(loc1))
				return loc2;
			else if(CheckRange(loc2)){
				__Size next_loc2 = Next(loc2);
				Next(loc1) = next_loc2;
				if(CheckRange(next_loc2))
					Prev(Next(loc2)) = loc1;
			}
			return loc1;
		}
	private:
		__Byte * mem_;
		__Size size_;
	};
}//namespace DoZerg

#endif