#pragma once

template<typename Real>
bool ParseReal(Real& r, const char* r_spec, int len_spec = -1)
{
	return false;
}

template<typename Real>
struct FI2Rthree
{
	Real r[3];
	bool dup;
};

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