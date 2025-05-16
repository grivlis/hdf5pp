#include "pch.h"
#include "hdf5pp_library.h"

namespace HDF5 {

	void* Library::AllocateMemory(size_t size, bool clear)
	{
		return H5allocate_memory(size, clear);
	}

	bool Library::CheckVersion(unsigned int maj_num, unsigned int min_num, unsigned int rel_num)
	{
		return H5check_version(maj_num, min_num, rel_num) >= 0;
	}

	bool Library::Close()
	{
		return H5close() >= 0;
	}

	bool Library::DontAtexit()
	{
		return H5dont_atexit() >= 0;
	}

	bool Library::FreeMemory(void* buf)
	{
		return H5free_memory(buf) >= 0;
	}

	bool Library::GarbageCollect()
	{
		return H5garbage_collect() >= 0;
	}

	bool Library::GetVersion(unsigned int& maj_num, unsigned int& min_num, unsigned int& rel_num)
	{
		return H5get_libversion(&maj_num, &min_num, &rel_num) >= 0;
	}

	bool Library::GetVersion(std::vector<unsigned int>& version)
	{
		version.resize(3);
		return H5get_libversion(&version[0], &version[1], &version[2]) >= 0;
	}

	std::vector<unsigned int> Library::GetVersion()
	{
		std::vector<unsigned int> version(3);
		H5get_libversion(&version[0], &version[1], &version[2]);
		return version;
	}

	bool Library::IsThreadSafe()
	{
		bool b{ false };
		if (H5is_library_threadsafe(&b) < 0) {
			b = false;
		}
		return b;
	}

	bool Library::Open()
	{
		return H5open() >= 0;
	}

	void* Library::ResizeMemory(void* mem, size_t new_size)
	{
		return H5resize_memory(mem, new_size);
	}

	bool Library::SetFreeListLimits(int reg_global_lim, int reg_list_lim, int arr_global_lim, int arr_list_lim, int blk_global_lim, int blk_list_lim)
	{
		return H5set_free_list_limits(reg_global_lim, reg_list_lim, arr_global_lim, arr_list_lim, blk_global_lim, blk_list_lim) >= 0;
	}

	bool Library::IsVersionGE(unsigned int maj_num, unsigned int min_num, unsigned int rel_num)
	{
		return H5_VERSION_GE(maj_num, min_num, rel_num);
	}

	bool Library::IsVersionLE(unsigned int maj_num, unsigned int min_num, unsigned int rel_num)
	{
		return H5_VERSION_LE(maj_num, min_num, rel_num);
	}

	bool Library::FilterAvailable(H5Z_filter_t filter, bool* failed /*= nullptr*/)
	{
		auto rv = H5Zfilter_avail(filter);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv > 0;
	}

	bool Library::GetFilterInfo(H5Z_filter_t filter, unsigned int& info)
	{
		return H5Zget_filter_info(filter, &info) >= 0;
	}

	bool Library::RegisterFilter(const H5Z_class2_t& filter_class)
	{
		return H5Zregister(&filter_class) >= 0;
	}

	bool Library::UnregisterFilter(H5Z_filter_t filter)
	{
		return H5Zunregister(filter) >= 0;
	}

	bool Library::AppendPluginSearchPath(const char* plugin_path)
	{
		return H5PLappend(plugin_path) >= 0;
	}

}
