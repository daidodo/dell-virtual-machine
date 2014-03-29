//*Debug
//#define VM_DBG_DELLVM
#define VM_DBG_EXECMDLINE
#define VM_DBG_RESOURCES
//#define VM_DBG_SYMBALS
//#define VM_DBG_INSSET
//#define VM_DBG_INS
//#define VM_DBG_ERRFILE
//#define VM_DBG_BLDCMDLINE
//#define VM_DBG_PASS
//#define VM_DBG_INTERRUPT
//*/
#include "DEllVM.h"

using namespace std;
using namespace DoZerg;

struct TT{		//platform
	typedef int INT;
};
template<class T>struct A{	//__Res
	typedef typename T::INT INT;
};
struct B{	//LBI
	template<class T>
		static void f(T & a,typename T::INT & i){}
		template<class T>
			static void g(const T & a){}
};
template<class T>struct C{	//Ins
	typedef void (*Fptr)(T &,typename T::INT &);
	typedef void (*Gptr)(const T &);
	Fptr fptr;
	Gptr gptr;
	C(Fptr f,Gptr g):fptr(f),gptr(g){}
};

int main(){
	Win32DEllVM vm;
	if(1){	//Build
		cout<<"Compile...";
		const argc = 3;
		const char str[argc][20] = {"C:\\advance.asm","-l","-o=C:\\Hec.RUN"};
		cout<<str[0]<<'\n';
		Win32DEllVM::__BldArgs args(str,str + argc);
		bool suc = vm.Build(args);
		cout<<(suc ? "SUCCESSFUL\n" : "FAILED\n");
		if(!suc)
			return 0;
	}
	if(1){	//Execute
		cout<<"Execute...\n";
		const argc = 3;
		const char str[argc][20] = {"-h=100","-d","C:\\Hec.RUN"};
		Win32DEllVM::__ExeArgs args(str,str + argc);
		cout<<(vm.Execute(args)?"SUCCESSFUL\n":"FAILED\n");
	}
	/*	typedef A<TT> _A;
	_A a;
	int x;
	B::f(a,x);
	B::g(a);

	C<_A> c(B::f,B::g);
	c.fptr(a,x);
	c.gptr(a);*/
}