#include "pch.h"
#include "hdf5pp_dspace.h"


namespace HDF5 {


	Dataspace::Dataspace(const Dataspace& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}


#ifdef _DEBUG
	Dataspace::Dataspace(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATASPACE == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not a Dataspace");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::Dataspace");
#endif

			}
		}
	}
#else
	Dataspace::Dataspace(hid_t hid) : Handle(hid)
	{

	}
#endif

	Dataspace::Dataspace(Type type /*= Type::Scalar*/)
	{
		m_hID = H5Screate((H5S_class_t)type);
	}

	Dataspace::Dataspace(const std::vector<hsize_t>& currentDims, const std::vector<hsize_t>& maxDims /*= std::vector<hsize_t>()*/)
	{
		if (maxDims.size() == 0) {
			m_hID = H5Screate_simple((int)currentDims.size(), currentDims.data(), nullptr);
		}
		else {
			assert(currentDims.size() == maxDims.size());
			m_hID = H5Screate_simple((int)currentDims.size(), currentDims.data(), maxDims.data());
		}
		if (m_hID < 0) {
			m_hID = InvalidHandle;
		}
	}

	Dataspace::Dataspace(unsigned char* buf)
	{
		m_hID = H5Sdecode(buf);
	}

	Dataspace::~Dataspace()
	{
		H5Sclose(m_hID);
		m_hID = InvalidHandle;
	}

	bool Dataspace::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATASPACE == H5Iget_type(hid)) {
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

	Dataspace Dataspace::Copy()
	{
		return Dataspace(H5Scopy(m_hID));
	}

	bool Dataspace::Encode(std::vector<unsigned char>& buf, const PropertyList& fapl /*= PropertyList()*/)
	{
		size_t ns = 100;
		buf.resize(ns);
		auto rv = H5Sencode(m_hID, buf.data(), &ns, (hid_t)fapl);
		if (ns > buf.size()) {
			buf.resize(ns);
			rv = H5Sencode(m_hID, buf.data(), &ns, (hid_t)fapl);
		}
		else {
			buf.erase(buf.cbegin() + ns, buf.cend());
		}
		return rv >= 0;
	}

	bool Dataspace::CopyExtent(const Dataspace& src_dspace)
	{
		return H5Sextent_copy(m_hID, (hid_t)src_dspace) >= 0;
	}

	bool Dataspace::ExtentEqual(const Dataspace& other_dspace)
	{
		return H5Sextent_equal(m_hID, (hid_t)other_dspace) > 0;
	}

	bool Dataspace::GetRegularHyperslab(std::vector<hsize_t>& start, std::vector<hsize_t>& stride, std::vector<hsize_t>& count, std::vector<hsize_t>& block)
	{
		auto sz = H5Sget_simple_extent_ndims(m_hID);
		if (sz >= 0) {
			start.resize(sz);
			count.resize(sz);
			stride.resize(sz);
			block.resize(sz);
			return H5Sget_regular_hyperslab(m_hID, start.data(), stride.data(), count.data(), block.data()) >= 0;
		}
		else {
			return false;
		}
	}

	bool Dataspace::GetSelectBounds(std::vector<hsize_t>& start, std::vector<hsize_t>& end)
	{
		auto sz = H5Sget_simple_extent_ndims(m_hID);
		if (sz >= 0) {
			start.resize(sz);
			end.resize(sz);
			return H5Sget_select_bounds(m_hID, start.data(), end.data()) >= 0;
		}
		else {
			return false;
		}
	}

	hssize_t Dataspace::GetSelectedElementsPointsCount()
	{
		return H5Sget_select_elem_npoints(m_hID);
	}

	bool Dataspace::GetSelectedElementsPoints(hsize_t startpoint, hsize_t numpoints, std::vector<hsize_t>& buf)
	{
		auto sz = H5Sget_simple_extent_ndims(m_hID);
		if (sz >= 0) {
			// each point has sz coordinates
			buf.resize((const unsigned int)numpoints * (unsigned int)sz);
			return H5Sget_select_elem_pointlist(m_hID, startpoint, numpoints, buf.data()) >= 0;
		}
		else {
			return false;
		}
	}

	bool Dataspace::GetSelectedHyperslabBlocks(hsize_t startblock, hsize_t numblocs, std::vector<hsize_t>& buf)
	{
		auto sz = H5Sget_simple_extent_ndims(m_hID);
		if (sz >= 0) {
			// each block has two point, start and oppsite
			// each point has sz coordinates
			buf.resize((const unsigned int)numblocs * sz * 2);
			return H5Sget_select_hyper_blocklist(m_hID, startblock, numblocs, buf.data()) >= 0;
		}
		else {
			return false;
		}
	}

	hssize_t Dataspace::GetHyperslabBlocksCount()
	{
		return H5Sget_select_hyper_nblocks(m_hID);
	}

	hssize_t Dataspace::GetSelectedElementsCount()
	{
		return H5Sget_select_npoints(m_hID) >= 0;
	}

	bool Dataspace::GetSelectionType(SelectionType& t)
	{
		auto tt = H5Sget_select_type(m_hID);
		if (tt >= 0) {
			t = (SelectionType)tt;
		}
		return tt >= 0;
	}

	int Dataspace::GetSimpleExtentDims(hsize_t* dims, hsize_t* maxdims /*= nullptr*/)
	{
		return H5Sget_simple_extent_dims(m_hID, dims, maxdims);
	}

	int Dataspace::GetSimpleExtentDimsCount()
	{
		return H5Sget_simple_extent_ndims(m_hID);
	}

	hssize_t Dataspace::GetSimpleExtentElementsCount()
	{
		return H5Sget_simple_extent_npoints(m_hID);
	}

	bool Dataspace::GetSimpleExtentType(Type& t)
	{
		auto rv = H5Sget_simple_extent_type(m_hID);
		if (rv >= 0) {
			t = (Type)rv;
		}
		return rv >= 0;
	}


	bool Dataspace::IsRegularHyperslab(bool* failed /* = nullptr*/)
	{
		auto rv = H5Sis_regular_hyperslab(m_hID);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return  rv > 0;
	}

	bool Dataspace::IsSimple(bool* failed /*= nullptr*/)
	{
		auto rv = H5Sis_simple(m_hID);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return  rv > 0;
	}

	bool Dataspace::SetOffsetSimple(const hssize_t* offset)
	{
		return H5Soffset_simple(m_hID, offset) >= 0;
	}

	bool Dataspace::SelectAll()
	{
		return H5Sselect_all(m_hID) >= 0;
	}

	bool Dataspace::SelectElements(SelectionOperation op, const std::vector<hsize_t>& coords)
	{
		return SelectElements(op, coords.size(), coords.data());
	}

	bool Dataspace::SelectElements(SelectionOperation op, size_t num_elements, const hsize_t* coords)
	{
		return H5Sselect_elements(m_hID, (H5S_seloper_t)op, num_elements, coords) >= 0;
	}

	bool Dataspace::SelectHyperslab(SelectionOperation op, const hsize_t* start, const hsize_t* stride, const hsize_t* count, const hsize_t* block)
	{
		return H5Sselect_hyperslab(m_hID, (H5S_seloper_t)op, start, stride, count, block) >= 0;
	}

	bool Dataspace::SelectNone()
	{
		return H5Sselect_none(m_hID) >= 0;
	}

	bool Dataspace::IsSelectionValid(bool* failed /*= nullptr*/)
	{
		auto rv = H5Sselect_valid(m_hID);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return  rv > 0;
	}

	bool Dataspace::SetExtentNone()
	{
		return H5Sset_extent_none(m_hID) >= 0;
	}

	bool Dataspace::SetExtentSimple(int rank, const hsize_t* dims, const hsize_t* max_dims /* = nullptr*/)
	{
		return H5Sset_extent_simple(m_hID, rank, dims, max_dims);
	}

	bool Dataspace::SetExtentSimple(const std::vector<hsize_t>& currentDims, const std::vector<hsize_t>& maxDims /*= std::vector<hsize_t>()*/)
	{
		if (maxDims.size() == 0) {
			return H5Sset_extent_simple(m_hID, (int)currentDims.size(), currentDims.data(), nullptr) >= 0;
		}
		else {
			assert(currentDims.size() == maxDims.size());
			return H5Sset_extent_simple(m_hID, (int)currentDims.size(), currentDims.data(), maxDims.data()) >= 0;
		}
	}

	Dataspace& Dataspace::operator=(const Dataspace& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

}
