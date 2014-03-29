#ifndef INTMATHCALL_H
#define INTMATHCALL_H

#include <cmath>
#include <cfloat>

namespace DoZerg{
	template<class __Res>class IntMathCall
	{
		typedef typename __Res::S8 S8;
	public:
		~IntMathCall(){}
		explicit IntMathCall(__Res & res):suc_(Handle(res)){}
		operator bool() const{return suc_;}
	private:
		bool Handle(__Res & res){
			switch(res.R1){
				case 0:{
					res.R3 = _atoi64(res.RAMPosition<char>(res.R2));
					break;}
				case 1:{
					res.D1 = std::atof(res.RAMPosition<char>(res.R2));
					break;}
				case 2:{
					std::sprintf(res.RAMPosition<char>(res.R3),"%I64d",res.R2);
					break;}
				case 3:{
					std::sprintf(res.RAMPosition<char>(res.R3),"%e",res.D1);
					break;}
				case 4:{
					switch(_fpclass(res.D1)){
						case _FPCLASS_NINF:{res.R2 = 0;break;}		//negative infinity
						case _FPCLASS_NN:{res.R2 = 1;break;}		//egative normal
						case _FPCLASS_ND:{res.R2 = 2;break;}		//negative denormal
						case _FPCLASS_NZ:{res.R2 = 3;break;}		//-0
						case _FPCLASS_PZ:{res.R2 = 4;break;}		//+0
						case _FPCLASS_PD:{res.R2 = 5;break;}		//positive denormal
						case _FPCLASS_PN:{res.R2 = 6;break;}		//positive normal
						case _FPCLASS_PINF:{res.R2 = 7;break;}		//positive infinity
						case _FPCLASS_SNAN:							//signaling NaN
						case _FPCLASS_QNAN:							//quiet NaN
						default:{res.R2 = 8;}
					}break;}
				case 5:{
					S8 val = res.R2;
					res.R3 = (val < 0 ? -val : val);
					break;}
				case 6:{
					res.D2 = std::fabs(res.D1);
					break;}
				case 7:{
					res.D2 = std::ceil(res.D1);
					break;}
				case 8:{
					res.D2 = std::floor(res.D1);
					break;}
				case 9:{
					res.D2 = std::exp(res.D1);
					break;}
				case 10:{
					res.D2 = std::log(res.D1);
					break;}
				case 11:{
					res.D2 = std::log10(res.D1);
					break;}
				case 12:{
					res.D3 = std::pow(res.D1,res.D2);
					break;}
				case 13:{
					res.D2 = std::sqrt(res.D1);
					break;}
				case 14:{
					res.D2 = std::cos(res.D1);
					break;}
				case 15:{
					res.D2 = std::sin(res.D1);
					break;}
				case 16:{
					res.D2 = std::tan(res.D1);
					break;}
				case 17:{
					res.D2 = std::acos(res.D1);
					break;}
				case 18:{
					res.D2 = std::asin(res.D1);
					break;}
				case 19:{
					res.D2 = std::atan(res.D1);
					break;}
				case 20:{
					res.D2 = std::cosh(res.D1);
					break;}
				case 21:{
					res.D2 = std::sinh(res.D1);
					break;}
				case 22:{
					res.D2 = std::tanh(res.D1);
					break;}
				default:{
					res.errFile<<BeginError
						<<"IntMathCall: INT 8, R1="<<res.R1<<" function not handled"
						<<EndError;
					return false;
				}
			}
			return true;
		}
	private:
		bool suc_;
	};
}//namespace DoZerg

#endif