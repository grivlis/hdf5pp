#pragma once

#include "hdf5pp_location.h"
#include "hdf5pp_proplist.h"

namespace HDF5 {

	class HDF5PP_API Dataspace;
	class HDF5PP_API Datatype;

	class HDF5PP_API Attribute : public Location
	{
	public:
		Attribute() : Location(InvalidHandle) { }
		Attribute(const Attribute& rhs);
		virtual ~Attribute();
		Attribute& operator=(const Attribute& rhs);

		bool Attach(hid_t hid) override;

		// Returns the attribute creation property list
		AttributeCreationPropertyList GetCreationPropertyList();

		// Retrieves attribute info
		bool GetInfo(H5A_info_t& info);

		// Retrieves attribute name
		bool GetName(std::string& name);

		// Returns the attribute  Dataspace
		Dataspace GetDataspace();

		// Get storage size
		hsize_t GetStorageSize();

		// Returns the attribute  Datatype
		Datatype GetDatatype();

		// Read the attribute data
		bool Read(const Datatype& mem_dtype, void* buf);

		// Write the attribute data
		bool Write(const Datatype& mem_dtype, const void* buf);
	protected:
		explicit Attribute(hid_t hid);
		friend class AttributedObject;
	};
}
