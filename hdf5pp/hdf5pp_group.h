#pragma once

#include "hdf5pp_object.h"
#include "hdf5pp_proplist.h"

namespace HDF5 {

	class HDF5PP_API Group : public Object
	{
	public:
		Group() = default;
		Group(const Group& rhs);
		virtual ~Group();
		Group& operator=(const Group& rhs);

		bool Attach(hid_t hid) override;

		// Iterates through links in a group
		bool Iterate(IndexType tIndex, OrderType tOrder, hsize_t& position, H5L_iterate2_t op, void* op_data);

		// Recursively visits all links starting from this group
		bool Visit(IndexType tIndex, OrderType tOrder, H5L_iterate_t op, void* op_data);

		// Flushes all buffers associated with the group to disk.
		bool Flush();

		// Returns the group's creation property list
		GroupCreationPropertyList GetCreationPropertyList();


		// Refreshes all buffers associated with this group
		bool Refresh();
	protected:
		explicit Group(hid_t hid);
		friend class Location;
	};

	bool CopyObject(const Group& srcGroup, const char* srcName, const Group& dstGroup, const char* dstName, const PropertyList& ocpl = PropertyList(), const PropertyList& lcpl = PropertyList());

}
