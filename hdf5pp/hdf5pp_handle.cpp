#include "pch.h"
#include "hdf5pp_handle.h"

namespace HDF5 {

	Handle::Handle(hid_t hid) : m_hID(hid < 0 ? InvalidHandle : hid)
	{

	}

	Handle::Handle(const Handle& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

	Handle::~Handle()
	{
		DecrementReferenceCount();
	}

	Handle& Handle::operator=(const Handle& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	// is object identifier valid? (H5I_IS_VALID)
	bool Handle::IsValid() const
	{
		return m_hID >= 0 && H5Iis_valid(m_hID);
	}

	// take ownership of identifier and will be responsible for closing it
	// return true if ownership acquired successfully; otherwise false
	bool Handle::Attach(hid_t hid)
	{
		DecrementReferenceCount(); // for current hid
		m_hID = hid >= 0 ? hid : InvalidHandle;
		return true;
	}

	// release ownership of indentifier; user should close it using the
	// appropriate C-API function
	hid_t Handle::Detach()
	{
		auto ret = m_hID;
		m_hID = InvalidHandle;
		return ret;
	}

	// return the object type of the Handle (H5I_GET_TYPE)
	Handle::ObjectType Handle::GetObjectType() const
	{
		return (ObjectType)H5Iget_type(m_hID);
	}

	// increment identifier reference count (H5I_INC_REF)
	// note that this may cause resource leaks by preventing identifiers from deleting
	int Handle::IncrementReferenceCount()
	{
		return m_hID < 0 ? 0 : H5Iinc_ref(m_hID);
	}

	// decrement identifier reference count (H5I_DEC_REF)
	// when reference count gets to zero, the identifier resource will be deleted
	int Handle::DecrementReferenceCount()
	{
		auto ret = m_hID < 0 ? 0 : H5Idec_ref(m_hID);
		if (ret == 0) {
			m_hID = InvalidHandle;
		}
		return ret;
	}

	int Handle::GetReferenceCount()
	{
		return m_hID < 0 ? 0 : H5Iget_ref(m_hID);
	}

	herr_t Handle::Iterate(ObjectType otype, H5I_iterate_func_t op, void* op_data /* = nullptr */)
	{
		return H5Iiterate((H5I_type_t)otype, op, op_data);
	}

}