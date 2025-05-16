#include "pch.h"
#include "hdf5pp_attribute.h"
#include "hdf5pp_dspace.h"
#include "hdf5pp_dtype.h"

namespace HDF5 {

	Attribute::Attribute(const Attribute& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

#ifdef _DEBUG
	Attribute::Attribute(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_ATTR == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not an Attribute");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::Attribute");
#endif
			}
		}
	}
#else
	Attribute::Attribute(hid_t hid) : Location(hid)
	{

	}
#endif

	Attribute::~Attribute()
	{
		if (m_hID >= 0) {
			H5Aclose(m_hID);
		}
		m_hID = InvalidHandle;
	}

	Attribute& Attribute::operator=(const Attribute& rhs)
	{
		if (this != &rhs) {
			if (m_hID >= 0) {
				H5Aclose(m_hID);
			}
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool Attribute::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_ATTR == H5Iget_type(hid)) {
				if (m_hID >= 0) {
					H5Aclose(m_hID);
				}
				m_hID = hid;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			H5Aclose(m_hID);
			m_hID = InvalidHandle;
			return true;
		}
	}

	HDF5::AttributeCreationPropertyList Attribute::GetCreationPropertyList()
	{
		return AttributeCreationPropertyList(H5Aget_create_plist(m_hID));
	}

	bool Attribute::GetInfo(H5A_info_t& info)
	{
		return H5Aget_info(m_hID, &info) >= 0;
	}

	bool Attribute::GetName(std::string& name)
	{
		auto s = H5Aget_name(m_hID, 0, nullptr); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Aget_name(m_hID, s + 1, nm);
			if (s2 == s) {
				name = nm;
			}
			delete[] nm;
			return s2 > 0;
		}
		else if (s == 0) {
			name.clear();
			return true;
		}
		return false;
	}

	HDF5::Dataspace Attribute::GetDataspace()
	{
		return Dataspace(H5Aget_space(m_hID));
	}

	hsize_t Attribute::GetStorageSize()
	{
		return H5Aget_storage_size(m_hID);
	}

	HDF5::Datatype Attribute::GetDatatype()
	{
		return Datatype(H5Aget_type(m_hID));
	}

	bool Attribute::Read(const Datatype& mem_dtype, void* buf)
	{
		return H5Aread(m_hID, (hid_t)mem_dtype, buf) >= 0;
	}

	bool Attribute::Write(const Datatype& mem_dtype, const void* buf)
	{
		return H5Awrite(m_hID, (hid_t)mem_dtype, buf) >= 0;
	}

}
