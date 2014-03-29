#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "Comparator.h"
#include "Entry.h"

namespace DoZerg{
	template<class __StrTbl>class Dictionary:
		public std::_Tree<
			std::_Tmap_traits<
				typename __StrTbl::U8,
				Entry<typename __StrTbl::__Platform>,
				Comparator<__StrTbl>,
				std::allocator<std::pair<
					const typename __StrTbl::U8,
					Entry<typename __StrTbl::__Platform> > >
				,false
			>
		>
	{
	public:
		typedef typename __StrTbl::__Platform __Platform;
		typedef typename __Platform::U1 U1;
		typedef typename __Platform::U4 U4;
		typedef typename __Platform::U8 U8;
		typedef Dictionary _Myt;
		typedef U8 key_type;
		typedef Entry<__Platform> mapped_type;
		typedef Entry<__Platform> referent_type;	// retained
		typedef Comparator<__StrTbl> key_compare;
		typedef std::_Tree<std::_Tmap_traits<key_type,mapped_type,key_compare,std::allocator<std::pair<const key_type,mapped_type> >,false> > _Mybase;
		typedef typename _Mybase::value_compare value_compare;
		typedef typename _Mybase::allocator_type allocator_type;
		typedef typename _Mybase::size_type size_type;
		typedef typename _Mybase::difference_type difference_type;
		typedef typename _Mybase::pointer pointer;
		typedef typename _Mybase::const_pointer const_pointer;
		typedef typename _Mybase::reference reference;
		typedef typename _Mybase::const_reference const_reference;
		typedef typename _Mybase::iterator iterator;
		typedef typename _Mybase::const_iterator const_iterator;
		typedef typename _Mybase::reverse_iterator reverse_iterator;
		typedef typename _Mybase::const_reverse_iterator const_reverse_iterator;
		typedef typename _Mybase::value_type value_type;
		//my declarations
		typedef typename key_compare::first_argument_type first_argument_type;
	private:
		iterator Lower_bound(const first_argument_type & val)
		{	// find leftmost node not less than _Keyval in mutable tree
			return (_TREE_ITERATOR(_LBound(val)));
		}
		_Nodeptr _LBound(const first_argument_type & val) const
		{	// find leftmost node not less than _Keyval
			_Nodeptr _Pnode = _Root();
			_Nodeptr _Wherenode = _Myhead;	// end() if search fails
			while (!_Isnil(_Pnode))
				if (this->comp(_Key(_Pnode), val))
					_Pnode = _Right(_Pnode);	// descend right subtree
				else{	// _Pnode not less than _Keyval, remember it
					_Wherenode = _Pnode;
					_Pnode = _Left(_Pnode);	// descend left subtree
				}
				return (_Wherenode);	// return best remembered candidate
		}
	public:
		~Dictionary(){}
		explicit Dictionary(const __StrTbl & strtbl):_Mybase(key_compare(strtbl), allocator_type()),strTbl_(strtbl){}
		iterator Query(const first_argument_type & val){	// find an element in mutable sequence that matches val
			iterator _Where = Lower_bound(val);
			return (_Where == end() || this->comp(val, _Key(_Where._Mynode())) ? end() : _Where);
		}
		bool Has(const first_argument_type & val){
			return Query(val) != end();
		}
		template<class __OutStream>
			void Print(__OutStream & out) const{
				out<<"Hash Table:\n";
				for(const_iterator i = begin();i != end();++i){
					out<<"\tid	= "<<strTbl_[i->first].c_str()<<'\n';
					i->second.Print(out);
				}
				out<<"End of Hash Table\n";
			}
		template<class __Symbol>
			void AddEntry(const __Symbol & sym,U4 index,U4 subIndex){
				insert(value_type(sym.Text(),mapped_type(U1(__Symbol::SymType),index,subIndex)));
			}
	private:
		const __StrTbl & strTbl_;
	};
}//namespace DoZerg

#endif