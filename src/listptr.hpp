
/**

@file listptr.hpp
@author Matheus Pimenta

*/

#ifndef LISTPTR_HPP
#define LISTPTR_HPP

#include <list>

/// Inherits C++ STL list to implementa automatic deletion for pointers and arrays.
template <class T, class Allocator = std::allocator<T> >
class listptr : public std::list<T*> {
private:
	bool array;
public:
	explicit listptr(bool array = false, const Allocator& my_allocator = Allocator()) : std::list<T*>(my_allocator), array(array) {}
	
	explicit listptr(unsigned int n, const T* value = new T(), bool array = false, const Allocator& my_allocator = Allocator()) : std::list<T*>(n, value, my_allocator), array(array) {}
	
	template <class InputIterator>
	listptr (InputIterator first, InputIterator last, bool array = false, const Allocator& my_allocator = Allocator()) : std::list<T*>(first, last, my_allocator), array(array) {}
	
	listptr (const std::list<T*, Allocator>& x, bool array = false) : std::list<T*>(x), array(array) {}
	
	template <class Predicate>
	void remove_if(Predicate pred) {
		for (typename std::list<T*>::iterator it = std::list<T*>::begin(); it != std::list<T*>::end(); ++it) {
			if (pred(*it)) {
				if (*it)
					delete *it;
			}
		}
		std::list<T*>remove_if(pred);
	}
	
	void remove(T* value) {
		if (value) {
			if (array)
				delete[] value;
			else
				delete value;
		}
		std::list<T*>::remove(value);
	}
	
	void pop_front() {
		if (!std::list<T*>::size())
			return;
		if (std::list<T*>::front()) {
			if (array)
				delete[] std::list<T*>::front();
			else
				delete std::list<T*>::front();
		}
		std::list<T*>::pop_front();
	}
	
	void pop_back() {
		if (!std::list<T*>::size())
			return;
		if (std::list<T*>::back()) {
			if (array)
				delete[] std::list<T*>::back();
			else
				delete std::list<T*>::back();
		}
		std::list<T*>::pop_back();
	}
	
	void clear() {
		while (std::list<T*>::size())
			pop_back();
	}
	
	typename std::list<T*>::iterator erase(typename std::list<T*>::iterator position) {
		if (*position) {
			if (array)
				delete[] *position;
			else
				delete *position;
		}
		return std::list<T*>::erase(position);
	}
	
	typename std::list<T*>::iterator erase(typename std::list<T*>::iterator first, typename std::list<T*>::iterator last) {
		for (typename std::list<T*>::iterator it = first; it != last; ++it) {
			if (*it) {
				if (array)
					delete[] *it;
				else
					delete *it;
			}
		}
		return std::list<T*>::erase(first, last);
	}
	
	~listptr() {
		clear();
	}
};

#endif
