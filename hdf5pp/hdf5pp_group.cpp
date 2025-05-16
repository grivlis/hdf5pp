#include "pch.h"
#include "hdf5pp_group.h"

namespace HDF5 {

	Group::Group(const Group& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

	Group::~Group()
	{
		if (m_hID >= 0) {
			H5Gclose(m_hID);
			m_hID = InvalidHandle;
		}
	}

	Group& Group::operator=(const Group& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool Group::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GROUP == H5Iget_type(hid)) {
				if (m_hID >= 0) {
					H5Gclose(m_hID);
				}
				m_hID = hid;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (m_hID >= 0) {
				H5Gclose(m_hID);
			}
			m_hID = InvalidHandle;
			return true;
		}
	}

	bool Group::Iterate(IndexType tInded, OrderType tOrder, hsize_t& position, H5L_iterate2_t op, void* op_data)
	{
		return H5Literate2(m_hID, (H5_index_t)tInded, (H5_iter_order_t)tOrder, &position, op, op_data) >= 0;
	}

	bool Group::Visit(IndexType tIndex, OrderType tOrder, H5L_iterate_t op, void* op_data)
	{
		return H5Lvisit(m_hID, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, op, op_data) >= 0;
	}

	bool Group::Flush()
	{
		return H5Gflush(m_hID) >= 0;
	}

	HDF5::GroupCreationPropertyList Group::GetCreationPropertyList()
	{
		return GroupCreationPropertyList(H5Gget_create_plist(m_hID));
	}

	bool Group::Refresh()
	{
		return H5Grefresh(m_hID) >= 0;
	}

#ifdef _DEBUG
	Group::Group(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_GROUP == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not an Group");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::Group");
#endif
			}
		}
		else {
			m_hID = InvalidHandle;
		}
	}
#else
	Group::Group(hid_t hid) : Object(hid)
	{

	}
#endif

	bool CopyObject(const Group& srcGroup, const char* srcName, const Group& dstGroup, const char* dstName, const PropertyList& ocpl /*= PropertyList()*/, const PropertyList& lcpl /*= PropertyList()*/)
	{
		return H5Ocopy((hid_t)srcGroup, srcName, (hid_t)dstGroup, dstName, (hid_t)ocpl, (hid_t)lcpl);
	}

}
