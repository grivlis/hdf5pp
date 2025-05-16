// hdf5pp_error.h
// classes for handling errors in the HDF5 library
// HDF5::ErrorMessage	represents an error message
// HDF5::ErrorStack		represents a stack of error messages
// HDF5::ErrorClass		used to defined custom error message types

#pragma once

#include "hdf5pp_handle.h"

namespace HDF5 {

	class HDF5PP_API ErrorMessage;
	class HDF5PP_API ErrorStack;
	class HDF5PP_API ErrorClass;

	class HDF5PP_API ErrorStack : public Handle
	{
	public:
		using TraversalFuncType = H5E_auto2_t;	//  'herr_t func(hid_t estack, void * client_data)'
		using WalkFuncType = H5E_walk2_t;		//	'herr_t func(unsigned n, const H5E_error2_t * err_desc, void * client_data)'

		// Returns a copy of the current error stack
		static ErrorStack GetCurrent();

		// Returns the default stack
		static ErrorStack GetDefault();

		ErrorStack();
		ErrorStack(const ErrorStack& rhs);
		virtual ~ErrorStack();
		ErrorStack& operator=(const ErrorStack& rhs);

		bool Attach(hid_t hid) override;

		// Determines type of error stack
		bool AutoIsV2(bool* failed = nullptr);

		// Clears the stack
		bool Clear();

		// Close the stack
		bool CLose();

		// Get the settings for the automatic error stack traversal function and its data
		bool GetAuto(TraversalFuncType* func, void** client_data);

		// Retrieves the number of error messages on this stack
		ssize_t GetMessagesCount();

		// Deletes specified number of error messages from this stack
		bool Pop(size_t count);

		// Prints the error stack in a default manner
		// will print on stderr if fp == nullptr
		bool Print(FILE* fp = nullptr);
		// #TODO: implement C++ ostream print
		// bool Print(std::ostream & os);
		// bool Print(std::wostream & os);

		// Pushes a new error record onto the stack
		bool Push(const char* filename, const char* funcname, unsigned int line, const ErrorClass& error_class, const ErrorMessage& maj_msg, const ErrorMessage& min_msg, const char* mesg, ...);

		// Turn automatic error printing on or off
		bool SetAuto(TraversalFuncType func, void* client_data);

		// Replaces the current error stack with the contents of this stack
		// and then closes this stack
		bool SetAsCurrent();

		enum class WalkDirection {
			Upward = H5E_WALK_UPWARD,
			Downward = H5E_WALK_DOWNWARD
		};
		bool Walk(WalkDirection dir, WalkFuncType func, void* client_data);
	protected:
		explicit ErrorStack(hid_t hid);
	};



	class HDF5PP_API ErrorMessage : public Handle
	{
	public:
		enum class Type {
			Major = H5E_MAJOR,
			Minor = H5E_MINOR
		};

		ErrorMessage() = default;
		ErrorMessage(const ErrorMessage& rhs);
		virtual ~ErrorMessage();
		ErrorMessage& operator=(const ErrorMessage& rhs);

		bool Attach(hid_t hid) override;

		// Closes this message
		bool Close();

		// Retrieves the message info
		bool Get(Type& t, std::string& mesg);

	protected:
		explicit ErrorMessage(hid_t hid);
		friend class ErrorClass;
	};

	class HDF5PP_API ErrorClass : public Handle
	{
	public:
		ErrorClass() = default;
		ErrorClass(const ErrorClass& rhs);
		ErrorClass(const char* cls_name, const char* lib_name, const char* version);

		// note that destroying the ErrorClass will unregister it and also
		// remove all of its messages
		virtual ~ErrorClass();
		ErrorClass& operator=(const ErrorClass& rhs);

		bool Attach(hid_t hid) override;

		// Creates and Adds an ErrorMessage to this class
		ErrorMessage CreateMessage(ErrorMessage::Type t, const char* mesg);

		// Retrieves this class name
		bool GetName(std::string& name);

	protected:
		explicit ErrorClass(hid_t hid);
	};

}
