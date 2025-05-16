#pragma once

#include "hdf5pp_api.h"

namespace HDF5 {

	class HDF5PP_API IntegerPDT;
	class HDF5PP_API FloatPDT;

	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned char);
	HDF5PP_API const IntegerPDT& DatatypeOf(signed char);
	HDF5PP_API const IntegerPDT& DatatypeOf(short);
	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned short);
	HDF5PP_API const IntegerPDT& DatatypeOf(int);
	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned int);
	HDF5PP_API const IntegerPDT& DatatypeOf(long);
	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned long);
	HDF5PP_API const IntegerPDT& DatatypeOf(long long);
	HDF5PP_API const IntegerPDT& DatatypeOf(unsigned long long);
	HDF5PP_API const FloatPDT& DatatypeOf(float);
	HDF5PP_API const FloatPDT& DatatypeOf(double);
	HDF5PP_API const FloatPDT& DatatypeOf(long double);

}
