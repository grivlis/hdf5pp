#pragma once


#include "hdf5pp_attrobj.h"
#include "hdf5pp_proplist.h"

namespace HDF5 {

	class HDF5PP_API Datatype;
	class HDF5PP_API Dataspace;

	class HDF5PP_API Dataset : public AttributedObject
	{
	public:
		Dataset(const Dataset& rhs);
		virtual ~Dataset();
		Dataset& operator=(const Dataset& rhs);

		bool Attach(hid_t hid) override;

		// Fills Dataspace elements with fill value in memory buffer
		static bool Fill(const void* fill_value, const Datatype& fill_value_dype, void* buf, const Datatype& buf_dype, const Dataspace& buf_dspace);

		// Flushes all buffers associated with this Dataset
		bool Flush();

		// Gathers data from a selection within a memory buffer
		// also see Scatter
		static bool Gather(const Dataspace& src_dspace, const void* src_buf, const Datatype& dype, size_t dst_buf_size, void* dst_buf, H5D_gather_func_t op, void* op_data);

		// Returns a copy of dataset access property list
		DatasetAccessPropertyList GetAccessPropertyList();

		// Returns a copy of dataset creation property list
		DatasetCreationPropertyList GetCreationPropertyList();

		// Retrieves the amount of storage allocated within the file for a raw data chunk
		bool GetChunkStorageSize(const std::vector<hsize_t>& offset, hsize_t& chunk_nbytes);

		// Returns the dataset address in the file
		haddr_t GetOffset();

		// Returns a copy of the Dataspace for this Dataset
		Dataspace GetDataspace();

		enum class SpaceStatus {
			NotAllocated = H5D_SPACE_STATUS_NOT_ALLOCATED,
			Allocated = H5D_SPACE_STATUS_ALLOCATED,
			PartlyAllocated = H5D_SPACE_STATUS_PART_ALLOCATED
		};
		// Determines whether space has been allocated for this Dataset
		bool GetSpaceStatus(SpaceStatus& status);

		// Returns the amount of storage allocated for the Dataset
		// Note that this member is not generally appropriate to use when determining 
		// the amount of memory required to work with a dataset. In such circumstances, 
		// you must determine the number of data points in a dataset and the size of an 
		// individual data element
		hsize_t GetStorageSize();

		// Returns a copy of the Datatype for this Dataset
		Datatype GetDatatype();

		// Iterates over all selected elements in a Dataspace
		// H5D_operator_t prototype: 'herr_t func(void elem, hid_t type_id, unsigned ndim, const hsize_t *point, void *operator_data)'
		static bool Iterate(void* buf, const Datatype& buf_dtype, const Dataspace& buf_dspace, H5D_operator_t op, void* op_data);

		// Reads raw data from the dataset into a buffer
		bool Read(const Datatype& mem_dype, const Dataspace& mem_dspace, const Dataspace& file_dspace, void* buf, const PropertyList& xpl = PropertyList());

		// Reads a raw data chunk from the dataset into a buffer
		bool ReadChunk(const std::vector<hsize_t>& offset, uint32_t& filters, void* buf, const PropertyList& xpl = PropertyList());

		// Refreshes all the buffers associated with this Dataset
		bool Refresh();

		// Scatters data into a selection within a memory buffer
		// see also: Gather
		static bool Scatter(H5D_scatter_func_t op, void* op_data, const Datatype& dtype, const Dataspace& dspace, void* dst_buf);

		// Changes the sizes of the Dataset dimensions
		bool SetExtent(const std::vector<hsize_t>& size);

		// Determines the number of bytes required to store variable-length (VL) data
		bool GetVariableLengthDataSize(const Datatype& dtype, const Dataspace& dspace, hsize_t& size);

		// Reclaims variable-length (VL) Datatype memory buffers
		static bool ReclaimVariableLength(const Datatype& dtype, const Dataspace& dspace, const PropertyList& pl, void* buf);

		// Writes raw data from a buffer to the Dataset
		bool Write(const Datatype& mem_dtype, const Dataspace& mem_dspace, const Dataspace& file_dspace, const void* buf, const PropertyList& xpl = PropertyList());

		// Writes a raw data chunk from a buffer directly to the Dataset in a file
		bool WriteChunk(uint32_t filters, std::vector<hsize_t>& offset, size_t data_size, const void* buf, const PropertyList& xpl = PropertyList());
	protected:
		explicit Dataset(hid_t hid);
		friend class Location;
	};
}
