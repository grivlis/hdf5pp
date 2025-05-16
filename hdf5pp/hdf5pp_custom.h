// hdf5pp_custom.h
// HDF5::CustomHandle is used for custom indentifiers
// 
#pragma once

#include "hdf5pp_handle.h"

namespace HDF5 {

	class HDF5PP_API CustomHandle : public Handle
	{
	public:
		using CustomHandleType = H5I_type_t;
		using FreeFunctionType = H5I_free_t; // 'herr_t free_func(void *)'
		using SearchFunctionType = H5I_search_func_t; // 'int search_func(void * obj, hid_t id, void * key)'

		// Creates and returns a new CustomHandle type.
		static CustomHandleType RegisterType(size_t hash_size, unsigned int reserved, FreeFunctionType free_func);

		// Removes the CustomHandleType and all identifiers within that type.
		// Note that it's possible to render instances of CustomHandle invalid
		static bool UnregisterType(CustomHandleType type);

		// Deletes all identifiers of the given type
		// if force==false, only identifiers whose reference count is 1 will be deleted
		// Note that it's possible to render instances of CustomHandle invalid
		static bool ClearTypeMembers(CustomHandleType type, bool force = false);

		// Decrements the reference count on an CustomHandleType
		static int DecrementTypeReferenceCount(CustomHandleType type);

		// Increments the reference count on a CustomHandleType
		static int IncrementTypeReferenceCount(CustomHandleType type);

		// Retrieves the reference count on a CustomHandleType
		static int GetTypeReferenceCount(CustomHandleType type);

		// Returns the number of identifiers in a given CustomHandleType
		static bool GetTypeMembersCount(CustomHandleType type, uint64_t& num_members);

		// Finds the memory referred to by an identifier within the given CustomHandleType such that some criterion is satisfied
		static void* SearchTypeMembers(CustomHandleType type, SearchFunctionType func, void* key);

		// Determines whether a CustomHandleType is registered
		// if returned false, it's possible also that the function just failed
		// failed will return true in this, but only if not null
		static bool TypeExists(CustomHandleType type, bool* failed = nullptr);

		// Creates and returns a new CustomHandle
		CustomHandle(CustomHandleType type, void* object);

		CustomHandle(const CustomHandle& rhs);
		CustomHandle& operator=(const CustomHandle& rhs);

		// removes ID _AND_ delete the memory by applying delete
		~CustomHandle();

		// can only attach identifiers of the same CustomHandleType
		bool Attach(hid_t hid) override;

		// attach with type
		bool Attach(hid_t hid, CustomHandleType type);

		CustomHandleType GetObjectType() const { return m_type; }

		// Removes an ID from internal storage (H5I_REMOVE_VERIFY)
		// Returns the object (memory) referenced by this object (H5I_OBJECT_VERIFY)
		void* GetObjectMemory(bool remove = false);
	protected:
		CustomHandleType m_type{ (CustomHandleType)(-1) };
	};

}
