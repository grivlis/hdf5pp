// hdf5pp_proplist.h
// all classes for handling property lists
//
#pragma once


#include "hdf5pp_handle.h"

namespace HDF5 {

	class HDF5PP_API PropertyList : public Handle
	{
	public:
		PropertyList();
		PropertyList(const PropertyList& rhs);

		// Decode from binary buffer of encoded PropertyList
		PropertyList(void* buf);
		virtual ~PropertyList();
		PropertyList& operator=(const PropertyList& rhs);

		bool Attach(hid_t hid) override;

		// Returns a copy of this property list (also see operator=)
		PropertyList Copy();

		// Encodes the list's property values into a binary buffer
		size_t GetRequiredEncodeBufferSize(const PropertyList& fapl = PropertyList());
		bool Encode(void* buf, size_t& nalloc, const PropertyList & fapl = PropertyList());

		// Create a property list from an encoded property list in a buffer
		static PropertyList Decode(void* buf);

		//bool GetVolID(hid_t& vol_id);

	protected:
		explicit PropertyList(hid_t hid);
	};

	class HDF5PP_API AttributeCreationPropertyList : public PropertyList
	{
	public:
		AttributeCreationPropertyList();
		AttributeCreationPropertyList(const AttributeCreationPropertyList& rhs);
		virtual ~AttributeCreationPropertyList();

		bool Attach(hid_t hid) override;
	protected:
		explicit AttributeCreationPropertyList(hid_t hid);
		friend class Attribute;
	};

	class HDF5PP_API GroupCreationPropertyList : public PropertyList
	{
	public:
		GroupCreationPropertyList();
		GroupCreationPropertyList(const GroupCreationPropertyList& rhs);
		virtual ~GroupCreationPropertyList();

		bool Attach(hid_t hid) override;
	protected:
		explicit GroupCreationPropertyList(hid_t hid);
		friend class Group;
	};

	class HDF5PP_API DatasetCreationPropertyList : public PropertyList
	{
	public:
		DatasetCreationPropertyList();
		DatasetCreationPropertyList(const DatasetCreationPropertyList& rhs);
		virtual ~DatasetCreationPropertyList();

		bool Attach(hid_t hid) override;
	protected:
		explicit DatasetCreationPropertyList(hid_t hid);
		friend class Dataset;
	};

	class HDF5PP_API FileCreationPropertyList : public PropertyList
	{
	public:
		FileCreationPropertyList();
		FileCreationPropertyList(const FileCreationPropertyList& rhs);
		virtual ~FileCreationPropertyList();

		bool Attach(hid_t hid) override;

		// Sets/Gets user block size; may be any power of 2 >= 512
		bool SetUserblock(hsize_t size);
		bool GetUserblock(hsize_t& size);

		// Sets/Gets the byte sizes of offsets and lengths used to address objects in the file
		bool SetSizes(size_t sizeof_addr, size_t sizeof_size);
		bool GetSizes(size_t& sizeof_addr, size_t& sizeof_size);

		// Sets/Gets the size of parameters used to control the symbol table nodes
		bool SetSymK(unsigned int ik, unsigned int lk);
		bool GetSymK(unsigned int& ik, unsigned int& lk);

		// Sets/Gets the size of the parameter used to control the B-trees for indexing chunked Datasets
		bool SetIstoreK(unsigned int ik);
		bool GetIstoreK(unsigned int& ik);

		// Sets/Gets the file space page size for a file creation property list
		bool SetFileSpacePageSize(hsize_t fsp_size);
		bool GetFileSpacePageSize(hsize_t& fsp_size);

		// Sets/Gets the file space handling strategy and persisting free-space values for a file creation property list
		enum class FileSpaceStrategy {
			FreeSpaceManagerAggregator = H5F_FSPACE_STRATEGY_FSM_AGGR,	// Mechanisms: free-space managers, aggregators, and virtual file drivers; This is the library default when not set */
			EmbededPageAggregator = H5F_FSPACE_STRATEGY_PAGE, // Mechanisms: free-space managers with embedded paged aggregation and virtual file drivers
			Aggregator = H5F_FSPACE_STRATEGY_AGGR, // Mechanisms: aggregators and virtual file drivers
			None = H5F_FSPACE_STRATEGY_NONE, // Mechanisms: virtual file drivers
		};
		bool SetFileSpaceStrategy(FileSpaceStrategy strategy, bool persist, hsize_t threshold);
		bool GetFileSpaceStrategy(FileSpaceStrategy& strategy, bool& persist, hsize_t& threshold);

		// Sets/Gets number of shared object header message indexes
		bool SetSharedMesgIndexesCount(unsigned int nindexes);
		bool GetSharedMesgIndexesCount(unsigned int& nindexes);

		// Configures the specified shared object header message index
		bool SetSharedMesgIndex(unsigned int index_num, unsigned int mesg_type_flags, unsigned int min_mesg_size);
		// Retrieves the configuration settings for a shared message index
		bool GetSharedMesgIndex(unsigned int index_num, unsigned int& mesg_type_flags, unsigned int& min_mesg_size);

		// Sets/Gets shared object header message storage phase change thresholds
		bool SetSharedMesgPhaseChange(unsigned int max_list, unsigned int min_btree);
		bool GetSharedMesgPhaseChange(unsigned int& max_list, unsigned int& min_btree);
	protected:
		explicit FileCreationPropertyList(hid_t hid);
		friend class File;
	};

	class HDF5PP_API FileAccessPropertyList : public PropertyList
	{
	public:
		FileAccessPropertyList();
		FileAccessPropertyList(const FileAccessPropertyList& rhs);
		virtual ~FileAccessPropertyList();

		bool Attach(hid_t hid) override;


	protected:
		explicit FileAccessPropertyList(hid_t hid);
		friend class File;
	};

	class HDF5PP_API DatasetAccessPropertyList : public PropertyList
	{
	public:
		DatasetAccessPropertyList();
		DatasetAccessPropertyList(const DatasetAccessPropertyList& rhs);
		virtual ~DatasetAccessPropertyList();

		bool Attach(hid_t hid) override;
	protected:
		explicit DatasetAccessPropertyList(hid_t hid);
		friend class Dataset;
	};

	class HDF5PP_API DatatypeCreationPropertyList : public PropertyList
	{
	public:
		DatatypeCreationPropertyList();
		DatatypeCreationPropertyList(const DatatypeCreationPropertyList& rhs);
		virtual ~DatatypeCreationPropertyList();

		bool Attach(hid_t hid) override;
	protected:
		explicit DatatypeCreationPropertyList(hid_t hid);
		friend class Datatype;
	};
}
