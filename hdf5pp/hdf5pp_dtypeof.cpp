#include "pch.h"
#include "hdf5pp_dtypeof.h"
#include "hdf5pp_dtype.h"

namespace HDF5 {


	HDF5PP_API const IntegerPDT& DatatypeOf(char)
	{
		return IntegerPDT::Native_CHAR;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned char)
	{
		return IntegerPDT::Native_UCHAR;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(signed char)
	{
		return IntegerPDT::Native_SCHAR;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(short)
	{
		return IntegerPDT::Native_SHORT;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned short)
	{
		return IntegerPDT::Native_USHORT;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(int)
	{
		return IntegerPDT::Native_INT;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned int)
	{
		return IntegerPDT::Native_UINT;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(long)
	{
		return IntegerPDT::Native_LONG;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned long)
	{
		return IntegerPDT::Native_ULONG;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(long long)
	{
		return IntegerPDT::Native_LLONG;
	}

	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned long long)
	{
		return IntegerPDT::Native_ULLONG;
	}

	HDF5PP_API const FloatPDT& DatatypeOf(float)
	{
		return FloatPDT::Native_FLOAT;
	}

	HDF5PP_API const FloatPDT& DatatypeOf(double)
	{
		return FloatPDT::Native_DOUBLE;
	}

	HDF5PP_API const FloatPDT& DatatypeOf(long double)
	{
		return FloatPDT::Native_LDOUBLE;
	}

}