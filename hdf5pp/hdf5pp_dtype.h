#pragma once

#include "hdf5pp_attrobj.h"
#include "hdf5pp_proplist.h"

namespace HDF5 {

	class HDF5PP_API Datatype : public AttributedObject
	{
	public:
		Datatype(const Datatype& rhs);

		// Decode a binary object description of Datatype
		// question: how to upcast decoded Datatype?
		Datatype(unsigned char* buf);

		virtual ~Datatype();
		Datatype& operator=(const Datatype& rhs);

		bool Attach(hid_t hid) override;

		// Determines whether the Datatype is a committed type or transient type
		bool IsCommitted(bool* failed = nullptr);

		// Check whether the library's default conversion is hard conversion
		bool IsCompilerConversion(const Datatype& dst_dtype, bool* failed = nullptr);

		// Converts data from this Datatype to another
		bool Convert(const Datatype& dst_dtype, size_t num_elements, void* buf, void* background, const PropertyList& xpl = PropertyList());

		// Returns a copy of this Datatype with a new ID
		Datatype Copy();

		enum ClassType {
			Integer = H5T_INTEGER,
			Float = H5T_FLOAT,
			String = H5T_STRING,
			Bitfield = H5T_BITFIELD,
			Opaque = H5T_OPAQUE,
			Compound = H5T_COMPOUND,
			Reference = H5T_REFERENCE,
			Enumeration = H5T_ENUM,
			VariableLengthArray = H5T_VLEN,
			Array = H5T_ARRAY
		};
		// Determines whether the Datatype contains any Datatypes of the given ClassType
		bool DetectClass(ClassType t, bool* failed = nullptr);

		// Encode this Datatype object description into a binary buffer
		bool Encode(unsigned char* buf, size_t& buf_size);
		bool Encode(std::vector<unsigned char>& buf);

		// Determines if Datatypes are the same
		bool Equals(const Datatype& other_dtype, bool* failed = nullptr);

		// Finds a conversion function to other Datatype
		H5T_conv_t FindConversion(const Datatype& other_dtype, H5T_cdata_t** pcdata);

		// Flushes all the buffers associated with a committed datatype to disk
		bool Flush();

		// Retrieves the Datatype class
		bool GetClass(ClassType& t);

		// Returns a copy of the Datatype creation property list
		DatatypeCreationPropertyList GetCreationPropertyList();

		enum class Direction {
			Ascend = H5T_DIR_ASCEND,
			Descend = H5T_DIR_DESCEND
		};
		// Returns a copy of the Datatype will all types converted into native types, recursively
		Datatype GetNativeType(Direction direction);

		// Retrieves the bit offset of the first significant bit
		int GetOffset();

		// Sets the bit offset of the first significant bit
		bool SetOffset(size_t offset);

		enum class ByteOrder {
			Error = H5T_ORDER_ERROR,
			LittleEndian = H5T_ORDER_LE,
			BigEndian = H5T_ORDER_BE,
			VAX = H5T_ORDER_VAX,
			Mixed = H5T_ORDER_MIXED,
			None = H5T_ORDER_NONE
		};
		// Returns the byte order of an atomic Datatype
		ByteOrder GetByteOrder();

		// Sets the byte order
		bool SetByteOrder(ByteOrder bo);

		enum class PaddingType {
			Error = H5T_PAD_ERROR,
			Zeros = H5T_PAD_ZERO,
			Ones = H5T_PAD_ONE,
			Background = H5T_PAD_BACKGROUND
		};
		// Retrieves the padding type for the least- and most-significant bit padding
		bool GetBitPadding(PaddingType& lsb, PaddingType& msb);

		// Sets the padding type for lsb and msb
		bool SetBitPadding(PaddingType lsb, PaddingType msb);

		// Returns the precision of an atomic type
		size_t GetPrecision() const;

		// Sets the precision
		bool SetPrecision(size_t p);


		// Returns the size of the Datatype
		size_t GetSize();

		// Sets the size
		bool SetSize(size_t size);

		// Returns the base Datatype from which this type is derived
		Datatype GetSuper();

		// Lock the Datatype
		bool Lock();

		// Refreshes all the buffers associated with a committed Datatype
		bool Refresh();

		enum class ConversionType {
			Hard = H5T_PERS_HARD,
			Soft = H5T_PERS_SOFT
		};
		// Register a conversion function to another Datatype
		bool RegisterConversion(ConversionType t, const char* name, const Datatype& other_dtype, H5T_conv_t func);

		// Unregister a conversion function
		bool UnregisterConversion(ConversionType t, const char* name, const Datatype& other_dtype, H5T_conv_t func);
	protected:
		Datatype();
		explicit Datatype(hid_t hid);
		friend class Attribute;
		friend class Dataset;
		friend class CompoundDatatype;
		friend class Location;
	};

	class HDF5PP_API PredefinedDatatype : public Datatype
	{
	public:
		PredefinedDatatype(const PredefinedDatatype& rhs) = delete;
		virtual ~PredefinedDatatype();
		PredefinedDatatype& operator=(const PredefinedDatatype& rhs) = delete;
	protected:
		explicit PredefinedDatatype(hid_t hid);
	};

	class HDF5PP_API IntegerPDT : public PredefinedDatatype
	{
	public:
		IntegerPDT(const IntegerPDT&) = delete;
		IntegerPDT& operator=(const IntegerPDT&) = delete;

		// standard types: 8, 16, 32, and 64 bits; signed, unsigned and bitfield; little and big endian
		static const IntegerPDT Std_I8BE;
		static const IntegerPDT Std_I8LE;
		static const IntegerPDT Std_I16BE;
		static const IntegerPDT Std_I16LE;
		static const IntegerPDT Std_I32BE;
		static const IntegerPDT Std_I32LE;
		static const IntegerPDT Std_I64BE;
		static const IntegerPDT Std_I64LE;
		static const IntegerPDT Std_U8BE;
		static const IntegerPDT Std_U8LE;
		static const IntegerPDT Std_U16BE;
		static const IntegerPDT Std_U16LE;
		static const IntegerPDT Std_U32BE;
		static const IntegerPDT Std_U32LE;
		static const IntegerPDT Std_U64BE;
		static const IntegerPDT Std_U64LE;
		static const IntegerPDT Std_B8BE;
		static const IntegerPDT Std_B8LE;
		static const IntegerPDT Std_B16BE;
		static const IntegerPDT Std_B16LE;
		static const IntegerPDT Std_B32BE;
		static const IntegerPDT Std_B32LE;
		static const IntegerPDT Std_B64BE;
		static const IntegerPDT Std_B64LE;

		// Intel specific
		static const IntegerPDT Intel_I8;
		static const IntegerPDT Intel_I16;
		static const IntegerPDT Intel_I32;
		static const IntegerPDT Intel_I64;

		static const IntegerPDT Intel_U8;
		static const IntegerPDT Intel_U16;
		static const IntegerPDT Intel_U32;
		static const IntegerPDT Intel_U64;

		static const IntegerPDT Intel_B8;
		static const IntegerPDT Intel_B16;
		static const IntegerPDT Intel_B32;
		static const IntegerPDT Intel_B64;

		// native types
		static const IntegerPDT Native_CHAR;
		static const IntegerPDT Native_SCHAR;
		static const IntegerPDT Native_UCHAR;

		static const IntegerPDT Native_SHORT;
		static const IntegerPDT Native_USHORT;

		static const IntegerPDT Native_INT;
		static const IntegerPDT Native_UINT;

		static const IntegerPDT Native_LONG;
		static const IntegerPDT Native_ULONG;

		static const IntegerPDT Native_LLONG;
		static const IntegerPDT Native_ULLONG;

		static const IntegerPDT Native_B8;
		static const IntegerPDT Native_B16;
		static const IntegerPDT Native_B32;
		static const IntegerPDT Native_B64;

		// ANSI C9x native types
		static const IntegerPDT Native_INT8;
		static const IntegerPDT Native_UINT8;
		static const IntegerPDT Native_INT_LEAST8;
		static const IntegerPDT Native_UINT_LEAST8;
		static const IntegerPDT Native_INT_FAST8;
		static const IntegerPDT Native_UINT_FAST8;

		static const IntegerPDT Native_INT16;
		static const IntegerPDT Native_UINT16;
		static const IntegerPDT Native_INT_LEAST16;
		static const IntegerPDT Native_UINT_LEAST16;
		static const IntegerPDT Native_INT_FAST16;
		static const IntegerPDT Native_UINT_FAST16;

		static const IntegerPDT Native_INT32;
		static const IntegerPDT Native_UINT32;
		static const IntegerPDT Native_INT_LEAST32;
		static const IntegerPDT Native_UINT_LEAST32;
		static const IntegerPDT Native_INT_FAST32;
		static const IntegerPDT Native_UINT_FAST32;

		static const IntegerPDT Native_INT64;
		static const IntegerPDT Native_UINT64;
		static const IntegerPDT Native_INT_LEAST64;
		static const IntegerPDT Native_UINT_LEAST64;
		static const IntegerPDT Native_INT_FAST64;
		static const IntegerPDT Native_UINT_FAST64;
	private:
		explicit IntegerPDT(hid_t hid);
	};

	class HDF5PP_API FloatPDT : public PredefinedDatatype
	{
	public:
		FloatPDT(const FloatPDT&) = delete;
		FloatPDT& operator=(const FloatPDT&) = delete;

		// standard types: 32 and 64 bits, little and big endian
		static const FloatPDT IEEE_F32BE;
		static const FloatPDT IEEE_F32LE;
		static const FloatPDT IEEE_F64BE;
		static const FloatPDT IEEE_F64LE;

		// Intel types
		static const FloatPDT Intel_F32;
		static const FloatPDT Intel_F64;

		// Native types
		static const FloatPDT Native_FLOAT;
		static const FloatPDT Native_DOUBLE;
		static const FloatPDT Native_LDOUBLE;
	private:
		explicit FloatPDT(hid_t hid);
	};

	class HDF5PP_API StringPDT : public PredefinedDatatype
	{
	public:
		StringPDT(const StringPDT&) = delete;
		StringPDT& operator=(const StringPDT&) = delete;

		static const StringPDT C_S1;
		static const StringPDT Fortran_S1;
	private:
		explicit StringPDT(hid_t hid);
	};

	class HDF5PP_API ArrayDatatype : public Datatype
	{
	public:
		ArrayDatatype(const ArrayDatatype& rhs);

		// Create an array Datatype
		ArrayDatatype(const Datatype& base_dtype, unsigned int rank, const hsize_t dims[/*rank*/]);
		ArrayDatatype(const Datatype& base_dtype, const std::vector<hsize_t>& dims);

		// Create a variable-length array
		ArrayDatatype(const Datatype& base_dtype);

		virtual ~ArrayDatatype();
		ArrayDatatype& operator=(const ArrayDatatype& rhs);

		bool Attach(hid_t hid) override;

		// Retrieves the sizes of the array dimensions
		bool GetDimensions(std::vector<hsize_t>& dims);

		// Returns the number of dimensions in the array (rank)
		int GetDimensionsCount();
	protected:
		explicit ArrayDatatype(hid_t hid);
	};

	class HDF5PP_API OpaqueDatatype : public Datatype
	{
	public:
		OpaqueDatatype(size_t size);
		OpaqueDatatype(const OpaqueDatatype& rhs);
		virtual ~OpaqueDatatype();
		OpaqueDatatype& operator=(const OpaqueDatatype& rhs);

		bool Attach(hid_t hid) override;

		// Returns the tag associated with this Datatype
		std::string GetTag();

		// Sets the tag
		bool SetTag(const char* tag);
	protected:
		explicit OpaqueDatatype(hid_t hid);
	};

	class HDF5PP_API EnumerationDatatype : public Datatype
	{
	public:
		// Create a new enumeration Datatype where the value is of the given size
		EnumerationDatatype(size_t size);

		// Create a new enumeration Datatype with the give base Datatype
		// note that the base Datatype needs to be a native integer type
		EnumerationDatatype(const Datatype& base_dtype);

		// Copy constructor
		EnumerationDatatype(const EnumerationDatatype& rhs);

		virtual ~EnumerationDatatype();
		EnumerationDatatype& operator=(const EnumerationDatatype& rhs);

		bool Attach(hid_t hid) override;

		// Insert a new member
		bool Insert(const char* name, const void* val);

		// Returns the name that corresponds to the specified member value of the enumeartion
		bool GetNameOf(const void* val, char* name, size_t size);

		// Returns the value corresponding to a specified member name
		bool GetValueOf(const char* name, void* val);

		// Retrieves the member value
		bool GetMemberValue(unsigned int member_no, void* val);

		// Returns the number of members
		int GetMembersCount();
	protected:
		explicit EnumerationDatatype(hid_t hid);
	};

	class HDF5PP_API CompoundDatatype : public Datatype
	{
	public:
		CompoundDatatype(size_t size);
		CompoundDatatype(const CompoundDatatype& rhs);
		virtual ~CompoundDatatype();
		CompoundDatatype& operator=(const CompoundDatatype& rhs);

		bool Attach(hid_t hid) override;

		// Returns the ClassType of a member
		ClassType GetMemberClass(unsigned int member_index);

		// Retrieves the index of a member
		int GetMemberIndex(const char* member_name);

		// Retrieves the name of a member
		std::string GetMemberName(unsigned int member_index);

		// Retrieves the offset of a member
		size_t GetMemberOffset(unsigned int member_index);

		// Retrieves a copy of the Datatype of a member
		Datatype GetMemberType(unsigned int member_index);

		// Returns the number of members
		int GetMembersCount();

		// Adds a new member
		bool Insert(const char* name, size_t offset, const Datatype& member_dtype);

		// Recursively remove padding from within the compound Datatype
		bool Pack();
	protected:
		explicit CompoundDatatype(hid_t hid);
	};

	class HDF5PP_API StringDatatype : public Datatype
	{
	public:
		StringDatatype(size_t size = H5T_VARIABLE);
		StringDatatype(const StringDatatype& rhs);
		StringDatatype(const StringPDT& rhs);
		virtual ~StringDatatype();
		StringDatatype& operator=(const StringDatatype& rhs);
		StringDatatype& operator=(const StringPDT& rhs);

		bool Attach(hid_t hid) override;

		enum class CharSet {
			Error = H5T_CSET_ERROR,
			ASCII = H5T_CSET_ASCII,
			UTF8 = H5T_CSET_UTF8
		};
		// Retrieves the character set type
		CharSet GetCharSet();

		// Set the character set type
		bool SetCharSet(CharSet cs);

		enum class StringPaddingType {
			Error = H5T_STR_ERROR,			// error
			NullTerm = H5T_STR_NULLTERM,	// terminate with NULL (like C)
			NullPad = H5T_STR_NULLPAD,		// pad with zeros
			SpacePad = H5T_STR_SPACEPAD		// pad with spaces
		};
		// Retrieves the type of padding used for the string Datatype
		StringPaddingType GetStringPaddingType();

		// Sets the type of padding
		bool SetStringPaddingType(StringPaddingType pt);

		// Determines whether the Datatype is a variable length string
		bool IsVariableLength(bool* failed = nullptr);
	protected:
		explicit StringDatatype(hid_t hid);
	};

	class HDF5PP_API FloatDatatype : public Datatype
	{
	public:
		FloatDatatype(const FloatDatatype& rhs);
		FloatDatatype(const FloatPDT& rhs);
		virtual ~FloatDatatype();
		FloatDatatype& operator=(const FloatDatatype& rhs);
		FloatDatatype& operator=(const FloatPDT& rhs);

		bool Attach(hid_t hid) override;

		// Gets the exponent bias
		size_t GetExponentBias();

		// Sets the exponent bias
		bool SetExponentBias(size_t ebias);

		// Gets bit field information
		bool GetBitFields(size_t* sign_pos, size_t* exp_pos, size_t* exp_bits, size_t* matissa_pos, size_t* mantissa_bits);

		// Sets bit field information
		bool SetBitFields(size_t sign_pos, size_t exp_pos, size_t exp_bits, size_t matissa_pos, size_t mantissa_bits);

		// Retrieves the internal padding type for unused bits
		PaddingType GetPaddingType();

		// Set the internal padding type
		bool SetPaddingType(PaddingType pt);

		enum class MantissaNormalization {
			Error = H5T_NORM_ERROR,
			Implied = H5T_NORM_IMPLIED,	// MSB of mantissa is not stored
			MSBSet = H5T_NORM_MSBSET,	// MSB of mantissa is always 1
			None = H5T_NORM_NONE		// Mantissa is not normalized
		};
		// returns the mantissa normalization
		MantissaNormalization GetMantissaNormalization();

		// Sets the mantissa normalization
		bool SetMantissaNormalization(MantissaNormalization mn);
	protected:
		explicit FloatDatatype(hid_t hid);
	};

	class HDF5PP_API IntegerDatatype : public Datatype
	{
	public:
		IntegerDatatype(const IntegerDatatype& rhs);
		IntegerDatatype(const IntegerPDT& rhs);
		virtual ~IntegerDatatype();
		IntegerDatatype& operator=(const IntegerDatatype& rhs);
		IntegerDatatype& operator=(const IntegerPDT& rhs);

		bool Attach(hid_t hid) override;

		enum class SignType {
			Error = H5T_SGN_ERROR,		// error
			Unsigned = H5T_SGN_NONE,	// unsigned integer
			Two = H5T_SGN_2,			// 2's complement signed integer 
		};
		// returns the sign type
		SignType GetSignType();

		// Sets the sign type
		bool SetSignType(SignType st);

	protected:
		explicit IntegerDatatype(hid_t hid);
	};

}
