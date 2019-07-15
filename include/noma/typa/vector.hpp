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

	vector(size_t size, T value = T()) : size_(size)
	{
		allocate();
		init(value);
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

	T const& at(size_t i) const
	{
		assert(i < size_);
		return data_[i];
	}

	T& at(size_t i)
	{
		assert(i < size_);
		return data_[i];
	}

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

	void init(T value) {
		for (size_t i = 0; i < size_; ++i) {
			at(i) = value;
		}
	}

	T* data_ = nullptr;
	size_t size_ = 0;
};

template<typename T>
struct type_to_regexp<vector<T>>
{
	static const std::string& exp_str();
};

template<typename T>
const std::string& type_to_regexp<vector<T>>::exp_str()
{
	static const std::string& value { make_braced_list(type_to_regexp<T>::exp_str()) };
	return value;
};

/**
 * This recursive specialisation allows arbitrary nesting of vector_wrapper.
 */
template<typename T>
struct string_to_value<vector<T>>
{
	static vector<T> parse(const std::string& input)
	{
		DEBUG_ONLY( std::cout << "Parsing vector from list: " << input << std::endl; )
		std::vector<T> vec = parse_braced_list<T>(input);
		DEBUG_ONLY( std::cout << "Parsed vector size: " << vec.size() << std::endl; )

		vector<T> result(vec.size());

		for (size_t i = 0; i < vec.size(); ++i)
			result.at(i) = vec[i];

		DEBUG_ONLY( std::cout << "Parsed vector from list: " << result << std::endl; )
		return result;
	}
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
		v = string_to_value<vector<T>>::parse(line);
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
