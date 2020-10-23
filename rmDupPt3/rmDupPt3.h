#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <time.h>
#include <assert.h>

template<typename Real>
struct R3
{
	R3() : dup(false)
		 , v{0, 0, 0}
	{
	}
	R3(Real v0, Real v1, Real v2)
		: dup(false)
		, v{v0, v1, v2}
	{
	}

	Real norm_sqr()
	{
		return v[0] * v[0]
			 + v[1] * v[1]
			 + v[2] * v[2];
	}

	Real v[3];
	bool dup;
};

template<typename Real> R3<Real> operator-(const R3<Real>& first
										, const R3<Real>& second)
{
	R3<Real> ret(first.v[0] - second.v[0]
			, first.v[1] - second.v[1]
			, first.v[2] - second.v[2]);
	return ret;
}

template<typename Real>
struct FI2R3
{
	std::vector<R3<Real>> vecR3;
	typedef Real (*String2Real)(const std::string& str, std::size_t* pos);
	static String2Real string2real;
};

typedef std::size_t Idx;

template<typename Real>
using FR2I = std::multimap<Real, Idx>;

typedef Idx DEG_n;
typedef std::map<Idx, DEG_n> FI2N;

struct Edge : public std::pair<Idx, Idx>
{
	Edge(Idx i, Idx j)
	{
		assert(i != j);
		if (i < j)
		{
			first = i;
			second = j;
		}
		else
		{
			first = j;
			second = i;
		}
	}
};

typedef std::list<Edge> Edges;


template<typename Real>
bool ParseReal(Real& r, const char* r_spec, int len_spec = -1)
{
	try
	{
		if (-1 == len_spec)
		{
			std::string r_spec2(r_spec);
			r = FI2R3<Real>::string2real(r_spec2, nullptr);
		}
		else
		{
			std::string r_spec2(r_spec, len_spec);
			r = FI2R3<Real>::string2real(r_spec2, nullptr);
		}
		return true;
	}
	catch(std::exception& )
	{
		return false;
	}
}

template<typename Real>
bool ParseReal3(FI2R3<Real>& i2r3, const char* path_spec)
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
void TagDupPt3(FI2R3<Real>& f_i2r3, Real epsilon)
{
	srand(time(NULL)); //radomly pick among {x, y, z} for screening.
	Real epsilon_sqr = epsilon * epsilon;
	int i_f = rand() % 3;
	FR2I<Real> f_r2i;
	for (Idx i_n = 0; i_n < f_i2r3.vecR3.size(); i_n ++)
	{
		std::pair<Real, Idx> r2i(f_i2r3.vecR3[i_n].v[i_f], i_n);
		f_r2i.insert(r2i);
	}
	Edges edges;
	auto it_j_end = f_r2i.end();
	auto it_i_end = it_j_end; it_i_end --;
	for (auto it_i = f_r2i.begin()
		; it_i != it_i_end
		; it_i ++)
	{
		auto r_i = it_i->first;
		auto i = it_i->second;
		auto r3_i = f_i2r3.vecR3[i];
		auto it_j = it_i; it_j ++;
		bool in_region = true;
		for (
			; it_j != it_j_end && in_region
			; it_j ++)
		{
			auto r_j = it_j->first;
			in_region = ((r_j - r_i) < epsilon);
			if (in_region)
			{
				auto j = it_j->second;
				auto r3_j = f_i2r3.vecR3[j];
				if ((r3_j - r3_i).norm_sqr() < epsilon_sqr)
				{
					Edge e(i, j);
					edges.push_back(e);
				}
			}
		}
	}

#ifdef _DEBUG
	std::cout << "Edges are within epsilon range:" <<std::endl;
	for (auto it_e = edges.begin()
		; it_e != edges.end()
		; it_e ++)
	{
		printf("\t(%5d, %5d)\n", it_e->first+1, it_e->second+1);
	}
#endif

	FI2N vert_deg;
	for (auto it_e = edges.begin()
		; it_e != edges.end()
		; it_e ++)
	{
		++ vert_deg[it_e->first];
		++ vert_deg[it_e->second];
	}

	std::multimap<DEG_n, Edge> deg_e;
	for (auto it_e = edges.begin()
		; it_e != edges.end()
		; it_e ++)
	{
		DEG_n deg = std::max(vert_deg[it_e->first], vert_deg[it_e->second]);
		deg_e.insert(std::pair<DEG_n, Edge>(deg, *it_e));
	}

	for (auto it_deg_e = deg_e.end()
		; it_deg_e != deg_e.begin()
		; )
	{
		it_deg_e --;
		const Edge& e = it_deg_e->second;
		Idx i = e.first;
		Idx j = e.second;
		R3<Real>& p_i = f_i2r3.vecR3[i];
		R3<Real>& p_j = f_i2r3.vecR3[j];
		bool not_tagged = (!p_i.dup
						&& !p_j.dup);
		if (not_tagged)
		{
			if (it_deg_e->first == vert_deg[i])
				p_i.dup = true;
			else
				p_j.dup = true;
		}
	}

}

template<typename Real>
bool Output(const FI2R3<Real>& i2r3, const char* path_spec)
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