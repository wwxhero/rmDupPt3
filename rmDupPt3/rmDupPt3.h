#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

template<typename Real>
struct R3
{
	R3() : dup(false)
		 , v{0, 0, 0}
	{

	}
	Real v[3];
	bool dup;
};

template<typename Real>
struct FI2Rthree
{
	std::vector<R3<Real>> vecR3;
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
	catch(std::exception& )
	{
		return false;
	}
}

template<typename Real>
bool ParseReal3(FI2Rthree<Real>& i2r3, const char* path_spec)
{
	//this is not a robust parser
	//, but is confined only with the specified format in BNF
	//, <file> := null
	//			| <r>, <r>, <r>
	//			| <file>\n<r>, <r>, <r>
	bool parsed = true;

	std::ifstream infile(path_spec);
	parsed = !infile.fail();
	std::string line;
	while (std::getline(infile, line)
		&& parsed)
	{
		std::istringstream iss(line);
		R3<Real> r3;
		char token[2]; //<r0>, <r1>, <r2>
		if(iss >> r3.v[0]
			   >> token[0]
			   >> r3.v[1]
			   >> token[1]
			   >> r3.v[2])
		{
			i2r3.vecR3.push_back(r3);
			parsed = true;
		}
	}

	return parsed;
}

template<typename Real>
void TagDupPt3(FI2Rthree<Real>& i2r3)
{
}

template<typename Real>
bool Output(const FI2Rthree<Real>& i2r3, const char* path_spec)
{
	std::ofstream outfile(path_spec);
	if (!outfile.fail())
	{
		for (auto it = i2r3.vecR3.begin()
			; it != i2r3.vecR3.end()
			; it ++)
		{
			if (!it->dup)
				outfile << it->v[0]
						<< ", "
						<< it->v[1]
						<< ", "
						<< it->v[2]
						<< std::endl;
		}
		return true;
	}
	else
		return false;
}