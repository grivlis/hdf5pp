#include "pch.h"
#include "hdf5pp_error.h"

namespace HDF5 {

	ErrorStack ErrorStack::GetCurrent()
	{
		return ErrorStack(H5Eget_current_stack());
	}

	ErrorStack ErrorStack::GetDefault()
	{
		return ErrorStack(H5E_DEFAULT);
	}

	ErrorStack::ErrorStack(const ErrorStack& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

	ErrorStack::ErrorStack()
	{
		m_hID = H5Ecreate_stack();
		if (m_hID < 0) {
			m_hID = InvalidHandle;
		}
	}

	ErrorStack::~ErrorStack()
	{
		if (m_hID >= 0) {
			H5Eclose_stack(m_hID);
			m_hID = InvalidHandle;
		}
	}

	ErrorStack& ErrorStack::operator=(const ErrorStack& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool ErrorStack::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_ERROR_STACK == H5Iget_type(hid)) {
				DecrementReferenceCount();
				m_hID = hid;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			DecrementReferenceCount();
			m_hID = InvalidHandle;
			return true;
		}
	}

	bool ErrorStack::AutoIsV2(bool* failed /*= nullptr*/)
	{
		bool ret{ false };
		unsigned int v{ 99 };
		auto rr = H5Eauto_is_v2(m_hID, &v);
		if (rr >= 0) {
			ret = v == 1;
		}
		if (failed != nullptr) {
			*failed = rr < 0;
		}
		return ret;
	}

	bool ErrorStack::Clear()
	{
		return H5Eclear2(m_hID) >= 0;
	}

	bool ErrorStack::CLose()
	{
		auto retval = H5Eclose_stack(m_hID);
		m_hID = InvalidHandle;
		return retval >= 0;
	}

	bool ErrorStack::GetAuto(TraversalFuncType* func, void** client_data)
	{
		return H5Eget_auto2(m_hID, func, client_data) >= 0;
	}

	ssize_t ErrorStack::GetMessagesCount()
	{
		return H5Eget_num(m_hID);
	}

	bool ErrorStack::Pop(size_t count)
	{
		return H5Epop(m_hID, count) >= 0;
	}

	bool ErrorStack::Print(FILE* fp)
	{
		return H5Eprint2(m_hID, fp) >= 0;
	}

	bool ErrorStack::Push(const char* filename, const char* funcname, unsigned int line, const ErrorClass& error_class, const ErrorMessage& maj_msg, const ErrorMessage& min_msg, const char* mesg, ...)
	{
		va_list args;
		va_start(args, mesg);
		auto needed = vsnprintf(nullptr, 0, mesg, args);
		auto pmsg = new char[needed + 10];
		vsnprintf(pmsg, needed + 10, mesg, args);
		va_end(args);
		auto retval = H5Epush2(m_hID, filename, funcname, line, (hid_t)error_class, (hid_t)maj_msg, (hid_t)min_msg, pmsg);
		delete pmsg;
		return retval;
	}

	bool ErrorStack::SetAuto(TraversalFuncType func, void* client_data)
	{
		return H5Eset_auto2(m_hID, func, client_data);
	}

	bool ErrorStack::SetAsCurrent()
	{
		auto rv = H5Eset_current_stack(m_hID);
		m_hID = InvalidHandle;
		return rv >= 0;
	}

	bool ErrorStack::Walk(WalkDirection dir, WalkFuncType func, void* client_data)
	{
		return H5Ewalk2(m_hID, (H5E_direction_t)dir, func, client_data) >= 0;
	}

#ifdef _DEBUG
	ErrorStack::ErrorStack(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_ERROR_STACK == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not an ErrorStack");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::ErrorStack");
#endif
			}
		}
	}
#else
	ErrorStack::ErrorStack(hid_t hid) : Handle(hid)
	{

	}
#endif

	ErrorMessage::ErrorMessage(const ErrorMessage& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

	ErrorMessage::~ErrorMessage()
	{
		if (m_hID >= 0) {
			H5Eclose_msg(m_hID);
			m_hID = InvalidHandle;
		}
	}

	ErrorMessage& ErrorMessage::operator=(const ErrorMessage& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool ErrorMessage::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_ERROR_MSG == H5Iget_type(hid)) {
				DecrementReferenceCount();
				m_hID = hid;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			DecrementReferenceCount();
			m_hID = InvalidHandle;
			return true;
		}
	}

	bool ErrorMessage::Close()
	{
		auto retval = H5Eclose_msg(m_hID);
		m_hID = InvalidHandle;
		return retval >= 0;
	}

	bool ErrorMessage::Get(Type& t, std::string& mesg)
	{
		H5E_type_t tt;
		auto s = H5Eget_msg(m_hID, &tt, nullptr, 0); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Eget_msg(m_hID, &tt, nm, s + 1);
			if (s2 == s) {
				mesg = nm;
			}
			delete[] nm;
			t = (Type)tt;
			return s2 > 0;
		}
		else if (s == 0) {
			mesg.clear();
			t = (Type)tt;
			return true;
		}
		return false;
	}

#ifdef _DEBUG
	ErrorMessage::ErrorMessage(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_ERROR_MSG == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not an ErrorMessage");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::ErrorMessage");
#endif
			}
		}
	}
#else
	ErrorMessage::ErrorMessage(hid_t hid) : Handle(hid)
	{

	}
#endif

	ErrorClass::ErrorClass(const ErrorClass& rhs)
	{
		m_hID = rhs.m_hID;
		IncrementReferenceCount();
	}

	ErrorClass::ErrorClass(const char* cls_name, const char* lib_name, const char* version)
	{
		m_hID = H5Eregister_class(cls_name, lib_name, version);
		if (m_hID < 0) {
			m_hID = InvalidHandle;
		}
	}

	ErrorClass::~ErrorClass()
	{
		H5Eunregister_class(m_hID);
		m_hID = InvalidHandle;
	}

	HDF5::ErrorMessage ErrorClass::CreateMessage(ErrorMessage::Type t, const char* mesg)
	{
		return ErrorMessage(H5Ecreate_msg(m_hID, (H5E_type_t)t, mesg));
	}

	bool ErrorClass::GetName(std::string& name)
	{
		auto s = H5Eget_class_name(m_hID, nullptr, 0); // get length
		if (s > 0) {
			auto nm = new char[s + 1];
			auto s2 = H5Eget_class_name(m_hID, nm, s + 1);
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

	ErrorClass& ErrorClass::operator=(const ErrorClass& rhs)
	{
		if (this != &rhs) {
			DecrementReferenceCount();
			m_hID = rhs.m_hID;
			IncrementReferenceCount();
		}
		return *this;
	}

	bool ErrorClass::Attach(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_ERROR_CLASS == H5Iget_type(hid)) {
				DecrementReferenceCount();
				m_hID = hid;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			DecrementReferenceCount();
			m_hID = InvalidHandle;
			return true;
		}
	}

#ifdef _DEBUG
	ErrorClass::ErrorClass(hid_t hid)
	{
		if (hid >= 0) {
			if (H5I_ERROR_CLASS == H5Iget_type(hid)) {
				m_hID = hid;
			}
			else {
				assert(false && L"input hid_t is not an ErrorClass");
#ifdef HDF5PP_USE_EXCEPTIONS
				throw std::invalid_argument("input is not convertable to HDF5::ErrorClass");
#endif
			}
		}
	}
#else
	ErrorClass::ErrorClass(hid_t hid) : Handle(hid)
	{

	}
#endif

}
