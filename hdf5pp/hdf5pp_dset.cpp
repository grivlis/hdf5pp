#include "pch.h"
#include "hdf5pp_dset.h"

#include "hdf5pp_dtype.h"
#include "hdf5pp_dspace.h"

namespace HDF5 {


	Dataset::Dataset(const Dataset& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

#ifdef _DEBUG
	Dataset::Dataset(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATASET == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not a Dataset");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::Dataset");
#endif
			}
		}
	}
#else
	Dataset::Dataset(hid_t hid) : AttributedObject(hid)
	{

	}
#endif

	Dataset::~Dataset()
	{
		H5Dclose(m_hID);
		m_hID = InvalidHandle;
	}

	bool Dataset::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATASET == H5Iget_type(hid)) {
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

	bool Dataset::Fill(const void* fill_value, const Datatype& fill_value_dtype, void* buf, const Datatype& buf_dtype, const Dataspace& buf_dspace)
	{
		return H5Dfill(fill_value, (hid_t)fill_value_dtype, buf, (hid_t)buf_dtype, (hid_t)buf_dspace) >= 0;
	}

	bool Dataset::Flush()
	{
		return H5Dflush(m_hID) >= 0;
	}

	bool Dataset::Gather(const Dataspace& src_dspace, const void* src_buf, const Datatype& dype, size_t dst_buf_size, void* dst_buf, H5D_gather_func_t op, void* op_data)
	{
		return H5Dgather((hid_t)src_dspace, src_buf, (hid_t)dype, dst_buf_size, dst_buf, op, op_data) >= 0;
	}

	DatasetAccessPropertyList Dataset::GetAccessPropertyList()
	{
		return DatasetAccessPropertyList(H5Dget_access_plist(m_hID));
	}

	DatasetCreationPropertyList Dataset::GetCreationPropertyList()
	{
		return DatasetCreationPropertyList(H5Dget_create_plist(m_hID));
	}

	bool Dataset::GetChunkStorageSize(const std::vector<hsize_t>& offset, hsize_t& chunk_nbytes)
	{
		return H5Dget_chunk_storage_size(m_hID, offset.data(), &chunk_nbytes) >= 0;
	}

	haddr_t Dataset::GetOffset()
	{
		return H5Dget_offset(m_hID);
	}

	HDF5::Dataspace Dataset::GetDataspace()
	{
		return Dataspace(H5Dget_space(m_hID));
	}

	bool Dataset::GetSpaceStatus(SpaceStatus& status)
	{
		return H5Dget_space_status(m_hID, (H5D_space_status_t*)(&status)) >= 0;
	}

	hsize_t Dataset::GetStorageSize()
	{
		return H5Dget_storage_size(m_hID);
	}

	HDF5::Datatype Dataset::GetDatatype()
	{
		return Datatype(H5Dget_type(m_hID));
	}

	bool Dataset::Iterate(void* buf, const Datatype& buf_dtype, const Dataspace& buf_dspace, H5D_operator_t op, void* op_data)
	{
		return H5Diterate(buf, (hid_t)buf_dtype, (hid_t)buf_dspace, op, op_data) >= 0;
	}

	bool Dataset::Read(const Datatype& mem_dype, const Dataspace& mem_dspace, const Dataspace& file_dspace, void* buf, const PropertyList& xpl /*= PropertyList()*/)
	{
		return H5Dread(m_hID, (hid_t)mem_dype, (hid_t)mem_dspace, (hid_t)file_dspace, (hid_t)xpl, buf) >= 0;
	}

	bool Dataset::ReadChunk(const std::vector<hsize_t>& offset, uint32_t& filters, void* buf, const PropertyList& xpl /*= PropertyList()*/)
	{
		return H5Dread_chunk(m_hID, (hid_t)xpl, offset.data(), &filters, buf) >= 0;
	}

	bool Dataset::Refresh()
	{
		return H5Drefresh(m_hID) >= 0;
	}

	bool Dataset::Scatter(H5D_scatter_func_t op, void* op_data, const Datatype& dtype, const Dataspace& dspace, void* dst_buf)
	{
		return H5Dscatter(op, op_data, (hid_t)dtype, (hid_t)dspace, dst_buf) >= 0;
	}

	bool Dataset::SetExtent(const std::vector<hsize_t>& size)
	{
		return H5Dset_extent(m_hID, size.data()) >= 0;
	}

	bool Dataset::GetVariableLengthDataSize(const Datatype& dtype, const Dataspace& dspace, hsize_t& size)
	{
		return H5Dvlen_get_buf_size(m_hID, (hid_t)dtype, (hid_t)dspace, &size) >= 0;
	}

	bool Dataset::ReclaimVariableLength(const Datatype& dtype, const Dataspace& dspace, const PropertyList& pl, void* buf)
	{
		return H5Dvlen_reclaim((hid_t)dtype, (hid_t)dspace, (hid_t)pl, buf) >= 0;
	}

	bool Dataset::Write(const Datatype& mem_dtype, const Dataspace& mem_dspace, const Dataspace& file_dspace, const void* buf, const PropertyList& xpl /*= PropertyList()*/)
	{
		return H5Dwrite(m_hID, (hid_t)mem_dtype, (hid_t)mem_dspace, (hid_t)file_dspace, (hid_t)xpl, buf) >= 0;
	}

	bool Dataset::WriteChunk(uint32_t filters, std::vector<hsize_t>& offset, size_t data_size, const void* buf, const PropertyList& xpl /*= PropertyList()*/)
	{
		return H5Dwrite_chunk(m_hID, (hid_t)xpl, filters, offset.data(), data_size, buf) >= 0;
	}

	Dataset& Dataset::operator=(const Dataset& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

}
