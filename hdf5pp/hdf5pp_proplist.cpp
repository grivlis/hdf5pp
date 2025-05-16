#include "pch.h"
#include "hdf5pp_proplist.h"

namespace HDF5 {


	PropertyList::PropertyList() : Handle(H5P_DEFAULT)
	{

	}

	PropertyList::PropertyList(const PropertyList& rhs)
	{
		m_hID = H5Pcopy(rhs.m_hID);
	}

	PropertyList::PropertyList(void* buf)
	{
		m_hID = H5Pdecode(buf);
		if (m_hID < 0) {
			m_hID = InvalidHandle;
		}
	}

	PropertyList::~PropertyList()
	{
		H5Pclose(m_hID);
		m_hID = InvalidHandle;
	}

	PropertyList& PropertyList::operator=(const PropertyList& rhs)
	{
		if (this != &rhs) {
			H5Pclose(m_hID);
			m_hID = H5Pcopy(rhs.m_hID);
		}
		return *this;
	}

	PropertyList PropertyList::Copy()
	{
		return PropertyList(H5Pcopy(m_hID));
	}

	size_t PropertyList::GetRequiredEncodeBufferSize(const PropertyList& fapl /*= PropertyList()*/)
	{
		size_t nalloc{ 0 };
		auto rv = H5Pencode(m_hID, nullptr, &nalloc, (hid_t)fapl);
		return rv >= 0 ? nalloc : 0;
	}

	bool PropertyList::Encode(void* buf, size_t& nalloc, const PropertyList& fapl /*= PropertyList()*/)
	{
		return H5Pencode(m_hID, buf, &nalloc, (hid_t)fapl) >= 0;
	}

	PropertyList PropertyList::Decode(void* buf)
	{
		return PropertyList(buf);
	}

	bool PropertyList::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				H5Pclose(m_hID);
				m_hID = hid;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			H5Pclose(m_hID);
			m_hID = H5P_DEFAULT;
			return true;
		}
	}

#ifdef _DEBUG
	PropertyList::PropertyList(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not a PropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::PropertyList");
#endif
			}
		}
		else {
			m_hID = H5P_DEFAULT;
		}
	}
#else
	PropertyList::PropertyList(hid_t hid) : Handle(hid)
	{

	}
#endif

#ifdef _DEBUG
	AttributeCreationPropertyList::AttributeCreationPropertyList(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_ATTRIBUTE_CREATE) > 0) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a AttributeCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::AttributeCreationPropertyList");
#endif
				}
			}
			else {
				assert(false && L"input hid_t is not a AttributeCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::AttributeCreationPropertyList");
#endif
			}
		}
	}
#else
	AttributeCreationPropertyList::AttributeCreationPropertyList(hid_t hid) : PropertyList(hid)
	{

	}
#endif

	AttributeCreationPropertyList::AttributeCreationPropertyList()
	{
		m_hID = H5Pcreate(H5P_ATTRIBUTE_CREATE);
	}

	AttributeCreationPropertyList::AttributeCreationPropertyList(const AttributeCreationPropertyList& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}


	AttributeCreationPropertyList::~AttributeCreationPropertyList()
	{
		// let PropertyList do its thing
	}

	bool AttributeCreationPropertyList::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_ATTRIBUTE_CREATE) > 0) {
					H5Pclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Pclose(m_hID);
			m_hID = InvalidHandle;
			return false;
		}
	}


#ifdef _DEBUG
	GroupCreationPropertyList::GroupCreationPropertyList(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_GROUP_CREATE) > 0) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a GroupCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::GroupCreationPropertyList");
#endif
				}
			}
			else {
				assert(false && L"input hid_t is not a GroupCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::GroupCreationPropertyList");
#endif
			}
		}
	}
#else
	GroupCreationPropertyList::GroupCreationPropertyList(hid_t hid) : PropertyList(hid)
	{

	}
#endif

	GroupCreationPropertyList::GroupCreationPropertyList()
	{
		m_hID = H5Pcreate(H5P_GROUP_CREATE);
	}

	GroupCreationPropertyList::GroupCreationPropertyList(const GroupCreationPropertyList& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}


	GroupCreationPropertyList::~GroupCreationPropertyList()
	{
		// let PropertyList do its thing
	}

	bool GroupCreationPropertyList::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_GROUP_CREATE) > 0) {
					H5Pclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Pclose(m_hID);
			m_hID = InvalidHandle;
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	DatasetCreationPropertyList::DatasetCreationPropertyList(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_DATASET_CREATE) > 0) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a DatasetCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::DatasetCreationPropertyList");
#endif
				}
			}
			else {
				assert(false && L"input hid_t is not a DatasetCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::DatasetCreationPropertyList");
#endif
			}
		}
	}
#else
	DatasetCreationPropertyList::DatasetCreationPropertyList(hid_t hid) : PropertyList(hid)
	{

	}
#endif

	DatasetCreationPropertyList::DatasetCreationPropertyList()
	{
		m_hID = H5Pcreate(H5P_DATASET_CREATE);
	}

	DatasetCreationPropertyList::DatasetCreationPropertyList(const DatasetCreationPropertyList& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}


	DatasetCreationPropertyList::~DatasetCreationPropertyList()
	{
		// let PropertyList do its thing
	}

	bool DatasetCreationPropertyList::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_DATASET_CREATE) > 0) {
					H5Pclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Pclose(m_hID);
			m_hID = InvalidHandle;
			return false;
		}
	}



	//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	DatasetAccessPropertyList::DatasetAccessPropertyList(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_DATASET_ACCESS) > 0) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a DatasetAccessPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::DatasetAccessPropertyList");
#endif
				}
			}
			else {
				assert(false && L"input hid_t is not a DatasetAccessPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::DatasetAccessPropertyList");
#endif
			}
		}
	}
#else
	DatasetAccessPropertyList::DatasetAccessPropertyList(hid_t hid) : PropertyList(hid)
	{

	}
#endif

	DatasetAccessPropertyList::DatasetAccessPropertyList()
	{
		m_hID = H5Pcreate(H5P_DATASET_ACCESS);
	}

	DatasetAccessPropertyList::DatasetAccessPropertyList(const DatasetAccessPropertyList& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}


	DatasetAccessPropertyList::~DatasetAccessPropertyList()
	{
		// let PropertyList do its thing
	}

	bool DatasetAccessPropertyList::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_DATASET_ACCESS) > 0) {
					H5Pclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Pclose(m_hID);
			m_hID = InvalidHandle;
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	FileCreationPropertyList::FileCreationPropertyList(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_FILE_CREATE) > 0) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a FileCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::FileCreationPropertyList");
#endif
				}
			}
			else {
				assert(false && L"input hid_t is not a FileCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::FileCreationPropertyList");
#endif
			}
		}
	}
#else
	FileCreationPropertyList::FileCreationPropertyList(hid_t hid) : PropertyList(hid)
	{

	}
#endif

	FileCreationPropertyList::FileCreationPropertyList()
	{
		m_hID = H5Pcreate(H5P_FILE_CREATE);
	}

	FileCreationPropertyList::FileCreationPropertyList(const FileCreationPropertyList& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}


	FileCreationPropertyList::~FileCreationPropertyList()
	{
		// let PropertyList do its thing
	}

	bool FileCreationPropertyList::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_FILE_CREATE) > 0) {
					H5Pclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Pclose(m_hID);
			m_hID = InvalidHandle;
			return false;
		}
	}

	bool FileCreationPropertyList::SetUserblock(hsize_t size)
	{
		return H5Pset_userblock(m_hID, size) >= 0;
	}

	bool FileCreationPropertyList::GetUserblock(hsize_t& size)
	{
		return H5Pget_userblock(m_hID, &size) >= 0;
	}

	bool FileCreationPropertyList::SetSizes(size_t sizeof_addr, size_t sizeof_size)
	{
		return H5Pset_sizes(m_hID, sizeof_addr, sizeof_size) >= 0;
	}

	bool FileCreationPropertyList::GetSizes(size_t& sizeof_addr, size_t& sizeof_size)
	{
		return H5Pget_sizes(m_hID, &sizeof_addr, &sizeof_size) >= 0;
	}

	bool FileCreationPropertyList::SetSymK(unsigned int ik, unsigned int lk)
	{
		return H5Pset_sym_k(m_hID, ik, lk) >= 0;
	}

	bool FileCreationPropertyList::GetSymK(unsigned int& ik, unsigned int& lk)
	{
		return H5Pget_sym_k(m_hID, &ik, &lk) >= 0;
	}

	bool FileCreationPropertyList::SetIstoreK(unsigned int ik)
	{
		return H5Pset_istore_k(m_hID, ik) >= 0;
	}

	bool FileCreationPropertyList::GetIstoreK(unsigned int& ik)
	{
		return H5Pget_istore_k(m_hID, &ik) >= 0;
	}

	bool FileCreationPropertyList::SetFileSpacePageSize(hsize_t fsp_size)
	{
		return H5Pset_file_space_page_size(m_hID, fsp_size) >= 0;
	}

	bool FileCreationPropertyList::GetFileSpacePageSize(hsize_t& fsp_size)
	{
		return H5Pget_file_space_page_size(m_hID, &fsp_size) >= 0;
	}

	bool FileCreationPropertyList::SetFileSpaceStrategy(FileSpaceStrategy strategy, bool persist, hsize_t threshold)
	{
		return H5Pset_file_space_strategy(m_hID, (H5F_fspace_strategy_t)strategy, persist, threshold) >= 0;
	}

	bool FileCreationPropertyList::GetFileSpaceStrategy(FileSpaceStrategy& strategy, bool& persist, hsize_t& threshold)
	{
		return H5Pget_file_space_strategy(m_hID, (H5F_fspace_strategy_t*)(&strategy), &persist, &threshold) >= 0;
	}

	bool FileCreationPropertyList::SetSharedMesgIndexesCount(unsigned int nindexes)
	{
		return H5Pset_shared_mesg_nindexes(m_hID, nindexes) >= 0;
	}

	bool FileCreationPropertyList::GetSharedMesgIndexesCount(unsigned int& nindexes)
	{
		return H5Pget_shared_mesg_nindexes(m_hID, &nindexes) >= 0;
	}

	bool FileCreationPropertyList::SetSharedMesgIndex(unsigned int index_num, unsigned int mesg_type_flags, unsigned int min_mesg_size)
	{
		return H5Pset_shared_mesg_index(m_hID, index_num, mesg_type_flags, min_mesg_size) >= 0;
	}

	bool FileCreationPropertyList::GetSharedMesgIndex(unsigned int index_num, unsigned int& mesg_type_flags, unsigned int& min_mesg_size)
	{
		return H5Pget_shared_mesg_index(m_hID, index_num, &mesg_type_flags, &min_mesg_size) >= 0;
	}

	bool FileCreationPropertyList::SetSharedMesgPhaseChange(unsigned int max_list, unsigned int min_btree)
	{
		return H5Pset_shared_mesg_phase_change(m_hID, max_list, min_btree) >= 0;
	}

	bool FileCreationPropertyList::GetSharedMesgPhaseChange(unsigned int& max_list, unsigned int& min_btree)
	{
		return H5Pget_shared_mesg_phase_change(m_hID, &max_list, &min_btree) >= 0;
	}

	//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	FileAccessPropertyList::FileAccessPropertyList(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_FILE_ACCESS) > 0) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a FileAccessPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::FileAccessPropertyList");
#endif
				}
			}
			else {
				assert(false && L"input hid_t is not a FileAccessPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::FileAccessPropertyList");
#endif
			}
		}
	}
#else
	FileAccessPropertyList::FileAccessPropertyList(hid_t hid) : PropertyList(hid)
	{

	}
#endif

	FileAccessPropertyList::FileAccessPropertyList()
	{
		m_hID = H5Pcreate(H5P_FILE_ACCESS);
	}

	FileAccessPropertyList::FileAccessPropertyList(const FileAccessPropertyList& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}


	FileAccessPropertyList::~FileAccessPropertyList()
	{
		// let PropertyList do its thing
	}

	bool FileAccessPropertyList::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_FILE_ACCESS) > 0) {
					H5Pclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Pclose(m_hID);
			m_hID = InvalidHandle;
			return false;
		}
	}


	//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	DatatypeCreationPropertyList::DatatypeCreationPropertyList(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_DATATYPE_CREATE) > 0) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a DatatypeCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::DatatypeCreationPropertyList");
#endif
				}
			}
			else {
				assert(false && L"input hid_t is not a DatatypeCreationPropertyList");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::DatatypeCreationPropertyList");
#endif
			}
		}
	}
#else
	DatatypeCreationPropertyList::DatatypeCreationPropertyList(hid_t hid) : PropertyList(hid)
	{

	}
#endif

	DatatypeCreationPropertyList::DatatypeCreationPropertyList()
	{
		m_hID = H5Pcreate(H5P_DATATYPE_CREATE);
	}

	DatatypeCreationPropertyList::DatatypeCreationPropertyList(const DatatypeCreationPropertyList& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}


	DatatypeCreationPropertyList::~DatatypeCreationPropertyList()
	{
		// let PropertyList do its thing
	}

	bool DatatypeCreationPropertyList::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GENPROP_LST == H5Iget_type(hid)) {
				auto pc = H5Pget_class(hid);
				if (H5Pequal(pc, H5P_DATATYPE_CREATE) > 0) {
					H5Pclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Pclose(m_hID);
			m_hID = InvalidHandle;
			return false;
		}
	}
}
