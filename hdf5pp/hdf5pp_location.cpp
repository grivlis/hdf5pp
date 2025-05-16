#include "pch.h"
#include "hdf5pp_location.h"
#include "hdf5pp_file.h"
#include "hdf5pp_object.h"
#include "hdf5pp_group.h"
#include "hdf5pp_dset.h"
#include "hdf5pp_dspace.h"
#include "hdf5pp_dtype.h"
#include "hdf5pp_attrobj.h"

namespace HDF5 {

	Location::Location(const Location& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

#ifdef _DEBUG
	Location::Location(hid_t hid)
	{
		if (hid >= 0) {
			switch (H5Iget_type(hid))
			{
			case H5I_FILE:
			case H5I_GROUP:
			case H5I_DATATYPE:
			case H5I_DATASET:
			case H5I_ATTR:
				m_hID = hid;
				break;
			default:
				assert(false && L"input hid_t is not a Location");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::Location");
#endif
				break;
			}
		}
	}
#else
	Location::Location(hid_t hid) : Handle(hid)
	{

	}
#endif

	// let Handle do its thing
	Location::~Location()
	{

	}

	Location& Location::operator=(const Location& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool Location::Attach(hid_t hid)
	{
		if (hid >= 0) {
			switch (H5Iget_type(hid))
			{
			case H5I_FILE:
			case H5I_GROUP:
			case H5I_DATATYPE:
			case H5I_DATASET:
			case H5I_ATTR:
				DecrementReferenceCount();
				m_hID = hid;
				return true;
			default:
				// hid is not a Location, can't take ownership
				return false;
			}
		}
		else {
			DecrementReferenceCount();
			m_hID = InvalidHandle;
			return true;
		}
	}

	// Get the file containing this object
	File Location::GetFile()
	{
		return File(H5Iget_file_id(m_hID));
	}

	// Retrieves the name for this object
	bool Location::GetName(std::string& name)
	{
		auto s = H5Iget_name(m_hID, nullptr, 0); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Iget_name(m_hID, nm, s + 1);
			if (s2 == s) {
				name = nm;
			}
			delete[] nm;
			return s2 > 0;
		}
		else if (s == 0) {
			name.clear();
			return true;
		}
		return false;
	}

	bool Location::ObjectExists(const char* link_name, const PropertyList& lapl /*= PropertyList()*/, bool* failed /*= nullptr*/)
	{
		auto rv = H5Oexists_by_name(m_hID, link_name, (hid_t)lapl);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv > 0;
	}

	bool Location::GetComment(std::string& comment)
	{
		auto s = H5Oget_comment(m_hID, nullptr, 0); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Oget_comment(m_hID, nm, s + 1);
			if (s2 == s) {
				comment = nm;
			}
			delete[] nm;
			return s2 > 0;
		}
		else if (s == 0) {
			comment.clear();
			return true;
		}
		return false;
	}

	bool Location::GetComment(const char* name, std::string& comment, const PropertyList& lapl /*= PropertyList()*/)
	{
		auto s = H5Oget_comment_by_name(m_hID, name, nullptr, 0, (hid_t)lapl); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Oget_comment_by_name(m_hID, name, nm, s + 1, (hid_t)lapl);
			if (s2 == s) {
				comment = nm;
			}
			delete[] nm;
			return s2 > 0;
		}
		else if (s == 0) {
			comment.clear();
			return true;
		}
		return false;
	}

	bool Location::SetComment(const char* comment)
	{
		return H5Oset_comment(m_hID, comment) >= 0;
	}

	bool Location::SetComment(const char* name, const char* comment, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Oset_comment_by_name(m_hID, name, comment, (hid_t)lapl) >= 0;
	}

	bool Location::GetInfo(H5O_info_t& info, unsigned int fields)
	{
		return H5Oget_info3(m_hID, &info, fields) >= 0;
	}

	bool Location::GetInfo(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, H5O_info_t& info, unsigned int fields, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Oget_info_by_idx3(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, &info, fields, (hid_t)lapl) >= 0;
	}

	bool Location::GetInfo(const char* name, H5O_info_t& info, unsigned int fields, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Oget_info_by_name3(m_hID, name, &info, fields, (hid_t)lapl) >= 0;
	}

	bool Location::GetNativeInfo(H5O_native_info_t& oinfo, unsigned int fields)
	{
		return H5Oget_native_info(m_hID, &oinfo, fields) >= 0;
	}

	bool Location::GetNativeInfo(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, H5O_native_info_t& oinfo, unsigned fields, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Oget_native_info_by_idx(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, &oinfo, fields, (hid_t)lapl) >= 0;
	}

	bool Location::GetNativeInfo(const char* name, H5O_native_info_t& oinfo, unsigned fields, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Oget_native_info_by_name(m_hID, name, &oinfo, fields, (hid_t)lapl) >= 0;
	}

	bool Location::GetFileInfo(H5F_info2_t& finfo)
	{
		return H5Fget_info2(m_hID, &finfo) >= 0;
	}

	bool Location::GetFileName(std::string& name)
	{
		auto s = H5Fget_name(m_hID, nullptr, 0); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Fget_name(m_hID, nm, s + 1);
			if (s2 == s) {
				name = nm;
			}
			delete[] nm;
			return s2 > 0;
		}
		else if (s == 0) {
			name.clear();
			return true;
		}
		return false;
	}

	bool Location::MountFile(const char* name, const File& file, const PropertyList& fmpl /*= PropertyList()*/)
	{
		return H5Fmount(m_hID, name, (hid_t)file, (hid_t)fmpl) >= 0;
	}

	bool Location::UnmountFile(const char* name)
	{
		return H5Funmount(m_hID, name) >= 0;
	}

	bool Location::CommitDatatype(const char* name, const Datatype& dtype, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& tcpl /*= PropertyList()*/, const PropertyList& tapl /*= PropertyList()*/)
	{
		return H5Tcommit2(m_hID, name, (hid_t)dtype, (hid_t)lcpl, (hid_t)tcpl, (hid_t)tapl) >= 0;
	}

	bool Location::CommitDatatype(const Datatype& dtype, const PropertyList& tcpl /*= PropertyList()*/, const PropertyList& tapl /*= PropertyList()*/)
	{
		return H5Tcommit_anon(m_hID, (hid_t)dtype, (hid_t)tcpl, (hid_t)tapl) >= 0;
	}

	Datatype Location::OpenDatatype(const char* name, const PropertyList& tapl /*= PropertyList()*/)
	{
		return Datatype(H5Topen2(m_hID, name, (hid_t)tapl));
	}

	bool Location::CreateReference(void* ref, const char* name)
	{
		return H5Rcreate(ref, m_hID, name, H5R_OBJECT, -1) >= 0;
	}

	bool Location::CreateReference(void* ref, const char* name, const Dataspace& dspace)
	{
		return H5Rcreate(ref, m_hID, name, H5R_DATASET_REGION, (hid_t)dspace) >= 0;
	}

	AttributedObject Location::DereferenceObject(void* ref, const PropertyList& oapl /*= PropertyList()*/)
	{
		return AttributedObject(H5Rdereference2(m_hID, (hid_t)oapl, H5R_OBJECT, ref));
	}

	HDF5::AttributedObject Location::DereferenceDatasetRegion(void* ref, const PropertyList& oapl /*= PropertyList()*/)
	{
		return AttributedObject(H5Rdereference2(m_hID, (hid_t)oapl, H5R_DATASET_REGION, ref));
	}

	bool Location::GetReferencedObjectName(void* ref, std::string& name)
	{
		auto s = H5Rget_name(m_hID, H5R_OBJECT, ref, nullptr, 0); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Rget_name(m_hID, H5R_OBJECT, ref, nm, s + 1);
			if (s2 == s) {
				name = nm;
			}
			delete[] nm;
			return s2 > 0;
		}
		else if (s == 0) {
			name.clear();
			return true;
		}
		return false;
	}

	bool Location::GetReferencedDatasetRegionName(void* ref, std::string& name)
	{
		auto s = H5Rget_name(m_hID, H5R_DATASET_REGION, ref, nullptr, 0); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Rget_name(m_hID, H5R_DATASET_REGION, ref, nm, s + 1);
			if (s2 == s) {
				name = nm;
			}
			delete[] nm;
			return s2 > 0;
		}
		else if (s == 0) {
			name.clear();
			return true;
		}
		return false;
	}

	bool Location::GetReferencedObjectType(void* ref, H5O_type_t& ot)
	{
		return H5Rget_obj_type2(m_hID, H5R_OBJECT, ref, &ot) >= 0;
	}

	bool Location::GetReferencedDatasetRegionType(void* ref, H5O_type_t& ot)
	{
		return H5Rget_obj_type2(m_hID, H5R_DATASET_REGION, ref, &ot) >= 0;
	}

	Dataspace Location::GetReferencedDatasetRegion(void* ref)
	{
		return Dataspace(H5Rget_region(m_hID, H5R_DATASET_REGION, ref));
	}

	Object Location::OpenObject(const char* name, const PropertyList& lapl /*= PropertyList()*/)
	{
		return Object(H5Oopen(m_hID, name, (hid_t)lapl));
	}

	Object Location::OpenObject(haddr_t addr)
	{
		return Object(H5Oopen_by_addr(m_hID, addr));
	}

	Object Location::OpenObject(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, const PropertyList& lapl /*= PropertyList()*/)
	{
		return Object(H5Oopen_by_idx(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, (hid_t)lapl));
	}

	HDF5::Object Location::OpenObject(H5O_token_t token)
	{
		return Object(H5Oopen_by_token(m_hID, token));
	}

	bool Location::VisitObjects(IndexType tIndex, OrderType tOrder, H5O_iterate_t op, void* op_data, unsigned int fields)
	{
		return H5Ovisit3(m_hID, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, op, op_data, fields) >= 0;
	}

	bool Location::VisitObjects(const char* object_name, IndexType tIndex, OrderType tOrder, H5O_iterate_t op, void* op_data, unsigned int fields, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Ovisit_by_name3(m_hID, object_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, op, op_data, fields, (hid_t)lapl) >= 0;
	}

	bool Location::CompareTokens(const H5O_token_t& token1, const H5O_token_t& token2, int& cmp_value)
	{
		return H5Otoken_cmp(m_hID, &token1, &token2, &cmp_value) >= 0;
	}

	bool Location::TokenFromString(const char* token_str, H5O_token_t& token)
	{
		return H5Otoken_from_str(m_hID, token_str, &token) >= 0;
	}

	bool Location::TokenToString(const H5O_token_t& token, char** token_str)
	{
		return H5Otoken_to_str(m_hID, &token, token_str) >= 0;
	}

	bool Location::CreateExternalLink(const char* target_file_name, const char* target_object_name, const char* link_name, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lcreate_external(target_file_name, target_object_name, m_hID, link_name, (hid_t)lcpl, (hid_t)lapl) >= 0;
	}

	bool Location::CreateHardLink(const Location& srcLoc, const char* srcName, const char* dst_link_name, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return Location::CreateHardLink(srcLoc, srcName, *this, dst_link_name, lcpl, lapl);
	}

	bool Location::CreateSoftLink(const char* target_path, const char* link_name, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return Location::CreateSoftLink(target_path, *this, link_name, lcpl, lapl);
	}

	bool Location::CreateUserDefinedLink(const char* link_name, H5L_type_t link_type, const char* udata, size_t udata_size, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lcreate_ud(m_hID, link_name, link_type, udata, udata_size, (hid_t)lcpl, (hid_t)lapl) >= 0;
	}

	bool Location::CreateUserDefinedLink(const char* link_name, H5L_type_t link_type, const std::vector<char>& udata, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lcreate_ud(m_hID, link_name, link_type, udata.data(), udata.size(), (hid_t)lcpl, (hid_t)lapl) >= 0;
	}

	bool Location::IsUserDefinedLinkClassRegisterd(H5L_type_t link_cls_id, bool* failed /*= nullptr*/)
	{
		auto rv = H5Lis_registered(link_cls_id);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv > 0;
	}

	bool Location::RegisterUserDefinedLinkClass(const H5L_class_t& link_class)
	{
		return H5Lregister(&link_class) >= 0;
	}

	bool Location::UnregisterUserDefinedLinkClass(H5L_type_t link_cls_id)
	{
		return H5Lunregister(link_cls_id) >= 0;
	}

	bool Location::DeleteLink(const char* name, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Ldelete(m_hID, name, (hid_t)lapl) >= 0;
	}

	bool Location::DeleteLink(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Ldelete_by_idx(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, (hid_t)lapl) >= 0;
	}

	bool Location::LinkExists(const char* name, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lexists(m_hID, name, (hid_t)lapl) > 0;
	}

	bool Location::GetLinkInfo(const char* link_name, H5L_info_t& info, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lget_info(m_hID, link_name, &info, (hid_t)lapl) >= 0;
	}

	bool Location::GetLinkInfo(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, H5L_info_t& info, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lget_info_by_idx(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, &info, (hid_t)lapl) >= 0;
	}

	bool Location::GetLinkName(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, std::string& name, const PropertyList& lapl /*= PropertyList()*/)
	{
		auto s = H5Lget_name_by_idx(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, nullptr, 0, (hid_t)lapl); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Lget_name_by_idx(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, nm, s + 1, (hid_t)lapl);
			if (s2 == s) {
				name = nm;
			}
			delete[] nm;
			return s2 > 0;
		}
		else if (s == 0) {
			name.clear();
			return true;
		}
		return false;
	}

	bool Location::GetLinkValue(const char* link_name, void* linkval_buff, size_t size, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lget_val(m_hID, link_name, linkval_buff, size, (hid_t)lapl) >= 0;
	}

	bool Location::GetLinkValue(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, void* linkval_buf, size_t size, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lget_val_by_idx(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, linkval_buf, size, (hid_t)lapl) >= 0;
	}

	bool Location::Iterate(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t& idx, H5L_iterate2_t op, void* op_data, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Literate_by_name2(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, &idx, op, op_data, (hid_t)lapl) >= 0;
	}

	bool Location::Visit(const char* group_name, IndexType tIndex, OrderType tOrder, H5L_iterate2_t op, void* op_data, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lvisit_by_name2(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, op, op_data, (hid_t)lapl) >= 0;
	}

	bool Location::UnpackExternalLinkValue(char* ext_linkval, size_t link_size, unsigned int* flags, const char** filename, const char** obj_path)
	{
		return H5Lunpack_elink_val(ext_linkval, link_size, flags, filename, obj_path) >= 0;
	}

	bool Location::CopyLink(const Location& srcLoc, const char* srcName, const Location& dstLoc, const char* dstName, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lcopy((hid_t)srcLoc, srcName, (hid_t)dstLoc, dstName, (hid_t)lcpl, (hid_t)lapl) >= 0;
	}

	bool Location::CreateHardLink(const Location& srcLoc, const char* srcName, const Location& dstLoc, const char* dst_link_name, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lcreate_hard((hid_t)srcLoc, srcName, (hid_t)dstLoc, dst_link_name, (hid_t)lcpl, (hid_t)lapl) >= 0;
	}

	bool Location::CreateSoftLink(const char* targetPath, const Location& linkLoc, const char* dst_link_name, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lcreate_soft(targetPath, (hid_t)linkLoc, dst_link_name, (hid_t)lcpl, (hid_t)lapl) >= 0;
	}

	bool Location::MoveLink(const Location& srcLoc, const char* src_name, const Location& dstLoc, const char* dest_name, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Lmove((hid_t)srcLoc, src_name, (hid_t)dstLoc, dest_name, (hid_t)lcpl, (hid_t)lapl) >= 0;
	}

	Group Location::CreateGroup(const char* group_name, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& gcpl /*= PropertyList()*/, const PropertyList& gapl /*= PropertyList()*/)
	{
		return Group(H5Gcreate2(m_hID, group_name, (hid_t)lcpl, (hid_t)gcpl, (hid_t)gapl));
	}

	Group Location::CreateGroup(const PropertyList& gcpl /*= PropertyList()*/, const PropertyList& gapl /*= PropertyList()*/)
	{
		return Group(H5Gcreate_anon(m_hID, (hid_t)gcpl, (hid_t)gapl));
	}

	bool Location::GetGroupInfo(H5G_info_t& ginfo)
	{
		return H5Gget_info(m_hID, &ginfo) >= 0;
	}

	bool Location::GetGroupInfo(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t n, H5G_info_t& ginfo, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Gget_info_by_idx(m_hID, group_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, n, &ginfo, (hid_t)lapl);
	}

	bool Location::GetGroupInfo(const char* group_name, H5G_info_t& group_info, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Gget_info_by_name(m_hID, group_name, &group_info, (hid_t)lapl) >= 0;
	}

	Group Location::OpenGroup(const char* group_name, const PropertyList& gapl /*= PropertyList()*/)
	{
		return Group(H5Gopen2(m_hID, group_name, (hid_t)gapl));
	}

	Dataset Location::CreateDataset(const char* dataset_name, const Datatype& dtype, const Dataspace& dspace, const PropertyList& lcpl /*= PropertyList()*/, const PropertyList& dcpl /*= PropertyList()*/, const PropertyList& dapl /*= PropertyList()*/)
	{
		return Dataset(H5Dcreate2(m_hID, dataset_name, (hid_t)dtype, (hid_t)dspace, (hid_t)lcpl, (hid_t)dcpl, (hid_t)dapl));
	}

	HDF5::Dataset Location::CreateDataset(const Datatype& dtype, const Dataspace& dspace, const PropertyList& dcpl /*= PropertyList()*/, const PropertyList& dapl /*= PropertyList()*/)
	{
		return Dataset(H5Dcreate_anon(m_hID, (hid_t)dtype, (hid_t)dspace, (hid_t)dcpl, (hid_t)dapl));
	}

	HDF5::Dataset Location::OpenDataset(const char* name, const PropertyList& dapl /*= PropertyList()*/)
	{
		return Dataset(H5Dopen2(m_hID, name, (hid_t)dapl));
	}

	bool Location::FlushFile(FlushScope scope)
	{
		return H5Fflush(m_hID, (H5F_scope_t)scope) >= 0;
	}

#define ADDDSET(x, y) bool Location::AddDataset(const char* name, const std::vector<x>& vals, const std::vector<size_t>& dims)\
	{\
	std::vector<hsize_t> dd;\
	if (dims.empty()) {\
		dd.push_back(vals.size());\
	}\
	else {\
		size_t elems{ 1 };\
		for (auto s : dims) {\
			elems *= s;\
		}\
		if (elems != vals.size()) {\
			return false;\
		}\
		dd.assign(dims.cbegin(), dims.cend());\
	}\
\
	Handle dspace(H5Screate_simple((int)dd.size(), dd.data(), nullptr));\
	if (!dspace.IsValid()) {\
		return false;\
	}\
		\
	Handle dset(H5Dcreate2(m_hID, name, (hid_t)y, (hid_t)dspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT));\
	if (!dset.IsValid()) {\
		return false;\
	}\
		\
	return H5Dwrite((hid_t)dset, (hid_t)y, (hid_t)dspace, (hid_t)dspace, H5P_DEFAULT, vals.data()) >= 0;\
	}\


	ADDDSET(int8_t, IntegerPDT::Native_INT8)
	ADDDSET(uint8_t, IntegerPDT::Native_UINT8)
	ADDDSET(int16_t, IntegerPDT::Native_INT16)
	ADDDSET(uint16_t, IntegerPDT::Native_UINT16)
	ADDDSET(int32_t, IntegerPDT::Native_INT32)
	ADDDSET(uint32_t, IntegerPDT::Native_UINT32)
	ADDDSET(int64_t, IntegerPDT::Native_INT64)
	ADDDSET(uint64_t, IntegerPDT::Native_UINT64)
	ADDDSET(float, FloatPDT::Native_FLOAT)
	ADDDSET(double, FloatPDT::Native_DOUBLE)
	ADDDSET(long, IntegerPDT::Native_LONG)
	ADDDSET(unsigned long, IntegerPDT::Native_ULONG)

#define ADDDSET2(x, y) bool Location::AddDataset(const char* name, const x* vals, size_t dim1, size_t dim2, size_t dim3, size_t dim4)\
	{\
		if (dim1 == 0 || dim2 == 0 || dim3 == 0 || dim4 == 0) {\
			return false;\
		}\
	std::vector<hsize_t> dd;\
	dd.push_back(dim1);\
	if (dim2 > 1) {\
		dd.push_back(dim2);\
	}\
	if (dim3 > 1) {\
		dd.push_back(dim3);\
	}\
	if (dim4 > 1) {\
		dd.push_back(dim4);\
	}\
\
	Handle dspace(H5Screate_simple((int)dd.size(), dd.data(), nullptr));\
	if (!dspace.IsValid()) {\
		return false;\
	}\
\
	Handle dset(H5Dcreate2(m_hID, name, (hid_t)y, (hid_t)dspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT));\
	if (!dset.IsValid()) {\
		return false;\
	}\
\
	return H5Dwrite((hid_t)dset, (hid_t)y, (hid_t)dspace, (hid_t)dspace, H5P_DEFAULT, vals) >= 0;\
\
	}\

	ADDDSET2(int8_t, IntegerPDT::Native_INT8)
	ADDDSET2(uint8_t, IntegerPDT::Native_UINT8)
	ADDDSET2(int16_t, IntegerPDT::Native_INT16)
	ADDDSET2(uint16_t, IntegerPDT::Native_UINT16)
	ADDDSET2(int32_t, IntegerPDT::Native_INT32)
	ADDDSET2(uint32_t, IntegerPDT::Native_UINT32)
	ADDDSET2(int64_t, IntegerPDT::Native_INT64)
	ADDDSET2(uint64_t, IntegerPDT::Native_UINT64)
	ADDDSET2(float, FloatPDT::Native_FLOAT)
	ADDDSET2(double, FloatPDT::Native_DOUBLE)
	ADDDSET2(long, IntegerPDT::Native_LONG)
	ADDDSET2(unsigned long, IntegerPDT::Native_ULONG)

#undef ADDDSET
#undef ADDDSET2


	bool Location::AddDataset(const char* name, const std::vector<std::string> & vStr)
	{

		std::vector<const char*> strs(vStr.size());
		for (size_t i = 0; i < vStr.size(); ++i) {
			strs[i] = vStr[i].data();
		}
		return AddDataset(name, strs);
	}

	bool Location::AddDataset(const char* name, const std::vector<const char*>& vStr)
	{
		std::vector<hsize_t> dims = { vStr.size() };
		Dataspace dspace(dims);
		if (!dspace.IsValid()) {
			return false;
		}
		Datatype dtype(StringPDT::C_S1);
		if (!dtype.IsValid()) {
			return false;
		}
		if (!dtype.SetSize(H5T_VARIABLE)) {
			return false;
		}
		auto dset = CreateDataset(name, dtype, dspace);
		if (!dset.IsValid()) {
			return false;
		}
		return dset.Write(dtype, dspace, dspace, vStr.data());

	}

}