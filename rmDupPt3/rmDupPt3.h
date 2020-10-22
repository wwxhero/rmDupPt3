#pragma once
#include <string>

template<typename Real>
struct FI2Rthree
{
	Real r[3];
	bool dup;
	typedef Real (*String2Real)(const std::string& str, std::size_t* pos);
	static String2Real string2real;
};

template<typename Real>
bool ParseReal(Real& r, const char* r_spec, int len_spec = -1)
{
	try
	{
		if (-1 == len_spec)
		{
			std::string r_spec2(r_spec);
			r = FI2Rthree<Real>::string2real(r_spec2, nullptr);
		}
		else
		{
			std::string r_spec2(r_spec, len_spec);
			r = FI2Rthree<Real>::string2real(r_spec2, nullptr);
		}
		return true;
	}
	catch(std::exception& e)
	{
		return false;
	}
}

template<typename Real>
bool ParseReal3(Real& r, const char* path_spec)
{
	return false;
}

template<typename Real>
void TagDupPt3(FI2Rthree<Real>& i2r3)
{
}

template<typename Real>
bool Output(const FI2Rthree<Real>& i2r3, const char* path_spec)
{
	return false;
}