// rmDupPt3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <tchar.h>

typedef double real;

using namespace std;
int main(int argc, const char* argv[])
{
	if (argc < 3
		|| argc > 4)
	{
		cout << "Run command as:" << endl;
		cout << "\trmDupPt3 <source file> <destination file> [<epsilon specification>]" << endl;
		return -1;
	}
	else
	{
		string pathSrc = argv[1];
		string pathDst = argv[2];
		real epsilon = 0.0001;
		bool ok = true;
		if (4 == argc)
			ok = ParseReal<real>(epsilon, argv[3]);
		if (!ok)
		{
			cout << "epsilon spec is not in a valid format!!!";
			return -1;
		}

		FI2Rthree<real> i2r3;
		ok = (ok && ParseReal3(pathSrc, i2r3));
		if (!ok)
		{
			cout << "source file is not in valid either because the file does exist or the file is not in valid format!!!";
			return -1;
		}
		RmDupPt3(i2r3);

		ok = (ok && Output(i2r3, pathDst));
		if (!ok)
		{
			cout << "destination file path is not valid!!!";
			return -1;
		}
		cout << "duplications are removed SUCCESSFULLY in the destination file!!!";
		return 0;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
