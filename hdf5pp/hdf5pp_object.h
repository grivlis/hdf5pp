// hdf5pp_object.h
// HDF5 objects are: group, dataset, and committed datatype
// those are objects have independent existence inside an HDF5 file
//
#pragma once


#include "hdf5pp_attrobj.h"
#include "hdf5pp_proplist.h"
#include "hdf5pp_object.h"

namespace HDF5 {

	class HDF5PP_API Object : public AttributedObject
	{
	public:
		Object() = default;
		Object(const Object& rhs);
		virtual ~Object();
		Object& operator=(const Object& rhs);

		bool Attach(hid_t hid) override;

		// Determines if the object has had flushes of metatdata entries disabled
		virtual bool AreMDCFlushesDisabled(bool* failed = nullptr);

		// Disables metadata automatic flushes;
		virtual bool DisableMDCFlushes();
		// Enables metadata automatic flushes;
		virtual bool EnableMDCFlushes();

		// Decrement the object's reference count
		// Note that this is for the actual object, in file, as in after a hard link!
		// use with EXTREME caution! This may render parts of the file inaccessible or corrupted!
		virtual bool DecrementObjectReferenceCount();

		// Increment the object's reference count
		// Note that this is for the actual object, in file, as in after a hard link!
		// use with EXTREME caution! This may render parts of the file inaccessible or corrupted!
		virtual bool IncrementObjectReferenceCount();

		// Flushes all buffers associated with this object to disk
		bool FlushObject();

		// Create a hard link to this object
		bool LinkObject(const Location& newLocation, const char* new_name, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());

		// Refreshes all buffers associated with this object
		bool RefreshObject();
	protected:
		explicit Object(hid_t hid);
		friend class Location;
	};
}
