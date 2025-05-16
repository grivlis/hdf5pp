#include "pch.h"
#include "hdf5pp_custom.h"

namespace HDF5 {


	// Creates and returns a new CustomHandle type.
	CustomHandle::CustomHandleType CustomHandle::RegisterType(size_t hash_size, unsigned int reserved, FreeFunctionType free_func)
	{
		return H5Iregister_type(hash_size, reserved, free_func);
	}

	// Removes the CustomHandleType and all identifiers within that type.
	// Note that it's possible to render instances of CustomHandle invalid
	bool CustomHandle::UnregisterType(CustomHandleType type)
	{
		return H5Idestroy_type(type) >= 0;
	}

	// Deletes all identifiers of the given type
	// if force==false, only identifiers whose reference count is 1 will be deleted
	// Note that it's possible to render instances of CustomHandle invalid
	bool CustomHandle::ClearTypeMembers(CustomHandleType type, bool force /*= false*/)
	{
		return H5Iclear_type(type, force) >= 0;
	}

	// Decrements the reference count on an CustomHandleType
	int CustomHandle::DecrementTypeReferenceCount(CustomHandleType type)
	{
		return H5Idec_type_ref(type);
	}

	// Increments the reference count on a CustomHandleType
	int CustomHandle::IncrementTypeReferenceCount(CustomHandleType type)
	{
		return H5Iinc_type_ref(type);
	}

	// Retrieves the reference count on a CustomHandleType
	int CustomHandle::GetTypeReferenceCount(CustomHandleType type)
	{
		return H5Iget_type_ref(type);
	}

	// Returns the number of identifiers in a given CustomHandleType
	bool CustomHandle::GetTypeMembersCount(CustomHandleType type, uint64_t& num_members)
	{
		hsize_t hs{ 0 };
		auto retval = H5Inmembers(type, &hs);
		if (retval >= 0) {
			num_members = hs;
		}
		return retval >= 0;
	}

	// Finds the memory referred to by an identifier within the given CustomHandleType such that some criterion is satisfied
	void* CustomHandle::SearchTypeMembers(CustomHandleType type, SearchFunctionType func, void* key)
	{
		return H5Isearch(type, func, key);
	}

	// Determines whether a CustomHandleType is registered
	// if returned false, it's possible also that the function just failed
	// failed will return true in this, but only if not null
	bool CustomHandle::TypeExists(CustomHandleType type, bool* failed /* = nullptr */)
	{
		auto retval = H5Itype_exists(type);
		if (failed != nullptr) {
			*failed = retval < 0;
		}

		return retval > 0;
	}

	// Creates and returns a new CustomHandle
	CustomHandle::CustomHandle(CustomHandleType type, void* object)
	{
		auto oid = H5Iregister(type, object);
		if (oid >= 0) {
			m_hID = oid;
			m_type = type;
		}
	}

	CustomHandle::CustomHandle(const CustomHandle& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

	CustomHandle& CustomHandle::operator=(const CustomHandle& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool CustomHandle::Attach(hid_t hid)
	{
		if (hid >= 0) {
			auto tt = H5Iget_type(hid);
			if (tt == m_type) {
				DecrementReferenceCount();
				m_hID = hid;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			DecrementReferenceCount();
			m_hID = InvalidHandle;
			return true;
		}
	}

	bool CustomHandle::Attach(hid_t hid, CustomHandleType type)
	{
		if (TypeExists(type)) {
			if (m_hID >= 0) {
				auto tt = H5Iget_type(hid);
				if (tt == type) {
					DecrementReferenceCount();
					m_hID = hid;
					m_type = type;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				DecrementReferenceCount();
				m_hID = InvalidHandle;
				m_type = type;
				return true;
			}
		}
		else {
			return false;
		}
	}

	// removes ID _AND_ delete the memory by applying delete
	CustomHandle::~CustomHandle()
	{
		auto o = GetObjectMemory(true);
		delete o;
	}

	// Removes an ID from internal storage (H5I_REMOVE_VERIFY)
	// Returns the object (memory) referenced by this object (H5I_OBJECT_VERIFY)
	void* CustomHandle::GetObjectMemory(bool remove /* = false */)
	{
		if (remove) {
			auto retval = H5Iremove_verify(m_hID, m_type);
			m_hID = InvalidHandle;
			return retval;
		}
		else {
			return H5Iobject_verify(m_hID, m_type);
		}
	}


}
