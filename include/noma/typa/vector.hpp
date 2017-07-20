// Copyright (c) 2016 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_vector_hpp
#define noma_typa_vector_hpp

#include <cstring> // memcpy()
#include <fstream>
#include <iostream>

#include "debug.hpp"
#include "noma/typa/typa.hpp"

namespace noma {
namespace typa {

template<typename T>
class vector {
public:
	vector() = default;

	vector(size_t size) : size_(size)
	{
		allocate();
	}

	// copy
	vector(const vector& other)
		: size_(other.size_)
	{
		allocate();
		for (size_t i = 0; i < size_; ++i) {
			at(i) = other.at(i);
		}
	}

	// assignment
	vector& operator=(const vector& other)
	{
		deallocate();
		size_ = other.size_;
		allocate();
		for (size_t i = 0; i < size_; ++i) {
			at(i) = other.at(i);
		}

		return *this;
	}

	// move
	vector(vector&& other)
		: size_(other.size_)
	{
		data_ = other.data_; // move ownership
		other.data_ = nullptr; // invalidate
	}

	~vector()
	{
		deallocate();
	}

	T const * data() const { return data_; }
	T* data() { return data_; }
	size_t size() const { return size_; }

	T const& operator[](size_t i) const { return data_[i]; }
	T& operator[](size_t i) { return data_[i]; }

	T const& at(size_t i) const { return data_[i]; }
	T& at(size_t i) { return data_[i]; }

	void resize(size_t size)
	{
		size_ = size;
		deallocate();
		allocate();
	}

	void print(std::ostream& out) const
	{
		std::ostringstream oss;
		oss.precision(std::numeric_limits<double>::max_digits10);
		oss << std::scientific;

		oss << "{";
		for (size_t i = 0; i < size_; ++i) {
			oss << at(i);
			if (i < size_ - 1)
				oss << ',';
		}
		oss << "}";

		out << oss.str();
	}

	void scale(T factor)
	{
		for (size_t i = 0; i < size_; ++i)
			at(i) *= factor;
	}

	void scale_inverse(T factor)
	{
		for (size_t i = 0; i < size_; ++i)
			at(i) = (1.0 / at(i)) * factor;
	}

private:
	void allocate()
	{
		assert(data_ == nullptr);
		data_ = new T[size_];
	}

	void deallocate()
	{
		if (data_) {
			delete [] data_;
			data_ = nullptr;
		}
	}

	T* data_ = nullptr;
	size_t size_ = 0;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const vector<T>& v)
{
	v.print(out);
	return out;
}

// parser function
template<typename T>
std::istream& operator>>(std::istream& in, vector<T>& v)
{
	std::string line;
	std::getline(in, line);
	bool is_list = line.front() == '{'; // TODO: maybe do a smarter test here
	DEBUG_ONLY( std::cout << "Parsing vector using protocol: " << (is_list ? "list" : "file") << std::endl; )
	if (is_list)
	{
		std::string& list = line;
		DEBUG_ONLY( std::cout << "Parsing vector from list: " << list << std::endl; )
		std::vector<T> vec = noma::typa::parse_braced_list<T>(list);

		size_t size = vec.size();

		DEBUG_ONLY( std::cout << "Parsed vector size: " << size << std::endl; )
		// fill vector
		v.resize(size);
		//size_t size_byte = sizeof(T) * size;
		//std::memcpy(v.data(), vec.data(), size_byte); // NOTE: first argument is typed
		for (size_t i = 0; i < size; ++i) {
			v.at(i) = vec[i];
		}
		DEBUG_ONLY( std::cout << "Parsed vector from list: " << v << std::endl; )
	}
	else // handle as file name
	{
		std::string& filename = line;
		DEBUG_ONLY( std::cout << "Parsing vector from file: " << filename << std::endl; )
		std::ifstream fs(filename);
		if (fs.fail())
			throw parser_error("noma::typa::vector<T>::operator>>(): error: could not open file '" + filename + "'.");
		size_t size;
		fs >> size;
		v.resize(size);
		for (size_t i = 0; i < size; ++i) {
			fs >> v.at(i);
		}
	}

	return in;
}

} // namespace typa
} // namespace noma

#endif // parser_vector_hpp
