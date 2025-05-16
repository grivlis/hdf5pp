// test2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <hdf5pp.h>

int main()
{
	const char* fn = "test2.h5";

	HDF5::File f(fn, H5F_ACC_EXCL);


	auto grp = f.CreateGroup("group1");
	auto aa = f.CreateAttribute("attr1", HDF5::IntegerPDT::Native_LONG, HDF5::Dataspace());
	long v{ -123 };
	aa.Write(HDF5::IntegerPDT::Native_LONG, &v);
	auto ab = grp.CreateAttribute("aa1", HDF5::FloatPDT::Native_DOUBLE, HDF5::Dataspace());
	double val{ 3.1415 };
	ab.Write(HDF5::FloatPDT::Native_DOUBLE, &val);

	short v2{ -16 };
	grp.AddAttribute("test_short", v2);
	grp.AddAttribute("test_double", val);
	grp.AddAttribute("test_long", v);
	std::vector<int> vv = { 1, 2, 3, 4, 5 };
	grp.AddAttribute("test_int_array", vv);
	grp.AddAttribute("test_int_p2i", vv.data(), vv.size());

	// attempt to rewrite attribute
	std::vector<double> foo = { 1.2, 2.3, 3.4, 4.5 };
	grp.SetAttribute("test_double", foo);

	// test string attribute
	grp.AddAttributeFormat("test_str_with_format", "this is a test string with %d", v2);

	// test wchar_t string
	grp.AddAttributeFormat("test_wchar_str", L"wide char with %g", val);
	grp.AddAttributeFormat("test_wchar_str2", L"\u03b1\u03b2\u03b3\u03b4");

	double f2{ 17 };
	grp.AddAttribute("ttttt", f2);
	double foo2{ 0 };
	grp.ReadAttribute("ttttt", foo2);

	std::vector<double> bar;
	grp.ReadAttribute("test_double", bar);

	std::string str;
	grp.ReadAttribute("test_str_with_format", str);

	std::wstring wstr;
	grp.ReadAttribute("test_wchar_str2", wstr);


	std::vector<unsigned long> vvv;
	for (long i = 1; i <= 3; ++i) {
		for (long j = 1; j <= 4; ++j) {
			for (long k = 1; k <= 5; ++k) {
				vvv.push_back(i * j * k);
			}
		}
	}
	grp.AddDataset("dset_1d", vvv);
	std::vector<size_t> dims;
	dims.push_back(3);
	dims.push_back(4 * 5);
	grp.AddDataset("dset_2d", vvv, dims);
	dims = { 3, 4, 5 };
	grp.AddDataset("dset_3d", vvv, dims);

	grp.AddDataset("ddset_1d", vvv.data(), vvv.size());
	grp.AddDataset("ddset_2d", vvv.data(), 3, 4 * 5);
	grp.AddDataset("ddset_3d", vvv.data(), 3, 4, 5);

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
