#include "pch.h"
#include "hdf5pp_object.h"


namespace HDF5 {

	Object::Object(const Object& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

	// let AttributedObject to the thing
	Object::~Object()
	{

	}

	Object& Object::operator=(const Object& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool Object::Attach(hid_t hid)
	{
		if (hid >= 0) {
			switch (H5Iget_type(hid)) {
			case H5I_GROUP:
			case H5I_DATASET:
			case H5I_DATATYPE:
				DecrementReferenceCount();
				m_hID = hid;
				return true;
			default:
				return false;
			}
		}
		else {
			DecrementReferenceCount();
			m_hID = InvalidHandle;
			return true;
		}
	}

	bool Object::AreMDCFlushesDisabled(bool* failed /*= nullptr*/)
	{
		bool retval{ false };
		auto rv = H5Oare_mdc_flushes_disabled(m_hID, &retval);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return retval;
	}

	bool Object::DisableMDCFlushes()
	{
		return H5Odisable_mdc_flushes(m_hID) >= 0;
	}

	bool Object::EnableMDCFlushes()
	{
		return H5Oenable_mdc_flushes(m_hID) >= 0;
	}

	bool Object::DecrementObjectReferenceCount()
	{
		return H5Odecr_refcount(m_hID) >= 0;
	}

	bool Object::IncrementObjectReferenceCount()
	{
		return H5Oincr_refcount(m_hID) >= 0;
	}

	bool Object::FlushObject()
	{
		return H5Oflush(m_hID) >= 0;
	}

	bool Object::LinkObject(const Location& newLocation, const char* new_name, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Olink(m_hID, (hid_t)newLocation, new_name, (hid_t)lcpl, (hid_t)lapl);
	}


	bool Object::RefreshObject()
	{
		return H5Orefresh(m_hID) >= 0;
	}

#ifdef _DEBUG
	Object::Object(hid_t hid)
	{
		if (hid >= 0) {
			switch (H5Iget_type(hid)) {
			case H5I_GROUP:
			case H5I_DATASET:
			case H5I_DATATYPE:
				m_hID = hid;
				break;
			default:
				assert(false && L"input hid_t is not an Object");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::Object");
#endif
				break;
			}

		}
		else {
			m_hID = InvalidHandle;
		}
	}
#else
	Object::Object(hid_t hid) : AttributedObject(hid)
	{
	}
#endif
}

