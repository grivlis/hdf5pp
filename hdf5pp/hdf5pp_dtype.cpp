#include "pch.h"
#include "hdf5pp_dtype.h"


namespace HDF5 {


	//////////////////////////////////////////////////////////////////////////

	Datatype::Datatype(const Datatype& rhs)
	{
		m_hID = H5Tcopy(rhs.m_hID);
	}


#ifdef _DEBUG
	Datatype::Datatype(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not a Datatype");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::Datatype");
#endif

			}
		}
	}
#else
	Datatype::Datatype(hid_t hid) : AttributedObject(hid)
	{

	}
#endif

	Datatype::Datatype()
	{
		m_hID = InvalidHandle;
	}

	Datatype::Datatype(unsigned char* buf)
	{
		m_hID = H5Tdecode(buf);
	}

	Datatype::~Datatype()
	{
		if (m_hID >= 0) {
			H5Tclose(m_hID);
		}
		m_hID = InvalidHandle;
	}

	bool Datatype::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				H5Tclose(m_hID);
				m_hID = hid;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			H5Tclose(m_hID);
			m_hID = InvalidHandle;
			return true;
		}
	}

	bool Datatype::IsCommitted(bool* failed /*= nullptr*/)
	{
		auto rv = H5Tcommitted(m_hID);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv > 0;
	}

	bool Datatype::IsCompilerConversion(const Datatype& dst_dtype, bool* failed /* = nullptr */)
	{
		auto rv = H5Tcompiler_conv(m_hID, (hid_t)dst_dtype);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv >= 0;
	}

	bool Datatype::Convert(const Datatype& dst_dtype, size_t num_elements, void* buf, void* background, const PropertyList& xpl /*= PropertyList()*/)
	{
		return H5Tconvert(m_hID, (hid_t)dst_dtype, num_elements, buf, background, (hid_t)xpl) >= 0;
	}

	Datatype Datatype::Copy()
	{
		return Datatype(H5Tcopy(m_hID));
	}

	bool Datatype::DetectClass(ClassType t, bool* failed /* = nullptr */)
	{
		auto rv = H5Tdetect_class(m_hID, (H5T_class_t)t);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv > 0;
	}

	bool Datatype::Encode(unsigned char* buf, size_t& buf_size)
	{
		return H5Tencode(m_hID, buf, &buf_size) >= 0;
	}

	bool Datatype::Encode(std::vector<unsigned char>& buf)
	{
		size_t n{ 0 };
		if (Encode(nullptr, n)) {
			buf.resize(n);
			return Encode(buf.data(), n);
		}
		return false;
	}

	bool Datatype::Equals(const Datatype& other_dtype, bool* failed /*= nullptr*/)
	{
		auto rv = H5Tequal(m_hID, (hid_t)other_dtype);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv > 0;
	}

	H5T_conv_t Datatype::FindConversion(const Datatype& other_dtype, H5T_cdata_t** pcdata)
	{
		return H5Tfind(m_hID, (hid_t)other_dtype, pcdata);
	}

	bool Datatype::Flush()
	{
		return H5Tflush(m_hID) >= 0;
	}

	bool Datatype::GetClass(ClassType& t)
	{
		auto rv = H5Tget_class(m_hID);
		if (rv >= 0) {
			t = (ClassType)rv;
		}
		return rv >= 0;
	}

	DatatypeCreationPropertyList Datatype::GetCreationPropertyList()
	{
		return DatatypeCreationPropertyList(H5Tget_create_plist(m_hID));
	}

	Datatype Datatype::GetNativeType(Direction direction)
	{
		return Datatype(H5Tget_native_type(m_hID, (H5T_direction_t)direction));
	}

	int Datatype::GetOffset()
	{
		return H5Tget_offset(m_hID);
	}

	bool Datatype::SetOffset(size_t offset)
	{
		return H5Tset_offset(m_hID, offset) >= 0;
	}

	Datatype::ByteOrder Datatype::GetByteOrder()
	{
		return (ByteOrder)H5Tget_order(m_hID);
	}

	bool Datatype::SetByteOrder(ByteOrder bo)
	{
		return H5Tset_order(m_hID, (H5T_order_t)bo) >= 0;
	}

	bool Datatype::GetBitPadding(PaddingType& lsb, PaddingType& msb)
	{
		return H5Tget_pad(m_hID, (H5T_pad_t*)&lsb, (H5T_pad_t*)&msb) >= 0;
	}

	bool Datatype::SetBitPadding(PaddingType lsb, PaddingType msb)
	{
		return H5Tset_pad(m_hID, (H5T_pad_t)lsb, (H5T_pad_t)msb) >= 0;
	}

	size_t Datatype::GetPrecision() const
	{
		return H5Tget_precision(m_hID);
	}

	bool Datatype::SetPrecision(size_t p)
	{
		return H5Tset_precision(m_hID, p) >= 0;
	}

	size_t Datatype::GetSize()
	{
		return H5Tget_size(m_hID);
	}

	bool Datatype::SetSize(size_t size)
	{
		return H5Tset_size(m_hID, size) >= 0;
	}

	Datatype Datatype::GetSuper()
	{
		return Datatype(H5Tget_super(m_hID));
	}

	bool Datatype::Lock()
	{
		return H5Tlock(m_hID) >= 0;
	}

	bool Datatype::Refresh()
	{
		return H5Trefresh(m_hID) >= 0;
	}

	bool Datatype::RegisterConversion(ConversionType t, const char* name, const Datatype& other_dtype, H5T_conv_t func)
	{
		return H5Tregister((H5T_pers_t)t, name, m_hID, (hid_t)other_dtype, func) >= 0;
	}

	bool Datatype::UnregisterConversion(ConversionType t, const char* name, const Datatype& other_dtype, H5T_conv_t func)
	{
		return H5Tunregister((H5T_pers_t)t, name, m_hID, (hid_t)other_dtype, func) >= 0;
	}

	Datatype& Datatype::operator=(const Datatype& rhs)
	{
		if (this != &rhs) {
			H5Tclose(m_hID);
			m_hID = H5Tcopy(rhs.m_hID);
		}
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////

	ArrayDatatype::ArrayDatatype(const ArrayDatatype& rhs) : Datatype(H5Tcopy(rhs.m_hID))
	{

	}


#ifdef _DEBUG
	ArrayDatatype::ArrayDatatype(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5Tget_class(hid) == H5T_ARRAY) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a ArrayDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::ArrayDatatype");
#endif
				}
			}
			else {
				assert(false && L"input hid_t is not a ArrayDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::ArrayDatatype");
#endif
			}
		}
	}
#else
	ArrayDatatype::ArrayDatatype(hid_t hid) : Datatype(hid)
	{

	}
#endif

	ArrayDatatype::ArrayDatatype(const Datatype& base_dtype, unsigned int rank, const hsize_t dims[/*rank*/])
		: Datatype(H5Tarray_create2((hid_t)base_dtype, rank, dims))
	{

	}

	ArrayDatatype::ArrayDatatype(const Datatype& base_dtype, const std::vector<hsize_t>& dims)
		: Datatype(H5Tarray_create2((hid_t)base_dtype, (unsigned int)dims.size(), dims.data()))
	{

	}

	ArrayDatatype::ArrayDatatype(const Datatype& base_dtype)
	{
		m_hID = H5Tvlen_create((hid_t)base_dtype);
	}

	ArrayDatatype::~ArrayDatatype()
	{

	}

	bool ArrayDatatype::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5T_ARRAY == H5Tget_class(hid)) {
					H5Tclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Tclose(m_hID);
			m_hID = InvalidHandle;
			return true;
		}
	}

	bool ArrayDatatype::GetDimensions(std::vector<hsize_t>& dims)
	{
		auto d = GetDimensionsCount();
		if (d >= 0) {
			dims.resize(d);
			return H5Tget_array_dims2(m_hID, dims.data()) >= 0;
		}
		return false;
	}

	int ArrayDatatype::GetDimensionsCount()
	{
		return H5Tget_array_ndims(m_hID);
	}

	ArrayDatatype& ArrayDatatype::operator=(const ArrayDatatype& rhs)
	{
		if (this != &rhs) {
			H5Tclose(m_hID);
			m_hID = H5Tcopy(rhs.m_hID);
		}
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////

	OpaqueDatatype::OpaqueDatatype(size_t size)
	{
		m_hID = H5Tcreate(H5T_OPAQUE, size);
	}

	OpaqueDatatype::OpaqueDatatype(const OpaqueDatatype& rhs)
		: Datatype(H5Tcopy(rhs.m_hID))
	{
	}

	OpaqueDatatype::~OpaqueDatatype()
	{

	}

	OpaqueDatatype& OpaqueDatatype::operator=(const OpaqueDatatype& rhs)
	{
		if (this != &rhs) {
			H5Tclose(m_hID);
			m_hID = H5Tcopy(rhs.m_hID);
		}
		return *this;
	}

	bool OpaqueDatatype::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5T_OPAQUE == H5Tget_class(hid)) {
					H5Tclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Tclose(m_hID);
			m_hID = InvalidHandle;
			return true;
		}
	}

	std::string OpaqueDatatype::GetTag()
	{
		std::string s;
		auto rv = H5Tget_tag(m_hID);
		if (rv != nullptr) {
			s = rv;
		}
		return s;
	}

	bool OpaqueDatatype::SetTag(const char* tag)
	{
		return H5Tset_tag(m_hID, tag) >= 0;
	}

#ifdef _DEBUG
	OpaqueDatatype::OpaqueDatatype(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5Tget_class(hid) == H5T_OPAQUE) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a OpaqueDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::OpaqueDatatype");
#endif
				}
			}
			else {
				assert(false && L"input hid_t is not a OpaqueDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::OpaqueDatatype");
#endif
			}
		}
	}
#else
	OpaqueDatatype::OpaqueDatatype(hid_t hid)
		: Datatype(hid)
	{

	}
#endif

	//////////////////////////////////////////////////////////////////////////

	EnumerationDatatype::EnumerationDatatype(size_t size)
	{
		m_hID = H5Tcreate(H5T_ENUM, size);
	}

	EnumerationDatatype::EnumerationDatatype(const EnumerationDatatype& rhs)
	{
		m_hID = H5Tcopy(rhs.m_hID);
	}

	EnumerationDatatype::EnumerationDatatype(const Datatype& base_dtype)
	{
		m_hID = H5Tenum_create((hid_t)base_dtype);
	}

	EnumerationDatatype::~EnumerationDatatype()
	{

	}

	EnumerationDatatype& EnumerationDatatype::operator=(const EnumerationDatatype& rhs)
	{
		if (this != &rhs) {
			H5Tclose(m_hID);
			m_hID = H5Tcopy(rhs.m_hID);
		}
		return *this;
	}

	bool EnumerationDatatype::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5T_ENUM == H5Tget_class(hid)) {
					H5Tclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Tclose(m_hID);
			m_hID = InvalidHandle;
			return true;
		}
	}

	bool EnumerationDatatype::Insert(const char* name, const void* val)
	{
		return H5Tenum_insert(m_hID, name, val) >= 0;
	}

	bool EnumerationDatatype::GetNameOf(const void* val, char* name, size_t size)
	{
		return H5Tenum_nameof(m_hID, val, name, size) >= 0;
	}

	bool EnumerationDatatype::GetValueOf(const char* name, void* val)
	{
		return H5Tenum_valueof(m_hID, name, val) >= 0;
	}

	bool EnumerationDatatype::GetMemberValue(unsigned int member_no, void* val)
	{
		return H5Tget_member_value(m_hID, member_no, val) >= 0;
	}

	int EnumerationDatatype::GetMembersCount()
	{
		return H5Tget_nmembers(m_hID);
	}

#ifdef _DEBUG
	EnumerationDatatype::EnumerationDatatype(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5Tget_class(hid) == H5T_ENUM) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a EnumerationDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::EnumerationDatatype");
#endif
				}
			}
		}
		else {
			assert(false && L"input hid_t is not a EnumerationDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
			throw std::invalid_argument("input is not convertable to HDF5::EnumerationDatatype");
#endif
		}
	}
#else
	EnumerationDatatype::EnumerationDatatype(hid_t hid)
		: Datatype(hid)
	{

	}
#endif

	//////////////////////////////////////////////////////////////////////////

	CompoundDatatype::CompoundDatatype(size_t size)
	{
		m_hID = H5Tcreate(H5T_COMPOUND, size);
	}

	CompoundDatatype::CompoundDatatype(const CompoundDatatype& rhs)
	{
		m_hID = H5Tcopy(rhs.m_hID);
	}

	CompoundDatatype::~CompoundDatatype()
	{

	}

	CompoundDatatype& CompoundDatatype::operator=(const CompoundDatatype& rhs)
	{
		if (this != &rhs) {
			H5Tclose(m_hID);
			m_hID = H5Tcopy(rhs.m_hID);
		}
		return *this;
	}

	bool CompoundDatatype::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5T_COMPOUND == H5Tget_class(hid)) {
					H5Tclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Tclose(m_hID);
			m_hID = InvalidHandle;
			return true;
		}
	}

	Datatype::ClassType CompoundDatatype::GetMemberClass(unsigned int member_index)
	{
		return (ClassType)H5Tget_member_class(m_hID, member_index);
	}

	int CompoundDatatype::GetMemberIndex(const char* member_name)
	{
		return H5Tget_member_index(m_hID, member_name);
	}

	std::string CompoundDatatype::GetMemberName(unsigned int member_index)
	{
		std::string rv;
		auto mn = H5Tget_member_name(m_hID, member_index);
		if (mn != nullptr) {
			rv = mn;
			H5free_memory(mn);
		}
		return rv;
	}

	size_t CompoundDatatype::GetMemberOffset(unsigned int member_index)
	{
		return H5Tget_member_offset(m_hID, member_index);
	}

	Datatype CompoundDatatype::GetMemberType(unsigned int member_index)
	{
		return Datatype(H5Tget_member_type(m_hID, member_index));
	}

	int CompoundDatatype::GetMembersCount()
	{
		return H5Tget_nmembers(m_hID);
	}

	bool CompoundDatatype::Insert(const char* name, size_t offset, const Datatype& member_dtype)
	{
		return H5Tinsert(m_hID, name, offset, (hid_t)member_dtype) >= 0;
	}

	bool CompoundDatatype::Pack()
	{
		return H5Tpack(m_hID) >= 0;
	}

#ifdef _DEBUG
	CompoundDatatype::CompoundDatatype(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5Tget_class(hid) == H5T_COMPOUND) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a CompoundDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::CompoundDatatype");
#endif
				}
			}
		}
		else {
			assert(false && L"input hid_t is not a CompoundDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
			throw std::invalid_argument("input is not convertable to HDF5::CompoundDatatype");
#endif
		}
	}
#else
	CompoundDatatype::CompoundDatatype(hid_t hid)
		: Datatype(hid)
	{

	}
#endif

	//////////////////////////////////////////////////////////////////////////

	StringDatatype::StringDatatype(size_t size /*= H5T_VARIABLE*/)
	{
		m_hID = H5Tcreate(H5T_STRING, size);
	}

	StringDatatype::StringDatatype(const StringDatatype& rhs)
	{
		m_hID = H5Tcopy(rhs.m_hID);
	}

	StringDatatype::StringDatatype(const StringPDT& rhs)
	{
		m_hID = H5Tcopy((hid_t)rhs);
	}

	StringDatatype::~StringDatatype()
	{

	}

	StringDatatype& StringDatatype::operator=(const StringDatatype& rhs)
	{
		if (this != &rhs) {
			H5Tclose(m_hID);
			m_hID = H5Tcopy(rhs.m_hID);
		}
		return *this;
	}

	StringDatatype& StringDatatype::operator=(const StringPDT& rhs)
	{
		H5Tclose(m_hID);
		m_hID = H5Tcopy((hid_t)rhs);
		return *this;
	}

	bool StringDatatype::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5T_STRING == H5Tget_class(hid)) {
					H5Tclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Tclose(m_hID);
			m_hID = InvalidHandle;
			return true;
		}
	}

	HDF5::StringDatatype::CharSet StringDatatype::GetCharSet()
	{
		return (CharSet)H5Tget_cset(m_hID);
	}

	bool StringDatatype::SetCharSet(CharSet cs)
	{
		return H5Tset_cset(m_hID, (H5T_cset_t)cs) >= 0;
	}

	StringDatatype::StringPaddingType StringDatatype::GetStringPaddingType()
	{
		return (StringPaddingType)H5Tget_strpad(m_hID);
	}

	bool StringDatatype::SetStringPaddingType(StringPaddingType pt)
	{
		return H5Tset_strpad(m_hID, (H5T_str_t)pt) >= 0;
	}

	bool StringDatatype::IsVariableLength(bool* failed /* = nullptr */)
	{
		auto rv = H5Tis_variable_str(m_hID);
		if (failed != nullptr) {
			*failed = rv < 0;
		}
		return rv > 0;
	}

#ifdef _DEBUG
	StringDatatype::StringDatatype(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5Tget_class(hid) == H5T_STRING) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a StringDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::StringDatatype");
#endif
				}
			}
		}
		else {
			assert(false && L"input hid_t is not a StringDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
			throw std::invalid_argument("input is not convertable to HDF5::StringDatatype");
#endif
		}
	}
#else
	StringDatatype::StringDatatype(hid_t hid)
		: Datatype(hid)
	{

	}
#endif

	//////////////////////////////////////////////////////////////////////////

	FloatDatatype::FloatDatatype(const FloatDatatype& rhs)
	{
		m_hID = H5Tcopy(rhs.m_hID);
	}

	FloatDatatype::FloatDatatype(const FloatPDT& rhs)
	{
		m_hID = H5Tcopy((hid_t)rhs);
	}

	FloatDatatype::~FloatDatatype()
	{

	}

	FloatDatatype& FloatDatatype::operator=(const FloatDatatype& rhs)
	{
		if (this != &rhs) {
			H5Tclose(m_hID);
			m_hID = H5Tcopy(rhs.m_hID);
		}
		return *this;
	}

	FloatDatatype& FloatDatatype::operator=(const FloatPDT& rhs)
	{
		H5Tclose(m_hID);
		m_hID = H5Tcopy((hid_t)rhs);
		return *this;
	}

	bool FloatDatatype::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5T_FLOAT == H5Tget_class(hid)) {
					H5Tclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Tclose(m_hID);
			m_hID = InvalidHandle;
			return true;
		}
	}

	size_t FloatDatatype::GetExponentBias()
	{
		return H5Tget_ebias(m_hID);
	}

	bool FloatDatatype::SetExponentBias(size_t ebias)
	{
		return H5Tset_ebias(m_hID, ebias) >= 0;
	}

	bool FloatDatatype::GetBitFields(size_t* sign_pos, size_t* exp_pos, size_t* exp_bits, size_t* matissa_pos, size_t* mantissa_bits)
	{
		return H5Tget_fields(m_hID, sign_pos, exp_pos, exp_bits, matissa_pos, mantissa_bits) >= 0;
	}

	bool FloatDatatype::SetBitFields(size_t sign_pos, size_t exp_pos, size_t exp_bits, size_t matissa_pos, size_t mantissa_bits)
	{
		return H5Tset_fields(m_hID, sign_pos, exp_pos, exp_bits, matissa_pos, mantissa_bits) >= 0;
	}

	FloatDatatype::PaddingType FloatDatatype::GetPaddingType()
	{
		return (PaddingType)H5Tget_inpad(m_hID);
	}

	bool FloatDatatype::SetPaddingType(PaddingType pt)
	{
		return H5Tset_inpad(m_hID, (H5T_pad_t)pt) >= 0;
	}

	FloatDatatype::MantissaNormalization FloatDatatype::GetMantissaNormalization()
	{
		return (MantissaNormalization)H5Tget_norm(m_hID);
	}

	bool FloatDatatype::SetMantissaNormalization(MantissaNormalization mn)
	{
		return H5Tset_norm(m_hID, (H5T_norm_t)mn) >= 0;
	}

#ifdef _DEBUG
	FloatDatatype::FloatDatatype(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5Tget_class(hid) == H5T_FLOAT) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a FloatDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::FloatDatatype");
#endif
				}
			}
		}
		else {
			assert(false && L"input hid_t is not a FloatDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
			throw std::invalid_argument("input is not convertable to HDF5::FloatDatatype");
#endif
		}
	}
#else
	FloatDatatype::FloatDatatype(hid_t hid)
		: Datatype(hid)
	{

	}
#endif

	//////////////////////////////////////////////////////////////////////////

	IntegerDatatype::IntegerDatatype(const IntegerDatatype& rhs)
	{
		m_hID = H5Tcopy(rhs.m_hID);
	}

	IntegerDatatype::IntegerDatatype(const IntegerPDT& rhs)
	{
		m_hID = H5Tcopy((hid_t)rhs);
	}

	IntegerDatatype::~IntegerDatatype()
	{

	}

	IntegerDatatype& IntegerDatatype::operator=(const IntegerDatatype& rhs)
	{
		if (this != &rhs) {
			H5Tclose(m_hID);
			m_hID = H5Tcopy(rhs.m_hID);
		}
		return *this;
	}

	IntegerDatatype& IntegerDatatype::operator=(const IntegerPDT& rhs)
	{
		H5Tclose(m_hID);
		m_hID = H5Tcopy((hid_t)rhs);
		return *this;
	}

	bool IntegerDatatype::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5T_INTEGER == H5Tget_class(hid)) {
					H5Tclose(m_hID);
					m_hID = hid;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			H5Tclose(m_hID);
			m_hID = InvalidHandle;
			return true;
		}
	}

	IntegerDatatype::SignType IntegerDatatype::GetSignType()
	{
		return (SignType)H5Tget_sign(m_hID);
	}

	bool IntegerDatatype::SetSignType(SignType st)
	{
		return H5Tset_sign(m_hID, (H5T_sign_t)st) >= 0;
	}

#ifdef _DEBUG
	IntegerDatatype::IntegerDatatype(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_DATATYPE == H5Iget_type(hid)) {
				if (H5Tget_class(hid) == H5T_INTEGER) {
					m_hID = hid;
				}
				else {
					assert(false && L"input hid_t is not a IntegerDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
					throw std::invalid_argument("input is not convertable to HDF5::IntegerDatatype");
#endif
				}
			}
		}
		else {
			assert(false && L"input hid_t is not a IntegerDatatype");
#ifdef HDF5PP_USE_EXCEPTIONS
			throw std::invalid_argument("input is not convertable to HDF5::IntegerDatatype");
#endif
		}
	}
#else
	IntegerDatatype::IntegerDatatype(hid_t hid)
		: Datatype(hid)
	{

	}
#endif


	PredefinedDatatype::~PredefinedDatatype()
	{
		// don't close!
		m_hID = InvalidHandle;
	}

	PredefinedDatatype::PredefinedDatatype(hid_t hid)
	{
		assert(H5Iget_type(hid) == H5I_DATATYPE);
		m_hID = hid;
	}

	IntegerPDT::IntegerPDT(hid_t hid) : PredefinedDatatype(hid)
	{

	}

	const IntegerPDT IntegerPDT::Std_I8BE(H5T_STD_I8BE);
	const IntegerPDT IntegerPDT::Std_I8LE(H5T_STD_I8LE);
	const IntegerPDT IntegerPDT::Std_I16BE(H5T_STD_I16BE);
	const IntegerPDT IntegerPDT::Std_I16LE(H5T_STD_I16LE);
	const IntegerPDT IntegerPDT::Std_I32BE(H5T_STD_I32BE);
	const IntegerPDT IntegerPDT::Std_I32LE(H5T_STD_I32LE);
	const IntegerPDT IntegerPDT::Std_I64BE(H5T_STD_I64BE);
	const IntegerPDT IntegerPDT::Std_I64LE(H5T_STD_I64LE);

	const IntegerPDT IntegerPDT::Std_U8BE(H5T_STD_U8BE);
	const IntegerPDT IntegerPDT::Std_U8LE(H5T_STD_U8LE);
	const IntegerPDT IntegerPDT::Std_U16BE(H5T_STD_U16BE);
	const IntegerPDT IntegerPDT::Std_U16LE(H5T_STD_U16LE);
	const IntegerPDT IntegerPDT::Std_U32BE(H5T_STD_U32BE);
	const IntegerPDT IntegerPDT::Std_U32LE(H5T_STD_U32LE);
	const IntegerPDT IntegerPDT::Std_U64BE(H5T_STD_U64BE);
	const IntegerPDT IntegerPDT::Std_U64LE(H5T_STD_U64LE);

	const IntegerPDT IntegerPDT::Std_B8BE(H5T_STD_B8BE);
	const IntegerPDT IntegerPDT::Std_B8LE(H5T_STD_B8LE);
	const IntegerPDT IntegerPDT::Std_B16BE(H5T_STD_B16BE);
	const IntegerPDT IntegerPDT::Std_B16LE(H5T_STD_B16LE);
	const IntegerPDT IntegerPDT::Std_B32BE(H5T_STD_B32BE);
	const IntegerPDT IntegerPDT::Std_B32LE(H5T_STD_B32LE);
	const IntegerPDT IntegerPDT::Std_B64BE(H5T_STD_B64BE);
	const IntegerPDT IntegerPDT::Std_B64LE(H5T_STD_B64LE);

	// Intel specific
	const IntegerPDT IntegerPDT::Intel_I8(H5T_INTEL_I8);
	const IntegerPDT IntegerPDT::Intel_I16(H5T_INTEL_I16);
	const IntegerPDT IntegerPDT::Intel_I32(H5T_INTEL_I32);
	const IntegerPDT IntegerPDT::Intel_I64(H5T_INTEL_I64);

	const IntegerPDT IntegerPDT::Intel_U8(H5T_INTEL_U8);
	const IntegerPDT IntegerPDT::Intel_U16(H5T_INTEL_U16);
	const IntegerPDT IntegerPDT::Intel_U32(H5T_INTEL_U32);
	const IntegerPDT IntegerPDT::Intel_U64(H5T_INTEL_U64);

	const IntegerPDT IntegerPDT::Intel_B8(H5T_INTEL_B8);
	const IntegerPDT IntegerPDT::Intel_B16(H5T_INTEL_B16);
	const IntegerPDT IntegerPDT::Intel_B32(H5T_INTEL_B32);
	const IntegerPDT IntegerPDT::Intel_B64(H5T_INTEL_B64);

	// native types
	const IntegerPDT IntegerPDT::Native_CHAR(H5T_NATIVE_CHAR);
	const IntegerPDT IntegerPDT::Native_SCHAR(H5T_NATIVE_SCHAR);
	const IntegerPDT IntegerPDT::Native_UCHAR(H5T_NATIVE_UCHAR);

	const IntegerPDT IntegerPDT::Native_SHORT(H5T_NATIVE_SHORT);
	const IntegerPDT IntegerPDT::Native_USHORT(H5T_NATIVE_USHORT);

	const IntegerPDT IntegerPDT::Native_INT(H5T_NATIVE_INT);
	const IntegerPDT IntegerPDT::Native_UINT(H5T_NATIVE_UINT);

	const IntegerPDT IntegerPDT::Native_LONG(H5T_NATIVE_LONG);
	const IntegerPDT IntegerPDT::Native_ULONG(H5T_NATIVE_ULONG);

	const IntegerPDT IntegerPDT::Native_LLONG(H5T_NATIVE_LLONG);
	const IntegerPDT IntegerPDT::Native_ULLONG(H5T_NATIVE_ULLONG);

	const IntegerPDT IntegerPDT::Native_B8(H5T_NATIVE_B8);
	const IntegerPDT IntegerPDT::Native_B16(H5T_NATIVE_B16);
	const IntegerPDT IntegerPDT::Native_B32(H5T_NATIVE_B32);
	const IntegerPDT IntegerPDT::Native_B64(H5T_NATIVE_B64);

	// ANSI C9x native types
	const IntegerPDT IntegerPDT::Native_INT8(H5T_NATIVE_INT8);
	const IntegerPDT IntegerPDT::Native_UINT8(H5T_NATIVE_UINT8);
	const IntegerPDT IntegerPDT::Native_INT_LEAST8(H5T_NATIVE_INT_LEAST8);
	const IntegerPDT IntegerPDT::Native_UINT_LEAST8(H5T_NATIVE_UINT_LEAST8);
	const IntegerPDT IntegerPDT::Native_INT_FAST8(H5T_NATIVE_INT_FAST8);
	const IntegerPDT IntegerPDT::Native_UINT_FAST8(H5T_NATIVE_UINT_FAST8);

	const IntegerPDT IntegerPDT::Native_INT16(H5T_NATIVE_INT16);
	const IntegerPDT IntegerPDT::Native_UINT16(H5T_NATIVE_UINT16);
	const IntegerPDT IntegerPDT::Native_INT_LEAST16(H5T_NATIVE_INT_LEAST16);
	const IntegerPDT IntegerPDT::Native_UINT_LEAST16(H5T_NATIVE_UINT_LEAST16);
	const IntegerPDT IntegerPDT::Native_INT_FAST16(H5T_NATIVE_INT_FAST16);
	const IntegerPDT IntegerPDT::Native_UINT_FAST16(H5T_NATIVE_UINT_FAST16);

	const IntegerPDT IntegerPDT::Native_INT32(H5T_NATIVE_INT32);
	const IntegerPDT IntegerPDT::Native_UINT32(H5T_NATIVE_UINT32);
	const IntegerPDT IntegerPDT::Native_INT_LEAST32(H5T_NATIVE_INT_LEAST32);
	const IntegerPDT IntegerPDT::Native_UINT_LEAST32(H5T_NATIVE_UINT_LEAST32);
	const IntegerPDT IntegerPDT::Native_INT_FAST32(H5T_NATIVE_INT_FAST32);
	const IntegerPDT IntegerPDT::Native_UINT_FAST32(H5T_NATIVE_UINT_FAST32);

	const IntegerPDT IntegerPDT::Native_INT64(H5T_NATIVE_INT64);
	const IntegerPDT IntegerPDT::Native_UINT64(H5T_NATIVE_UINT64);
	const IntegerPDT IntegerPDT::Native_INT_LEAST64(H5T_NATIVE_INT_LEAST64);
	const IntegerPDT IntegerPDT::Native_UINT_LEAST64(H5T_NATIVE_UINT_LEAST64);
	const IntegerPDT IntegerPDT::Native_INT_FAST64(H5T_NATIVE_INT_FAST64);
	const IntegerPDT IntegerPDT::Native_UINT_FAST64(H5T_NATIVE_UINT_FAST64);

	FloatPDT::FloatPDT(hid_t hid) : PredefinedDatatype(hid)
	{

	}

	const FloatPDT FloatPDT::IEEE_F32BE(H5T_IEEE_F32BE);
	const FloatPDT FloatPDT::IEEE_F32LE(H5T_IEEE_F32LE);
	const FloatPDT FloatPDT::IEEE_F64BE(H5T_IEEE_F64BE);
	const FloatPDT FloatPDT::IEEE_F64LE(H5T_IEEE_F64LE);

	// Intel types
	const FloatPDT FloatPDT::Intel_F32(H5T_INTEL_F32);
	const FloatPDT FloatPDT::Intel_F64(H5T_INTEL_F64);

	// Native types
	const FloatPDT FloatPDT::Native_FLOAT(H5T_NATIVE_FLOAT);
	const FloatPDT FloatPDT::Native_DOUBLE(H5T_NATIVE_DOUBLE);
	const FloatPDT FloatPDT::Native_LDOUBLE(H5T_NATIVE_LDOUBLE);


	StringPDT::StringPDT(hid_t hid) : PredefinedDatatype(hid)
	{

	}

	const HDF5::StringPDT StringPDT::C_S1(H5T_C_S1);
	const HDF5::StringPDT StringPDT::Fortran_S1(H5T_FORTRAN_S1);



}
