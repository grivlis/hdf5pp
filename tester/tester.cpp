// tester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <hdf5pp.h>

int main()
{
#ifdef _DEBUG
	// detect memory leaks on any exit
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// break code when given id memory is allocated
	//_CrtSetBreakAlloc(59);
#endif

	const char* fn = "test2.h5";

	HDF5::File f(fn, H5F_ACC_EXCL);

    std::cout << "Hello World!\n";
}

