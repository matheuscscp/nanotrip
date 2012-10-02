
/**

Some structures that can be easily needed everywhere.

@file common.hpp
@author Matheus Costa de Sousa Carvalho Pimenta

*/

#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <sstream>

#define CALLBACK(ref,mfunc)	((ref).*(mfunc))

#define SHOW(X)	std::cout << #X << ": (" << (X) << ")" << std::endl

/// Namespace for some common classes and functions.
namespace common {

/// Evaluate function for strings.
template <typename T>
bool eval(const std::string& raw, T& buf);

/// Evaluate function to strings.
template <typename T>
std::string eval(T buf);

/// Class to manage the main function arguments.
class MainArgs {
private:
	static int argc;
	static char** argv;
public:
	static void init(int argc, char** argv);	///< Set the arguments only once.
	static int size();	///< Returns the amount of arguments passed.
	static int find(const std::string& what);	///< Find an argument.
	
	/// Access to an argument by the name of its predecessor.
	template <typename T> static T get(const std::string& what);
};

/// Inherited exception class from std::exception, to log errors into a ErrorLog.txt file.
class mexception : public std::exception {
private:
	std::string what_;
public:
	mexception(const std::string& what) throw();	///< Constructor to assign the description.
	virtual ~mexception() throw();	///< Empty destructor.
	const char* what() const throw();	///< Returns the description.
	void logerr() const throw();	///< Logs the exception into the ErrorLog.txt file.
};

/// Class to throw quit exceptions.
class Quit : public mexception {
public:
	// Empty constructor.
	Quit();
};

/// Auto pointer Class.
template <class T>
class aptr {
private:
	T* ptr;
public:
	aptr(T* ptr) : ptr(ptr) {}	///< Assignment constructor.
	~aptr() { if (ptr) delete ptr; }	///< Deleting destructor.
	T* operator()() { return ptr; }	///< Returns the value of the pointer.
	T* operator->() { return ptr; }	///< Access to member operator.
	T& operator*() { return *ptr; }	///< Dereference operator.
};

/// Auto pointer Class for arrays.
template <class T>
class arr_aptr {
private:
	T* ptr;
public:
	arr_aptr(T* ptr) : ptr(ptr) {}	///< Assignment constructor.
	~arr_aptr() { if (ptr) delete[] ptr; }	///< Deleting destructor.
	T* operator()() { return ptr; }	///< Returns the value of the pointer.
	T& operator[](int i) { return ptr[i]; }	///< Access to an element of the array.
};

/// Auto pointer Class for arrays.
class RootPath {
private:
	static bool isset;
	static std::string path;
public:
	static void init(const std::string& path_);
	static const std::string& get();
	static std::string get(const std::string& path_);
};

}	// namespace common end

// =============================================================================
// eval functions
// =============================================================================

template <typename T>
bool common::eval(const std::string& raw, T& buf) {
	if (!raw.size()) {
		buf = T();
		return false;
	}
	
	std::stringstream ss;
	ss << raw;
	ss >> buf;
	return ((!ss.fail()) && (ss.eof()));
}

template <typename T>
std::string common::eval(T buf) {
	std::stringstream ss;
	ss << buf;
	return ss.str();
}

// =============================================================================
// MainArgs class
// =============================================================================

template <typename T>
T common::MainArgs::get(const std::string& what) {
	int i = find(what);
	
	// if there is the argument and its successor
	if ((i) && (i + 1 < argc))
		return eval<T>(argv[i + 1]);
	
	return T();
}

#endif
