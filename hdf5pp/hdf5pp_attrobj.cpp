#include "pch.h"
#include "hdf5pp_attrobj.h"

#include "hdf5pp_attribute.h"
#include "hdf5pp_dtype.h"
#include "hdf5pp_dspace.h"

namespace HDF5 {

	AttributedObject::AttributedObject(const AttributedObject& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

	// let Location to its thing
	AttributedObject::~AttributedObject()
	{

	}

	AttributedObject& AttributedObject::operator=(const AttributedObject& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool AttributedObject::Attach(hid_t hid)
	{
		if (hid >= 0) {
			switch (H5Iget_type(hid)) {
			case H5I_GROUP:
			case H5I_DATASET:
			case H5I_DATATYPE:
			case H5I_FILE:
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

	HDF5::Attribute AttributedObject::CreateAttribute(const char* attr_name, const Datatype& dtype, const Dataspace& dspace, const PropertyList& acpl /*= PropertyList()*/, const PropertyList& aapl /*= PropertyList()*/)
	{
		return Attribute(H5Acreate2(m_hID, attr_name, (hid_t)dtype, (hid_t)dspace, (hid_t)acpl, (hid_t)aapl));
	}

	HDF5::Attribute AttributedObject::CreateAttribute(const char* obj_name, const char* attr_name, const Datatype& dtype, const Dataspace& dspace, const PropertyList& acpl /*= PropertyList()*/, const PropertyList& aapl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return Attribute(H5Acreate_by_name(m_hID, obj_name, attr_name, (hid_t)dtype, (hid_t)dspace, (hid_t)acpl, (hid_t)aapl, (hid_t)lapl));
	}

	bool AttributedObject::DeleteAttribute(const char* attr_name)
	{
		return H5Adelete(m_hID, attr_name) >= 0;
	}

	bool AttributedObject::DeleteAttribute(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Adelete_by_idx(m_hID, obj_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, (hid_t)lapl) >= 0;
	}

	bool AttributedObject::DeleteAttribute(const char* obj_name, const char* attr_name, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Adelete_by_name(m_hID, obj_name, attr_name, (hid_t)lapl) >= 0;
	}

	bool AttributedObject::AttributeExists(const char* attr_name, bool* failed /* = nullptr */)
	{
		auto rv = H5Aexists(m_hID, attr_name);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv > 0;
	}

	bool AttributedObject::AttributeExists(const char* obj_name, const char* attr_name, bool* failed /* = nullptr */, const PropertyList& lapl /* = PropertyList() */)
	{
		auto rv = H5Aexists_by_name(m_hID, obj_name, attr_name, (hid_t)lapl);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv > 0;
	}

	bool AttributedObject::GetAttributeInfo(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, H5A_info_t& ainfo, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Aget_info_by_idx(m_hID, obj_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, &ainfo, (hid_t)lapl) >= 0;
	}

	bool AttributedObject::GetAttributeInfo(const char* obj_name, const char* attr_name, H5A_info_t& ainfo, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Aget_info_by_name(m_hID, obj_name, attr_name, &ainfo, (hid_t)lapl) >= 0;
	}

	bool AttributedObject::GetAttributeName(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, std::string& name, const PropertyList& lapl /*= PropertyList()*/)
	{
		auto s = H5Aget_name_by_idx(m_hID, obj_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, nullptr, 0, (hid_t)lapl); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Aget_name_by_idx(m_hID, obj_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, nm, s + 1, (hid_t)lapl);
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

	// DEPRECATED
	//int AttributedObject::GetAttributesCount()
	//{
	//	return H5Aget_num_attrs(m_hID);
	//}

	bool AttributedObject::IterateAttributes(IndexType tIndex, OrderType tOrder, hsize_t& nOffset, H5A_operator2_t op, void* op_data)
	{
		return H5Aiterate2(m_hID, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, &nOffset, op, op_data) >= 0;
	}

	bool AttributedObject::IterateAttributes(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t& nOffset, H5A_operator2_t op, void* op_data, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Aiterate_by_name(m_hID, obj_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, &nOffset, op, op_data, (hid_t)lapl) >= 0;
	}

	HDF5::Attribute AttributedObject::OpenAttribute(const char* attr_name, const PropertyList& aapl /*= PropertyList()*/)
	{
		return Attribute(H5Aopen(m_hID, attr_name, (hid_t)aapl));
	}

	HDF5::Attribute AttributedObject::OpenAttribute(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, const PropertyList& aapl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return Attribute(H5Aopen_by_idx(m_hID, obj_name, (H5_index_t)tIndex, (H5_iter_order_t)tOrder, nOffset, (hid_t)aapl, (hid_t)lapl));
	}

	HDF5::Attribute AttributedObject::OpenAttribute(const char* obj_name, const char* attr_name, const PropertyList& aapl /*= PropertyList()*/, const PropertyList& lapl /*= PropertyList()*/)
	{
		return Attribute(H5Aopen_by_name(m_hID, obj_name, attr_name, (hid_t)aapl, (hid_t)lapl));
	}

	bool AttributedObject::RenameAttribute(const char* old_attr_name, const char* new_attr_name)
	{
		return H5Arename(m_hID, old_attr_name, new_attr_name) >= 0;
	}

	bool AttributedObject::RenameAttribute(const char* obj_name, const char* old_attr_name, const char* new_attr_name, const PropertyList& lapl /*= PropertyList()*/)
	{
		return H5Arename_by_name(m_hID, obj_name, old_attr_name, new_attr_name, (hid_t)lapl) >= 0;
	}

#define ADDATTR(x, y) bool AttributedObject::AddAttribute(const char* name, x c)\
	{\
		auto attr = CreateAttribute(name, y, Dataspace());\
		if (attr.IsValid()) {\
			return attr.Write(y, &c);\
		}\
		return false;\
	}\


	ADDATTR(int8_t, IntegerPDT::Native_INT8)
	ADDATTR(uint8_t, IntegerPDT::Native_UINT8)
	ADDATTR(int16_t, IntegerPDT::Native_INT16)
	ADDATTR(uint16_t, IntegerPDT::Native_UINT16)
	ADDATTR(int32_t, IntegerPDT::Native_INT32)
	ADDATTR(uint32_t, IntegerPDT::Native_UINT32)
	ADDATTR(int64_t, IntegerPDT::Native_INT64)
	ADDATTR(uint64_t, IntegerPDT::Native_UINT64)
	ADDATTR(float, FloatPDT::Native_FLOAT)
	ADDATTR(double, FloatPDT::Native_DOUBLE)
		ADDATTR(long, IntegerPDT::Native_LONG)
		ADDATTR(unsigned long, IntegerPDT::Native_ULONG)
		ADDATTR(char, IntegerPDT::Native_CHAR)


#define ADDATTR2(x, y) bool AttributedObject::AddAttribute(const char* name, const std::vector<x>& vals)\
	{\
		if (vals.size() > 0) {\
			std::vector<hsize_t> dims = { vals.size() };\
			Dataspace dspace(dims);\
			if (dspace.IsValid()) {\
				auto attr = CreateAttribute(name, y, dspace);\
				if (attr.IsValid()) {\
					return attr.Write(y, vals.data());\
				}\
			}\
		}\
	return false;\
	}\



	ADDATTR2(int8_t, IntegerPDT::Native_INT8)
	ADDATTR2(uint8_t, IntegerPDT::Native_UINT8)
	ADDATTR2(int16_t, IntegerPDT::Native_INT16)
	ADDATTR2(uint16_t, IntegerPDT::Native_UINT16)
	ADDATTR2(int32_t, IntegerPDT::Native_INT32)
	ADDATTR2(uint32_t, IntegerPDT::Native_UINT32)
	ADDATTR2(int64_t, IntegerPDT::Native_INT64)
	ADDATTR2(uint64_t, IntegerPDT::Native_UINT64)
	ADDATTR2(float, FloatPDT::Native_FLOAT)
	ADDATTR2(double, FloatPDT::Native_DOUBLE)

#define ADDATTR3(x, y) bool AttributedObject::AddAttribute(const char* name, const x* vals, hsize_t length)\
	{\
		if (length > 0) {\
			std::vector<hsize_t> dims = { length };\
			Dataspace dspace(dims);\
			if (dspace.IsValid()) {\
				auto attr = CreateAttribute(name, y, dspace);\
				if (attr.IsValid()) {\
					return attr.Write(y, vals);\
				}\
			}\
		}\
		return false;\
	}\

	ADDATTR3(int8_t, IntegerPDT::Native_INT8)
	ADDATTR3(uint8_t, IntegerPDT::Native_UINT8)
	ADDATTR3(int16_t, IntegerPDT::Native_INT16)
	ADDATTR3(uint16_t, IntegerPDT::Native_UINT16)
	ADDATTR3(int32_t, IntegerPDT::Native_INT32)
	ADDATTR3(uint32_t, IntegerPDT::Native_UINT32)
	ADDATTR3(int64_t, IntegerPDT::Native_INT64)
	ADDATTR3(uint64_t, IntegerPDT::Native_UINT64)
	ADDATTR3(float, FloatPDT::Native_FLOAT)
	ADDATTR3(double, FloatPDT::Native_DOUBLE)

#undef ADDATTR
#undef ADDATTR2
#undef ADDATTR3

	bool AttributedObject::AddAttributeFormat(const char* name, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		auto ss = vsnprintf(nullptr, 0, format, args);
		auto msg = new char[ss + 10];
		vsprintf_s(msg, ss + 10, format, args);
		va_end(args);

		StringDatatype dtype(strlen(msg));
		Dataspace dspace;
		auto attr = CreateAttribute(name, dtype, dspace);
		auto rv = attr.Write(dtype, msg);
		delete[] msg;
		return rv;
	}

#pragma warning(push)
#pragma warning(disable:4996)
	bool AttributedObject::AddAttributeFormat(const char* name, const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		auto ss = _vsnwprintf(nullptr, 0, format, args);
		auto msg = new wchar_t[ss + 10];
		vswprintf_s(msg, ss + 10, format, args);
		va_end(args);

		auto len = WideCharToMultiByte(CP_UTF8, 0, msg, -1, NULL, 0, 0, 0);
		char* pA = new char[len + 10];
		WideCharToMultiByte(CP_UTF8, 0, msg, -1, pA, len + 10, 0, 0);


		StringDatatype dtype(StringPDT::C_S1);
		dtype.SetCharSet(StringDatatype::CharSet::UTF8);
		dtype.SetSize(strlen(pA));
		auto attr = CreateAttribute(name, dtype, Dataspace());

		auto rv = attr.Write(dtype, pA);
		delete[] pA;
		delete[] msg;
		return rv;
	}
#pragma warning(pop)

	bool AttributedObject::AddAttributeFormatV(const char* name, const char* format, va_list args)
	{
		auto ss = vsnprintf(nullptr, 0, format, args);
		auto msg = new char[ss + 10];
		vsprintf_s(msg, ss + 10, format, args);

		StringDatatype dtype(strlen(msg));
		Dataspace dspace;
		auto attr = CreateAttribute(name, dtype, dspace);
		auto rv = attr.Write(dtype, msg);
		delete[] msg;
		return rv;
	}

#pragma warning(push)
#pragma warning(disable:4996)

	bool AttributedObject::AddAttributeFormatV(const char* name, const wchar_t* format, va_list args)
	{
		auto ss = _vsnwprintf(nullptr, 0, format, args);
		auto msg = new wchar_t[(size_t)ss + 10];
		vswprintf_s(msg, (size_t)ss + 10, format, args);

		auto len = WideCharToMultiByte(CP_UTF8, 0, msg, -1, NULL, 0, 0, 0);
		char* pA = new char[(size_t)len + 10];
		WideCharToMultiByte(CP_UTF8, 0, msg, -1, pA, len + 10, 0, 0);


		StringDatatype dtype(StringPDT::C_S1);
		dtype.SetCharSet(StringDatatype::CharSet::UTF8);
		dtype.SetSize(strlen(pA));
		auto attr = CreateAttribute(name, dtype, Dataspace());

		auto rv = attr.Write(dtype, pA);
		delete[] pA;
		delete[] msg;
		return rv;
	}

	bool AttributedObject::ReadAttribute(const char* name, std::string& val)
	{
		if (!AttributeExists(name)) {
			return false;
		}

		auto attr = OpenAttribute(name);
		if (!attr.IsValid()) {
			return false;
		}

		auto dtype = attr.GetDatatype();
		auto dspace = attr.GetDataspace();
		if (!dtype.IsValid() || !dspace.IsValid()) {
			return false;
		}
		Datatype::ClassType cls;
		if (!dtype.GetClass(cls)) {
			return false;
		}

		if (cls != Datatype::ClassType::String) {
			return false;
		}

		auto sz = dtype.GetSize();
		auto pA = new char[sz + 1];
		auto rv = attr.Read(dtype, pA);
		pA[sz] = 0;
		if (rv) {
			val = pA;

		}
		delete[] pA;
		return rv;
	}

	bool AttributedObject::ReadAttribute(const char* name, std::wstring& val)
	{
		if (!AttributeExists(name)) {
			return false;
		}

		auto attr = OpenAttribute(name);
		if (!attr.IsValid()) {
			return false;
		}

		auto dtype = attr.GetDatatype();
		auto dspace = attr.GetDataspace();
		if (!dtype.IsValid() || !dspace.IsValid()) {
			return false;
		}
		Datatype::ClassType cls;
		if (!dtype.GetClass(cls)) {
			return false;
		}

		if (cls != Datatype::ClassType::String) {
			return false;
		}

		auto sz = dtype.GetSize();
		auto pA = new char[sz + 1];
		auto rv = attr.Read(dtype, pA);
		pA[sz] = 0;
		if (rv) {
			auto codepage = H5Tget_cset((hid_t)dtype) == H5T_CSET_ASCII ? CP_ACP : CP_UTF8;
			auto len = MultiByteToWideChar(codepage, 0, pA, -1, nullptr, 0);
			wchar_t* wstr = new wchar_t[len + 10];
			MultiByteToWideChar(codepage, 0, pA, -1, wstr, len + 10);
			val = wstr;
			delete[] wstr;
		}
		delete[] pA;
		return rv;
	}

#define READATTR(x, y) bool AttributedObject::ReadAttribute(const char* name, x& val)\
	{\
		if (!AttributeExists(name)) {\
			return false;\
		}\
\
		auto attr = OpenAttribute(name);\
		if (!attr.IsValid()) {\
			return false;\
		}\
\
		auto dtype = attr.GetDatatype();\
		auto dspace = attr.GetDataspace();\
		if (!dtype.IsValid() || !dspace.IsValid()) {\
			return false;\
		}\
\
		auto rr = dspace.GetSimpleExtentDimsCount();\
		if (rr < 0) {\
			return false;\
		}\
\
		size_t sz = dtype.GetSize();\
		if (rr > 0) {\
			std::vector<hsize_t> vv(rr);\
			if (dspace.GetSimpleExtentDims(vv.data()) < 0) {\
				return false;\
			}\
			for (auto s : vv) {\
				if (s < 0) {\
					return false;\
				}\
				sz *= (size_t)s;\
			}\
		}\
		if (sz == 0) {\
			return false;\
		}\
		if (sizeof(val) < sz) {\
			return false;\
		}\
\
		return attr.Read(y, &val);\
	}\

	READATTR(int8_t, IntegerPDT::Native_INT8)
	READATTR(uint8_t, IntegerPDT::Native_UINT8)
	READATTR(int16_t, IntegerPDT::Native_INT16)
	READATTR(uint16_t, IntegerPDT::Native_UINT16)
	READATTR(int32_t, IntegerPDT::Native_INT32)
	READATTR(uint32_t, IntegerPDT::Native_UINT32)
	READATTR(int64_t, IntegerPDT::Native_INT64)
	READATTR(uint64_t, IntegerPDT::Native_UINT64)
	READATTR(float, FloatPDT::Native_FLOAT)
	READATTR(double, FloatPDT::Native_DOUBLE)


#define READATTR2(x, y) bool AttributedObject::ReadAttribute(const char* name, std::vector<x>& vals)\
	{\
		if (!AttributeExists(name)) {\
			return false;\
		}\
\
		auto attr = OpenAttribute(name);\
		if (!attr.IsValid()) {\
			return false;\
		}\
\
		auto dtype = attr.GetDatatype();\
		auto dspace = attr.GetDataspace();\
		if (!dtype.IsValid() || !dspace.IsValid()) {\
			return false;\
		}\
\
		auto nelems = dspace.GetSimpleExtentElementsCount();\
		if (nelems < 0) {\
			return false;\
		}\
\
		size_t sz = dtype.GetSize();\
		if (sz == 0) {\
			return false;\
		}\
\
		vals.resize((size_t)nelems);\
		if (sizeof(vals[0]) < sz) {\
			return false;\
		}\
\
		return attr.Read(y, vals.data());\
	}\

	READATTR2(int8_t, IntegerPDT::Native_INT8)
	READATTR2(uint8_t, IntegerPDT::Native_UINT8)
	READATTR2(int16_t, IntegerPDT::Native_INT16)
	READATTR2(uint16_t, IntegerPDT::Native_UINT16)
	READATTR2(int32_t, IntegerPDT::Native_INT32)
	READATTR2(uint32_t, IntegerPDT::Native_UINT32)
	READATTR2(int64_t, IntegerPDT::Native_INT64)
	READATTR2(uint64_t, IntegerPDT::Native_UINT64)
	READATTR2(float, FloatPDT::Native_FLOAT)
	READATTR2(double, FloatPDT::Native_DOUBLE)

#undef READATTR
#undef READATTR2

#pragma warning(pop)

#ifdef _DEBUG
	AttributedObject::AttributedObject(hid_t hid)
	{
		if (hid >= 0) {
			switch (H5Iget_type(hid)) {
			case H5I_GROUP:
			case H5I_DATASET:
			case H5I_DATATYPE:
			case H5I_FILE:
				m_hID = hid;
				break;
			default:
				assert(false && L"input hid_t is not an AttributedObject");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::AttributedObject");
#endif
				break;
			}

		}
		else {
			m_hID = InvalidHandle;
		}
	}
#else
	AttributedObject::AttributedObject(hid_t hid) : Location(hid)
	{
	}
#endif
}

