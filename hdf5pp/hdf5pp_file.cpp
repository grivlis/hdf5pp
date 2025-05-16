#include "pch.h"
#include "hdf5pp_file.h"


namespace HDF5 {

	File::File(const File& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

	File::File(const char* name, unsigned int flags, const PropertyList& fcpl /*= PropertyList()*/, const PropertyList& fapl /*= PropertyList()*/)
	{
		m_hID = H5Fcreate(name, flags, (hid_t)fcpl, (hid_t)fapl);
		if (m_hID < 0) {
			m_hID = InvalidHandle;
		}
	}

	File::~File()
	{
		Close();
	}

	File& File::operator=(const File& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool File::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_FILE == H5Iget_type(hid)) {
				DecrementReferenceCount();
				m_hID = hid;
				return true;
			}
			else {
				// hid is not a File, can't take ownership
				return false;
			}
		}
		else {
			DecrementReferenceCount();
			m_hID = InvalidHandle;
			return true;
		}
	}

	bool File::ClearExternalLinkCache()
	{
		return H5Fclear_elink_file_cache(m_hID) >= 0;
	}

	bool File::Create(const char* name, unsigned int flags, const PropertyList& fcpl /*= PropertyList()*/, const PropertyList& fapl /*= PropertyList()*/)
	{
		Close();
		m_hID = H5Fcreate(name, flags, (hid_t)fcpl, (hid_t)fapl);
		if (m_hID < 0) {
			m_hID = InvalidHandle;
		}
		return m_hID >= 0;
	}

	bool File::Close()
	{
		if (m_hID >= 0) {
			auto rv = H5Fclose(m_hID);
			m_hID = InvalidHandle;
			return rv >= 0;
		}
		else {
			return true; // already closed
		}
	}

	bool Delete(const char* filename, const PropertyList& fapl /*= PropertyList()*/)
	{
		return H5Fdelete(filename, (hid_t)fapl) >= 0;
	}

	FileCreationPropertyList File::GetCreationPropertyList()
	{
		return FileCreationPropertyList(H5Fget_create_plist(m_hID));
	}

	FileAccessPropertyList File::GetAccessPropertyList()
	{
		return FileAccessPropertyList(H5Fget_access_plist(m_hID));
	}

	bool File::GetFileno(unsigned long& fnumber)
	{
		return H5Fget_fileno(m_hID, &fnumber) >= 0;
	}

	bool File::GetEOA(haddr_t& eoa)
	{
		return H5Fget_eoa(m_hID, &eoa) >= 0;
	}

	ssize_t File::GetFileImageSize()
	{
		return H5Fget_file_image(m_hID, nullptr, 0);
	}

	ssize_t File::GetFileImage(void* buf, size_t buf_len)
	{
		return H5Fget_file_image(m_hID, buf, buf_len);
	}

	bool File::GetFileSize(hsize_t& size)
	{
		return H5Fget_filesize(m_hID, &size) >= 0;
	}

	ssize_t File::GetFreeSections(MemoryAllocationType t, size_t nsects, H5F_sect_info_t& sect_info)
	{
		return H5Fget_free_sections(m_hID, (H5F_mem_t)t, nsects, &sect_info);
	}

	hssize_t File::GetFileFreeSpace()
	{
		return H5Fget_freespace(m_hID);
	}

	bool File::GetIntent(unsigned int& intent)
	{
		return H5Fget_intent(m_hID, &intent) >= 0;
	}

	bool File::GetMDCConfig(H5AC_cache_config_t& cfg)
	{
		return H5Fget_mdc_config(m_hID, &cfg) >= 0;
	}

	bool File::SetMDCConfig(H5AC_cache_config_t& cfg)
	{
		return H5Fset_mdc_config(m_hID, &cfg) >= 0;
	}

	bool File::GetMDCHitRate(double& rate)
	{
		return H5Fget_mdc_hit_rate(m_hID, &rate) >= 0;
	}

	bool File::ResetMDCHitRateStats()
	{
		return H5Freset_mdc_hit_rate_stats(m_hID) >= 0;
	}

	bool File::GetMDCImageInfo(haddr_t& image_addr, hsize_t& image_len)
	{
		return H5Fget_mdc_image_info(m_hID, &image_addr, &image_len) >= 0;
	}

	bool File::GetMDCLoggingStatus(bool& is_enabled, bool& is_currently_logging)
	{
		return H5Fget_mdc_logging_status(m_hID, &is_enabled, &is_currently_logging) >= 0;
	}

	bool File::StartMDCLogging()
	{
		return H5Fstart_mdc_logging(m_hID) >= 0;
	}

	bool File::StopMDCLogging()
	{
		return H5Fstop_mdc_logging(m_hID) >= 0;
	}

	bool File::GetMDCSize(size_t* max_size, size_t* min_clean_size, size_t* cur_size, int* cur_num_entries)
	{
		return H5Fget_mdc_size(m_hID, max_size, min_clean_size, cur_size, cur_num_entries) >= 0;
	}

	bool File::GetMetadataReadEntryInfo(H5F_retry_info_t& rinfo)
	{
		return H5Fget_metadata_read_retry_info(m_hID, &rinfo) >= 0;
	}

#ifdef H5_HAVE_PARALLEL
	bool File::GetMPIAtomicity(bool& flag)
	{
		return H5Fget_mpi_atomicity(m_hID, &flag);
	}

	bool File::SetMPIAtomicity(bool flag)
	{
		return H5Fset_mpi_atomicity(m_hID, flag);
	}
#endif

	ssize_t File::GetObjectsCount(unsigned int types)
	{
		return H5Fget_obj_count(m_hID, types);
	}

	ssize_t File::GetAllObjectsCount(unsigned int types)
	{
		return H5Fget_obj_count(H5F_OBJ_ALL, types);
	}



	ssize_t File::GetOpenObjects(unsigned int types, size_t max_objs, hid_t* obj_id_list)
	{
		return H5Fget_obj_ids(m_hID, types, max_objs, obj_id_list);
	}

	ssize_t File::GetAllOpenObjects(unsigned int types, size_t max_objs, hid_t* obj_id_list)
	{
		return H5Fget_obj_ids(H5F_OBJ_ALL, types, max_objs, obj_id_list);
	}

	bool File::GetPageBufferingStatus(BufferingStats& stats)
	{
		return H5Fget_page_buffering_stats(m_hID, stats.accesses, stats.hits, stats.misses, stats.evictions, stats.bypasses) >= 0;
	}

	bool File::ResetPageBufferingStats()
	{
		return H5Freset_page_buffering_stats(m_hID) >= 0;
	}

	bool File::GetVFDHandle(const PropertyList& fapl, void** file_handle)
	{
		return H5Fget_vfd_handle(m_hID, (hid_t)fapl, file_handle) >= 0;
	}

	bool File::IncrementFileSize(hsize_t increment)
	{
		return H5Fincrement_filesize(m_hID, increment) >= 0;
	}

	bool File::IsHDF5(const char* name)
	{
		return H5Fis_hdf5(name) > 0;
	}

	htri_t File::IsAccessible(const char* name, const PropertyList& fapl /*= PropertyList()*/)
	{
		return H5Fis_accessible(name, (hid_t)fapl);
	}

	bool File::Open(const char* name, unsigned int flags, const PropertyList& fapl /*= PropertyList()*/)
	{
		Close();
		m_hID = H5Fopen(name, flags, (hid_t)fapl);
		if (m_hID < 0) {
			m_hID = InvalidHandle;
		}
		return m_hID >= 0;
	}

	File File::Reopen()
	{
		return File(H5Freopen(m_hID));
	}

	bool File::SetLibraryVersionBounds(H5F_libver_t low, H5F_libver_t high)
	{
		return H5Fset_libver_bounds(m_hID, low, high) >= 0;
	}

	bool File::StartSWMRWrite()
	{
		return H5Fstart_swmr_write(m_hID) >= 0;
	}

#ifdef _DEBUG
	File::File(hid_t hid)
	{
		m_hID = InvalidHandle;
		if (hid >= 0) {
			if (H5I_FILE == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not a File");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::File");
#endif
			}
		}
	}
#else
	File::File(hid_t hid) : Group(hid)
	{

	}
#endif

}
