// hdf5pp_location.h
// HDF5::Location is the superclass for HDF5 objects that can identify a location within
// an HDF5 structure. Those are:
//	file, group, dataset, attribute, and committed datatype
//
#pragma once

#include "hdf5pp_handle.h"
#include "hdf5pp_proplist.h"

namespace HDF5 {

	// forward declarations
	class HDF5PP_API File;
	class HDF5PP_API Object;
	class HDF5PP_API Group;
	class HDF5PP_API Dataset;
	class HDF5PP_API Dataspace;
	class HDF5PP_API Datatype;
	class HDF5PP_API AttributedObject;

	class HDF5PP_API Location : public Handle
	{
	public:
		Location() = default;
		Location(const Location & rhs);
		virtual ~Location();
		Location& operator=(const Location & rhs);

		// take ownership of identifier and will be responsible for closing it
		virtual bool Attach(hid_t hid) override;

		// Get the file containing this object
		File GetFile();

		// Retrieves the name for this object
		bool GetName(std::string& name);

		// Determines whether a link resolve to an actual object
		bool ObjectExists(const char* link_name, const PropertyList& lapl = PropertyList(), bool* failed = nullptr);

		// Retrieves comment
		bool GetComment(std::string& comment);
		bool GetComment(const char* name, std::string& comment, const PropertyList& lapl = PropertyList());

		// Sets comment
		bool SetComment(const char* comment);
		bool SetComment(const char* name, const char* comment, const PropertyList& lapl = PropertyList());

		// Retrieves the metadata info for this object
		bool GetInfo(H5O_info_t& info, unsigned int fields);

		enum class IndexType {
			Unknown = H5_INDEX_UNKNOWN,				// unknown index
			ByName = H5_INDEX_NAME,					// index on names
			ByCreationOrder = H5_INDEX_CRT_ORDER	// index on creation order
		};

		enum class OrderType {
			Unknown = H5_ITER_UNKNOWN,				// unknown order
			Increasing = H5_ITER_INC,				// increasing order
			Decreasing = H5_ITER_DEC,				// decreasing order
			Native = H5_ITER_NATIVE					// no particular order, whichever is fastest
		};

		// Retrieves the metadata info for n-th object of specified group
		bool GetInfo(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, H5O_info_t& info, unsigned int fields, const PropertyList& lapl = PropertyList());

		// Retrieves the metadata info for the named object
		bool GetInfo(const char* name, H5O_info_t& info, unsigned int fields, const PropertyList& lapl = PropertyList());

		// Retrieves the native file format information for an object
		bool GetNativeInfo(H5O_native_info_t& oinfo, unsigned int fields);

		// Retrieves the native file format information for an object specified by loc_id, group name, group_name, the index by which objects in the group are tracked, idx_type, the order by which the index is to be traversed, order , and an object's position n within that index
		bool GetNativeInfo(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, H5O_native_info_t& oinfo, unsigned fields, const PropertyList& lapl = PropertyList());

		// Retrieves the native file format information for an object specified by loc_id and the name name
		bool GetNativeInfo(const char* name, H5O_native_info_t& oinfo, unsigned fields, const PropertyList& lapl = PropertyList());

		// Opens an object in HDF5 file with the given name
		Object OpenObject(const char* name, const PropertyList& lapl = PropertyList());

		// Opens an object by address within an HDF5 file
		// use extreme caution using this function! improper use can lead to file corruption
		Object OpenObject(haddr_t addr);

		// Opens an object specified by an object token, token
		Object OpenObject(H5O_token_t token);

		// Opens an object by index in a group in this location
		Object OpenObject(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, const PropertyList& lapl = PropertyList());

		// Recursively visits all objects accessible from this object
		bool VisitObjects(IndexType tIndex, OrderType tOrder, H5O_iterate_t op, void* op_data, unsigned int fields);

		// Recursively visits all objects accessible from this object staring with specified object
		bool VisitObjects(const char* object_name, IndexType tIndex, OrderType tOrder, H5O_iterate_t op, void* op_data, unsigned int fields, const PropertyList& lapl = PropertyList());

		// Compares two VOL connector object tokens. A comparison value, cmp_value, is returned, which indicates the result of the comparison.
		bool CompareTokens(const H5O_token_t& token1, const H5O_token_t& token2, int& cmp_value);

		// Deserializes a string, token_str, into a connector object token, token
		bool TokenFromString(const char* token_str, H5O_token_t& token);

		// Serializes a connector's object token into a string, token_str
		bool TokenToString(const H5O_token_t& token, char** token_str);

		// Creates a new, soft, link to an object in another file
		bool CreateExternalLink(const char* target_file_name, const char* target_object_name, const char* link_name, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());

		// Create a new hard link
		bool CreateHardLink(const Location& srcLoc, const char* srcName, const char* dst_link_name, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());

		// Creates a new soft link
		bool CreateSoftLink(const char* target_path, const char* link_name, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());

		// Create a link of user defined type
		bool CreateUserDefinedLink(const char* link_name, H5L_type_t link_type, const char* udata, size_t udata_size, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());
		bool CreateUserDefinedLink(const char* link_name, H5L_type_t link_type, const std::vector<char>& udata, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());

		// Determines whether a class of user-defined links is registered
		static bool IsUserDefinedLinkClassRegisterd(H5L_type_t link_cls_id, bool* failed = nullptr);

		// Registers a user-defined link class or changes behavior of an existing class
		static bool RegisterUserDefinedLinkClass(const H5L_class_t& link_class);

		// Unregisters a class of user-defined links
		static bool UnregisterUserDefinedLinkClass(H5L_type_t link_cls_id);

		// Removes a link from group
		// use extreme caution using this function! improper use can lead inaccessible objects in the file
		bool DeleteLink(const char* name, const PropertyList& lapl = PropertyList());

		// Remove the n-th link in a group
		// use extreme caution using this function! improper use can lead inaccessible objects in the file
		bool DeleteLink(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, const PropertyList& lapl = PropertyList());

		// Determines whether a link with the specified name exists in group
		bool LinkExists(const char* link_name, const PropertyList& lapl = PropertyList());

		// Returns information about the link
		bool GetLinkInfo(const char* link_name, H5L_info_t& info, const PropertyList& lapl = PropertyList());

		// Returns information about the n-th link in a group
		bool GetLinkInfo(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, H5L_info_t& info, const PropertyList& lapl = PropertyList());

		// Retrieves the name of the n-th link in a group
		bool GetLinkName(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, std::string& name, const PropertyList& lapl = PropertyList());

		// Retrieves the value of a symbolic link
		bool GetLinkValue(const char* link_name, void* linkval_buff, size_t size, const PropertyList& lapl = PropertyList());

		// Retrieves the value of the n-th symbolic link in group
		bool GetLinkValue(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t nOffset, void* linkval_buf, size_t size, const PropertyList& lapl = PropertyList());

		// Iterates through links in a group (see also Group::Iterate)
		bool Iterate(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t& idx, H5L_iterate2_t op, void* op_data, const PropertyList& lapl = PropertyList());

		// Recursively visits all links starting from a specified group (see also Group::Visit)
		bool Visit(const char* group_name, IndexType tIndex, OrderType tOrder, H5L_iterate2_t op, void* op_data, const PropertyList& lapl = PropertyList());

		// Decodes external link information
		static bool UnpackExternalLinkValue(char* ext_linkval, size_t link_size, unsigned int* flags, const char** filename, const char** obj_path);

		static bool CopyLink(const Location& srcLoc, const char* srcName, const Location& dstLoc, const char* dstName, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());
		static bool CreateHardLink(const Location& srcLoc, const char* srcName, const Location& dstLoc, const char* dst_link_name, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());
		static bool CreateSoftLink(const char* targetPath, const Location& linkLoc, const char* dst_link_name, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());
		static bool MoveLink(const Location& srcLoc, const char* src_name, const Location& dstLoc, const char* dest_name, const PropertyList& lcpl = PropertyList(), const PropertyList& lapl = PropertyList());

		// Create Group and link it into the file
		Group CreateGroup(const char* group_name, const PropertyList& lcpl = PropertyList(), const PropertyList& gcpl = PropertyList(), const PropertyList& gapl = PropertyList());

		// Create an anonymous group without linking into the file
		Group CreateGroup(const PropertyList& gcpl = PropertyList(), const PropertyList& gapl = PropertyList());

		// Get group info
		bool GetGroupInfo(H5G_info_t& ginfo);

		// Get group info for the specified group
		bool GetGroupInfo(const char* group_name, IndexType tIndex, OrderType tOrder, hsize_t n, H5G_info_t& group_info, const PropertyList& lapl = PropertyList());

		// Get group info for named group
		bool GetGroupInfo(const char* group_name, H5G_info_t& group_info, const PropertyList& lapl = PropertyList());

		// Open group
		Group OpenGroup(const char* group_name, const PropertyList& gapl = PropertyList());

		// Create Dataset and link it into the file
		Dataset CreateDataset(const char* dataset_name, const Datatype& dtype, const Dataspace& dspace, const PropertyList& lcpl = PropertyList(), const PropertyList& dcpl = PropertyList(), const PropertyList& dapl = PropertyList());

		// Create an anonymous Dataset without linking into the file
		Dataset CreateDataset(const Datatype& dtype, const Dataspace& dspace, const PropertyList& dcpl = PropertyList(), const PropertyList& dapl = PropertyList());

		// Open an existing Dataset
		Dataset OpenDataset(const char* name, const PropertyList& dapl = PropertyList());

		enum class FlushScope {
			Global = H5F_SCOPE_GLOBAL,
			Local = H5F_SCOPE_LOCAL
		};
		// Flushes all buffers associated with the file to disk
		bool FlushFile(FlushScope scope);

		// Returns global information for the file
		bool GetFileInfo(H5F_info2_t& finfo);


		// Get the file name to which the object belongs
		bool GetFileName(std::string& name);

		// Mounts a file
		bool MountFile(const char* name, const File& file, const PropertyList& fmpl = PropertyList());

		// Un-mounts a file
		bool UnmountFile(const char* name);

		// Commits a transient Datatype, linking it into the file and creating a new committed Datatype
		bool CommitDatatype(const char* name, const Datatype& dtype, const PropertyList& lcpl = PropertyList(), const PropertyList& tcpl = PropertyList(), const PropertyList& tapl = PropertyList());

		// Commits a transient Datatype to a file, creating a new committed Datatype, but does not link it into the file structure
		// If Object::LinkObject is not susequently used, the new Datatype will be delete from the file
		bool CommitDatatype(const Datatype& dtype, const PropertyList& tcpl = PropertyList(), const PropertyList& tapl = PropertyList());

		// Opens a committed Datatype
		Datatype OpenDatatype(const char* name, const PropertyList& tapl = PropertyList());


		// Creates an object reference
		bool CreateReference(void* ref, const char* name);

		// Creates a dataset region reference
		bool CreateReference(void* ref, const char* name, const Dataspace& dspace);

		// Open an object from reference
		AttributedObject DereferenceObject(void* ref, const PropertyList& oapl = PropertyList());

		// Open a dataset region from reference
		AttributedObject DereferenceDatasetRegion(void* ref, const PropertyList& oapl = PropertyList());

		// Retrieves the name for referenced object
		bool GetReferencedObjectName(void* ref, std::string& name);
		bool GetReferencedDatasetRegionName(void* ref, std::string& name);

		bool GetReferencedObjectType(void* ref, H5O_type_t& ot);
		bool GetReferencedDatasetRegionType(void* ref, H5O_type_t& ot);

		Dataspace GetReferencedDatasetRegion(void* ref);

		//////////////////////////////////////////////////////////////////////////
		// some shortcut methods for adding datasets

		// add a simple dataset; failed if it exists already
		bool AddDataset(const char* name, const std::vector<int8_t>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<uint8_t>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<int16_t>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<uint16_t>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<int32_t>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<uint32_t>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<int64_t>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<uint64_t>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<float>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<double>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<long>& vals, const std::vector<size_t>& dims = std::vector<size_t>());
		bool AddDataset(const char* name, const std::vector<unsigned long>& vals, const std::vector<size_t>& dims = std::vector<size_t>());

		bool AddDataset(const char* name, const int8_t* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const uint8_t* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const int16_t* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const uint16_t* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const int32_t* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const uint32_t* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const int64_t* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const uint64_t* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const float* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const double* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const long* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);
		bool AddDataset(const char* name, const unsigned long* vals, size_t dim1, size_t dim2 = 1, size_t dim3 = 1, size_t dim4 = 1);

		bool AddDataset(const char* name, const std::vector<std::string>& vStr);
		bool AddDataset(const char* name, const std::vector<const char *>& vStr);
	protected:
		explicit Location(hid_t hid);
	};
}