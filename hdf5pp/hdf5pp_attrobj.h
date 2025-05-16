// hdf5pp_attrobj.h
// An HDF5 object that can have attributes is an AttributedObject
// they are: file, group, dataset, and committed datatype
//
#pragma once

#include "hdf5pp_location.h"

namespace HDF5 {

	class HDF5PP_API Attribute;
	class HDF5PP_API Datatype;
	class HDF5PP_API Dataspace;

	class HDF5PP_API AttributedObject : public Location
	{
	public:
		AttributedObject() = default;
		AttributedObject(const AttributedObject& rhs);
		virtual ~AttributedObject();
		AttributedObject& operator=(const AttributedObject& rhs);

		// take ownership of identifier and will be responsible for closing it
		virtual bool Attach(hid_t hid) override;

		// Creates an attribute attached here
		Attribute CreateAttribute(const char* attr_name, const Datatype& dtype, const Dataspace& dspace, const PropertyList& acpl = PropertyList(), const PropertyList& aapl = PropertyList());

		// Creates an attribute attached to a specified object
		Attribute CreateAttribute(const char* obj_name, const char* attr_name, const Datatype& dtype, const Dataspace& dspace, const PropertyList& acpl = PropertyList(), const PropertyList& aapl = PropertyList(), const PropertyList& lapl = PropertyList());

		// Deletes an attribute from this object
		bool DeleteAttribute(const char* attr_name);

		// Deletes the n-th attribute specified object
		bool DeleteAttribute(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, const PropertyList& lapl = PropertyList());

		// Deletes an attribute attached to a specified object
		bool DeleteAttribute(const char* obj_name, const char* attr_name, const PropertyList& lapl = PropertyList());

		// Does attribute exist?
		bool AttributeExists(const char* attr_name, bool* failed = nullptr);

		// Does attribute exists in specified object?
		bool AttributeExists(const char* obj_name, const char* attr_name, bool* failed = nullptr, const PropertyList& lapl = PropertyList());

		// Retrieves n-th attribute info attached to specified object
		bool GetAttributeInfo(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, H5A_info_t& ainfo, const PropertyList& lapl = PropertyList());

		// Retrieves named attribute info attached to specified object
		bool GetAttributeInfo(const char* obj_name, const char* attr_name, H5A_info_t& ainfo, const PropertyList& lapl = PropertyList());

		// Retrieves n-th attribute name attached to specified object
		bool GetAttributeName(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, std::string& name, const PropertyList& lapl = PropertyList());

		// Get attributes count // DEPRECATED
		// int GetAttributesCount();

		// Apply op to all attributes of this object
		bool IterateAttributes(IndexType tIndex, OrderType tOrder, hsize_t& nOffset, H5A_operator2_t op, void* op_data);

		// Apply op to all attributes of specified object
		bool IterateAttributes(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t& nOffset, H5A_operator2_t op, void* op_data, const PropertyList& lapl = PropertyList());

		// Open attribute of this object
		Attribute OpenAttribute(const char* attr_name, const PropertyList& aapl = PropertyList());

		// Open n-th attribute of specified object
		Attribute OpenAttribute(const char* obj_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, const PropertyList& aapl = PropertyList(), const PropertyList& lapl = PropertyList());

		// Open the named attribute of specified object
		Attribute OpenAttribute(const char* obj_name, const char* attr_name, const PropertyList& aapl = PropertyList(), const PropertyList& lapl = PropertyList());

		// Rename an attribute
		bool RenameAttribute(const char* old_attr_name, const char* new_attr_name);

		// Rename an attributed of a specified object
		bool RenameAttribute(const char* obj_name, const char* old_attr_name, const char* new_attr_name, const PropertyList& lapl = PropertyList());


		////////////////////////////////////////////////////////////////////////////

		//// Add scalar and 1-d array attributes; Will fail if attribute already exists
		bool AddAttribute(const char* name, int8_t c);
		bool AddAttribute(const char* name, uint8_t c);
		bool AddAttribute(const char* name, int16_t c);
		bool AddAttribute(const char* name, uint16_t c);
		bool AddAttribute(const char* name, int32_t c);
		bool AddAttribute(const char* name, uint32_t c);
		bool AddAttribute(const char* name, int64_t c);
		bool AddAttribute(const char* name, uint64_t c);
		bool AddAttribute(const char* name, float c);
		bool AddAttribute(const char* name, double c);
		bool AddAttribute(const char* name, long c);
		bool AddAttribute(const char* name, unsigned long c);
		bool AddAttribute(const char* name, char c);

		bool AddAttribute(const char* name, const std::vector<int8_t>& vals);
		bool AddAttribute(const char* name, const std::vector<uint8_t>& vals);
		bool AddAttribute(const char* name, const std::vector<int16_t>& vals);
		bool AddAttribute(const char* name, const std::vector<uint16_t>& vals);
		bool AddAttribute(const char* name, const std::vector<int32_t>& vals);
		bool AddAttribute(const char* name, const std::vector<uint32_t>& vals);
		bool AddAttribute(const char* name, const std::vector<int64_t>& vals);
		bool AddAttribute(const char* name, const std::vector<uint64_t>& vals);
		bool AddAttribute(const char* name, const std::vector<float>& vals);
		bool AddAttribute(const char* name, const std::vector<double>& vals);

		bool AddAttribute(const char* name, const int8_t* vals, hsize_t length);
		bool AddAttribute(const char* name, const uint8_t* vals, hsize_t length);
		bool AddAttribute(const char* name, const int16_t* vals, hsize_t length);
		bool AddAttribute(const char* name, const uint16_t* vals, hsize_t length);
		bool AddAttribute(const char* name, const int32_t* vals, hsize_t length);
		bool AddAttribute(const char* name, const uint32_t* vals, hsize_t length);
		bool AddAttribute(const char* name, const int64_t* vals, hsize_t length);
		bool AddAttribute(const char* name, const uint64_t* vals, hsize_t length);
		bool AddAttribute(const char* name, const float* vals, hsize_t length);
		bool AddAttribute(const char* name, const double* vals, hsize_t length);

		bool AddAttributeFormat(const char* name, const char* format, ...);
		bool AddAttributeFormat(const char* name, const wchar_t* format, ...);
		bool AddAttributeFormatV(const char* name, const char* format, va_list args);
		bool AddAttributeFormatV(const char* name, const wchar_t* format, va_list args);

		//// Rewrite attribute if it exits, add if not
		template <class T> bool SetAttribute(const char* name, T val);
		template <class T> bool SetAttribute(const char* name, const std::vector<T>& vals);
		template <class T> bool SetAttribute(const char* name, const T* vals, hsize_t length);

		//// Read attribute data; return false if not there, or not convertible
		bool ReadAttribute(const char* name, std::string& val);
		bool ReadAttribute(const char* name, std::wstring& val);
		bool ReadAttribute(const char* name, int8_t& val);
		bool ReadAttribute(const char* name, uint8_t& val);
		bool ReadAttribute(const char* name, int16_t& val);
		bool ReadAttribute(const char* name, uint16_t& val);
		bool ReadAttribute(const char* name, int32_t& val);
		bool ReadAttribute(const char* name, uint32_t& val);
		bool ReadAttribute(const char* name, int64_t& val);
		bool ReadAttribute(const char* name, uint64_t& val);
		bool ReadAttribute(const char* name, float& val);
		bool ReadAttribute(const char* name, double& val);

		bool ReadAttribute(const char* name, std::vector<int8_t>& val);
		bool ReadAttribute(const char* name, std::vector<uint8_t>& val);
		bool ReadAttribute(const char* name, std::vector<int16_t>& val);
		bool ReadAttribute(const char* name, std::vector<uint16_t>& val);
		bool ReadAttribute(const char* name, std::vector<int32_t>& val);
		bool ReadAttribute(const char* name, std::vector<uint32_t>& val);
		bool ReadAttribute(const char* name, std::vector<int64_t>& val);
		bool ReadAttribute(const char* name, std::vector<uint64_t>& val);
		bool ReadAttribute(const char* name, std::vector<float>& val);
		bool ReadAttribute(const char* name, std::vector<double>& val);
	protected:
		explicit AttributedObject(hid_t hid);
		friend class Location;
	};


	template <class T> bool AttributedObject::SetAttribute(const char* name, T val)
	{
		bool failed{ false };
		if (AttributeExists(name, &failed)) {
			failed = !DeleteAttribute(name);
		}

		if (!failed) {
			return AddAttribute(name, val);
		}

		return false;
	}

	template <class T> bool AttributedObject::SetAttribute(const char* name, const std::vector<T>& vals)
	{
		bool failed{ false };
		if (AttributeExists(name, &failed)) {
			failed = !DeleteAttribute(name);
		}

		if (!failed) {
			return AddAttribute(name, vals);
		}

		return false;
	}

	template <class T> bool AttributedObject::SetAttribute(const char* name, const T* vals, hsize_t length)
	{
		bool failed{ false };
		if (AttributeExists(name, &failed)) {
			failed = !DeleteAttribute(name);
		}

		if (!failed) {
			return AddAttribute(name, vals, length);
		}

		return false;
	}
}


