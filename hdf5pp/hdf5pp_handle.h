// hdf5pp_api.h
// all HDF5 objects that are identified by a hid_t identifier inherit from this class
// 
#pragma once

#include "hdf5pp_api.h"

namespace HDF5 {

	class HDF5PP_API Handle
	{
	public:
		static const hid_t InvalidHandle = H5I_INVALID_HID;

		Handle() = default;
		Handle(const Handle& rhs);
		virtual ~Handle();
		Handle& operator=(const Handle& rhs);

		// is object identifier valid? (H5I_IS_VALID)
		bool IsValid() const;

		// take ownership of identifier and will be responsible for closing it
		// return true if ownership acquired successfully; otherwise false
		virtual bool Attach(hid_t hid);

		// release ownership of indentifier; user should close it using the
		// appropriate C-API function
		hid_t Detach();

		// casting does not increment reference count; 
		// user should not close the handle with any C-API functions
		explicit operator hid_t() const { return m_hID; }

		enum class ObjectType {
			Uninitialized = H5I_UNINIT,
			Unknown = H5I_BADID,
			File = H5I_FILE,
			Group = H5I_GROUP,
			Datatype = H5I_DATATYPE,
			Dataspace = H5I_DATASPACE,
			Dataset = H5I_DATASET,
			Attribute = H5I_ATTR,
			// Reference = H5I_REFERENCE,	// removed in 1.12.0
			VirtualFileLayer = H5I_VFL,
			PropertyListClass = H5I_GENPROP_CLS,
			PropertyList = H5I_GENPROP_LST,
			ErrorClass = H5I_ERROR_CLASS,
			ErrorMessage = H5I_ERROR_MSG,
			ErrorStack = H5I_ERROR_STACK
		};

		// return the object type of the Handle (H5I_GET_TYPE)
		ObjectType GetObjectType() const;

		// increment identifier reference count (H5I_INC_REF)
		// note that this may cause resource leaks by preventing identifiers from deleting
		int IncrementReferenceCount();

		// decrement identifier reference count (H5I_DEC_REF)
		// when reference count gets to zero, the identifier resource will be deleted
		int DecrementReferenceCount();

		int GetReferenceCount();

		// Calls the callback function op for each member of the identifier type type. The callback function type for op, H5I_iterate_func_t, is defined as:
		// typedef herr_t(*H5I_iterate_func_t)(hid_t id, void* udata);
		// op takes as parameters the identifier and a pass through of op_data, and returns an herr_t.
		static herr_t Iterate(ObjectType otype, H5I_iterate_func_t op, void* op_data = nullptr);
	protected:
		explicit Handle(hid_t hid);
		hid_t m_hID{ InvalidHandle };
		friend class File;
		friend class Location;
	};


}