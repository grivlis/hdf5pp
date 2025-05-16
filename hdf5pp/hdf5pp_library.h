#pragma once

#include "hdf5pp_api.h"

namespace HDF5 {

	class HDF5PP_API Library
	{
	public:

		// Allocate memory that will later be freed internally by the HDF5 library
		static void* AllocateMemory(size_t size, bool clear);

		// Verify that HDF5 library versions are consistent
		// will call abort() if not consistent
		static bool CheckVersion(unsigned int maj_num, unsigned int min_num, unsigned int rel_num);

		// Flushes all data to disk, closes all open objects, and cleans up memory
		// Will be called automatically by the library on application exit
		static bool Close();

		// Instructs the library not to install atexit cleanup routine
		static bool DontAtexit();

		// Frees memory previously allocated by the HDF5 library
		static bool FreeMemory(void* buf);

		// Garbage collects on all free-lists and all types
		static bool GarbageCollect();

		// Returns the HDF5 library version
		static bool GetVersion(unsigned int& maj_num, unsigned int& min_num, unsigned int& rel_num);
		static bool GetVersion(std::vector<unsigned int>& version);
		static std::vector<unsigned int> GetVersion();

		// Determines whether the HDF5 library was built with the thread safety feature enabled
		static bool IsThreadSafe();

		// Initialize the HDF5 library
		// this is not needed with CPP
		static bool Open();

		// Resizes and, if required, re-allocated memory that was previously allocated by the HDF5 library
		static void* ResizeMemory(void* mem, size_t new_size);

		// Sets free-list size limits
		static bool SetFreeListLimits(int reg_global_lim, int reg_list_lim, int arr_global_lim, int arr_list_lim, int blk_global_lim, int blk_list_lim);

		// Determines whether the version of the library being used is greater than or equal to the specified version
		static bool IsVersionGE(unsigned int maj_num, unsigned int min_num, unsigned int rel_num);

		// Determines whether the version of the library being used is less than or equal to the specified version
		static bool IsVersionLE(unsigned int maj_num, unsigned int min_num, unsigned int rel_num);

		// Determines if a filter is available
		static bool FilterAvailable(H5Z_filter_t filter, bool* failed = nullptr);

		// Retrieves information about the filter
		static bool GetFilterInfo(H5Z_filter_t filter, unsigned int& info);

		// Register a filter
		static bool RegisterFilter(const H5Z_class2_t& filter_class);

		// Unregister a filter
		static bool UnregisterFilter(H5Z_filter_t filter);

		// Insert a plugin path at the end of the list
		static bool AppendPluginSearchPath(const char* plugin_path);
	};
}
