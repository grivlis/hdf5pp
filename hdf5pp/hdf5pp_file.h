// hdf5pp_file.h
// class for HDF5 files

#pragma once

#include "hdf5pp_group.h"
#include "hdf5pp_proplist.h"

namespace HDF5 {

	class HDF5PP_API File : public Group
	{
	public:
		File() = default;
		File(const File& rhs);
		File(const char* name, unsigned int flags, const PropertyList& fcpl = PropertyList(), const PropertyList& fapl = PropertyList());
		virtual ~File();
		File& operator=(const File& rhs);

		bool Attach(hid_t hid) override;

		// cannot use these on a file
		bool DecrementObjectReferenceCount() override { assert(false);  return false; }
		bool IncrementObjectReferenceCount() override { assert(false);  return false; }
		virtual bool AreMDCFlushesDisabled(bool* /*failed*/ = nullptr) override { assert(false);  return false; }
		virtual bool DisableMDCFlushes() override { assert(false);  return false; }
		virtual bool EnableMDCFlushes() override { assert(false);  return false; }


		// Clears the external link open file cache
		bool ClearExternalLinkCache();

		// Creates a new file; Will close currently opened file, if any
		// flags: H5F_ACC_TRUNC or H5F_ACC_EXCL, H5F_ACC_DEBUG
		bool Create(const char* name, unsigned int flags, const PropertyList& fcpl = PropertyList(), const PropertyList& fapl = PropertyList());

		// Terminates access to an HDF5 file
		bool Close();

		// the static function File::Delete deletes an HDF5 file filename with a file 
		// access property list fapl_id.  The fapl_id should be configured with the 
		// same VOL connector or VFD that was used to open the file.
		// This API was introduced for use with the Virtual Object Layer(VOL). With 
		// the VOL, HDF5 "files" can map to arbitrary storage schemes such as object 
		// stores and relational database tables.The data created by these implementations
		// may be inconvenient for a user to remove without a detailed knowledge of the 
		// storage scheme. File::Delete gives VOL connector authors the ability to add 
		// connector - specific delete code to their connectors so that users can remove 
		// these "files" without detailed knowledge of the storage scheme.
		// For a VOL connector, File::Delete deletes the file in a way that makes sense 
		// for the specified VOL connector.
		// For the native HDF5 connector, HDF5 files will be deleted via the VFDs, each 
		// of which will have to be modified to delete the files it creates.
		// For all implementations, File::Delete will first check if the file is an HDF5 
		// file via H5F_IS_ACCESSIBLE. This is done to ensure that File::Delete cannot 
		// be used as an arbitrary file deletion call.
		static bool Delete(const char* filename, const PropertyList& fapl = PropertyList());

		// Returns a copy of the creation property list for the file
		FileCreationPropertyList GetCreationPropertyList();

		// Returns a copy of the access property list for the file
		FileAccessPropertyList GetAccessPropertyList();

		// Retrieves a file number for the file
		bool GetFileno(unsigned long& fnumber);

		// Retrieves the EOA
		bool GetEOA(haddr_t& eoa);

		// Retrieves a copy of the image of the (existing, opened) file
		ssize_t GetFileImageSize();
		ssize_t GetFileImage(void* buf, size_t buf_len);

		// Retrieves the size of the file
		// unlike GetFileImageSize, this retrieves the size of the entire file, including
		// user block, if any, HDF5 portion of the file, and any data that may have been
		// appended beyond HDF5 data
		bool GetFileSize(hsize_t& size);

		enum class MemoryAllocationType {
			Default = H5FD_MEM_DEFAULT,
			Superblock = H5FD_MEM_SUPER,
			BTree = H5FD_MEM_BTREE,
			RawData = H5FD_MEM_DRAW,
			GlobalHeap = H5FD_MEM_GHEAP,
			LocalHeap = H5FD_MEM_LHEAP,
			ObjectHeader = H5FD_MEM_OHDR
		};
		// Retrieves free-space section information for a file
		// Note that the documentation is wrong
		ssize_t GetFreeSections(MemoryAllocationType t, size_t nsects, H5F_sect_info_t& sect_info);

		// Returns the amount of free space
		hssize_t GetFileFreeSpace();

		// Retrieves the read/write or read-only status of the file
		bool GetIntent(unsigned int& intent);

		// Obtain current metadata cache configuration
		bool GetMDCConfig(H5AC_cache_config_t& cfg);

		// Attempt to configure the metadata cache
		bool SetMDCConfig(H5AC_cache_config_t& cfg);

		// Obtain the metadata cache hit rate
		bool GetMDCHitRate(double& rate);

		// Reset hit rate statistics counters
		bool ResetMDCHitRateStats();

		// Obtain information about a cache image if it exists
		bool GetMDCImageInfo(haddr_t& image_addr, hsize_t& image_len);

		// Gets the current metadata cache logging status
		bool GetMDCLoggingStatus(bool& is_enabled, bool& is_currently_logging);

		// Starts logging metadata cache events if logging was previous enabled
		bool StartMDCLogging();

		// Stops logging metadata cache events if logging was previous enabled and is ongoing
		bool StopMDCLogging();

		// Obtain current metadata cache size data
		bool GetMDCSize(size_t* max_size, size_t* min_clean_size, size_t* cur_size, int* cur_num_entries);

		// Retrieves the collection of read retries for metadata entries with checksum
		bool GetMetadataReadEntryInfo(H5F_retry_info_t& rinfo);

#ifdef H5_HAVE_PARALLEL
		// Retrieves the atomicity mode in use
		bool GetMPIAtomicity(bool& flag);

		// Sets the MPI atomicity mode
		bool SetMPIAtomicity(bool flag);
#endif

		// Returns the open objects handles of a given type for the file
		ssize_t GetObjectsCount(unsigned int types);

		// Returns the open objects handles of a given type for all open HDF5 files
		static ssize_t GetAllObjectsCount(unsigned int types);

		// Returns a list of open object handles for this file
		// Do we need to close the returned handles?
		ssize_t GetOpenObjects(unsigned int types, size_t max_objs, hid_t* obj_id_list);

		// Returns a list of open object handles for all opened hdf5 files
		// Do we need to close the returned handles?
		ssize_t GetAllOpenObjects(unsigned int types, size_t max_objs, hid_t* obj_id_list);

		struct BufferingStats {
			unsigned int accesses[2];
			unsigned int hits[2];
			unsigned int misses[2];
			unsigned int evictions[2];
			unsigned int bypasses[2];
		};
		// Retrieves statistics about page access when it is enabled
		bool GetPageBufferingStatus(BufferingStats& stats);

		// Resets the page buffering statistics
		bool ResetPageBufferingStats();

		// Returns pointer to the file handle from the virtual file driver
		bool GetVFDHandle(const PropertyList& fapl, void** file_handle);

		// Sets the EOA to the maximum of (EOA, EOF) + increment
		bool IncrementFileSize(hsize_t increment);

		// Determines whether a file is in the HDF5 format
		static bool IsHDF5(const char* name);

		// Checks if the file specified by filename can be opened with the file access specified in fapl
		// Enables files to be checked with a given file access property list, unlike 
		// IsHDF5, which only uses the default file driver when opening a file.
		// Returns a positive value if the specified file can be opened. Returns 0 if not.
		// Returns a negative value when the function fails or if a file does not exist.
		static htri_t IsAccessible(const char* name, const PropertyList& fapl = PropertyList());

		// Opens an existing HDF5 file; will close the currently opened file, if any
		// flags: H5F_ACC_RDWR, H5F_ACC_RDONLY, H5F_ACC_DEBUG
		bool Open(const char* name, unsigned int flags, const PropertyList& fapl = PropertyList());

		// Returns a new handle for a previously-opened file
		File Reopen();

		// Enable the switch of version bounds setting
		bool SetLibraryVersionBounds(H5F_libver_t low, H5F_libver_t high);

		// Enabled SWMR writing mode
		bool StartSWMRWrite();
	protected:
		explicit File(hid_t hid);
		friend class Location;
	};

}