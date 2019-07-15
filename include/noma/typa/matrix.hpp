// Copyright (c) 2016 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_matrix_hpp
#define noma_typa_matrix_hpp

#include <cstring> // memcpy()
#include <fstream>
#include <iostream>

#include "debug.hpp"
#include "noma/typa/typa.hpp"

namespace noma {
namespace typa {

template<typename T>
class matrix {
public:
	matrix() = default;

	matrix(size_t rows, size_t cols, T value = T()) : rows_(rows), cols_(cols)
	{
		allocate();
		init(value);
	}

	// copy
	matrix(const matrix& other)
		: rows_(other.rows_), cols_(other.cols_)
	{
		allocate();
		for (size_t i = 0; i < rows_; ++i) {
			for (size_t j = 0; j < cols_; ++j) {
				at(i, j) = other.at(i, j);
			}
		}
	}

	// assignment
	matrix& operator=(const matrix& other)
	{
		deallocate();
		rows_ = other.rows_;
		cols_ = other.cols_;
		allocate();
		for (size_t i = 0; i < rows_; ++i) {
			for (size_t j = 0; j < cols_; ++j) {
				at(i, j) = other.at(i, j);
			}
		}

		return *this;
	}

	// move
	matrix(matrix&& other)
		: rows_(other.rows_), cols_(other.cols_)
	{
		data_ = other.data_; // move ownership
		other.data_ = nullptr; // invalidate
	}

	~matrix()
	{
		deallocate();
	}

	//T* data() { return data_; }
	T const * data() const { return data_; }
	T* data() { return data_; }
	size_t rows() const { return rows_; }
	size_t cols() const { return cols_; }

	T const & at(size_t i, size_t j) const
	{
		assert(i < rows_ && j < cols_);
		return data_[i * cols_ + j];
	}

	T& at(size_t i, size_t j)
	{
		assert(i < rows_ && j < cols_);
		return data_[i * cols_ + j];
	}

	void resize(size_t rows, size_t cols)
	{
		rows_ = rows;
		cols_ = cols;
		deallocate();
		allocate();
	}

	matrix<T> transposed() const
	{
		matrix<T> result(rows_, cols_);
		for (size_t i = 0; i < rows_; ++i) {
			for (size_t j = 0; j < cols_; ++j) {
				result.at(j,i) = this->at(i,j);
			}
		}
		return result;
	}

	void print_raw(std::ostream& out, const char& delimiter = '\t') const
	{
		std::ostringstream oss;
		oss.precision(std::numeric_limits<double>::max_digits10);
		oss << std::scientific;

		for (size_t i = 0; i < rows_; ++i) {
			for (size_t j = 0; j < cols_; ++j) {
				oss << at(i,j)
				    << ((j == cols_ - 1) ? '\n' : delimiter);
			}
		}

		out << oss.str();
	}

	void print_flat(std::ostream& out) const
	{
		std::ostringstream oss;
		oss.precision(std::numeric_limits<double>::max_digits10);
		oss << std::scientific;

		oss << "{";
		for (size_t i = 0; i < rows_; ++i) {
			for (size_t j = 0; j < cols_; ++j) {
				oss << at(i,j);
				if ((i * cols_ + j) < (cols_ * rows_) - 1)
					oss << ',';
			}
		}
		oss << "}";

		out << oss.str();
	}

	void print(std::ostream& out) const
	{
		std::ostringstream oss;
		oss.precision(std::numeric_limits<double>::max_digits10);
		oss << std::scientific;

		oss << "{";
		for (size_t i = 0; i < rows_; ++i) {
			oss << "{";
			for (size_t j = 0; j < cols_; ++j) {
				oss << at(i,j);
				if (j < cols_ - 1)
					oss << ',';

			}
			oss << "}";
			if (i < rows_ - 1)
				oss << ',';
		}
		oss << "}";

		out << oss.str();
	}

	void scale(T factor)
	{
		for (size_t i = 0; i < rows_; ++i)
			for (size_t j = 0; j < cols_; ++j) {
				at(i,j) *= factor;
			}
	}

private:
	void allocate()
	{
		assert(data_ == nullptr);
		data_ = new T[rows_ * cols_];
	}

	void deallocate()
	{
		if (data_) {
			delete [] data_;
			data_ = nullptr;
		}
	}
	void init(T value) {
		for (size_t i = 0; i < rows_; ++i) {
			for (size_t j = 0; j < cols_; ++j) {
				at(i, j) = value;
			}
		}
	}

	T* data_ = nullptr;
	size_t rows_ = 0;
	size_t cols_ = 0;
};



template<typename T>
struct type_to_regexp<matrix<T>>
{
	static const std::string& exp_str();
};
template<typename T>
const std::string& type_to_regexp<matrix<T>>::exp_str()
{
	static const std::string& value { make_braced_list(make_braced_list(type_to_regexp<T>::exp_str())) };
	return value;
};

// output function
template<typename T>
std::ostream& operator<<(std::ostream& out, const matrix<T>& m)
{
	m.print(out);

	return out;
}

// parser/input function
template<typename T>
std::istream& operator>>(std::istream& in, matrix<T>& m)
{
	std::string line;
	std::getline(in, line);
	bool is_list = line.front() == '{'; // TODO: maybe do a smarter test here
	DEBUG_ONLY( std::cout << "Parsing matrix using protocol: " << (is_list ? "list" : "file") << std::endl; )
	if (is_list)
	{
		std::string& list = line;
		DEBUG_ONLY( std::cout << "Parsing matrix from list: " << list << std::endl; )
		std::vector<std::vector<T>> row_vec = noma::typa::parse_braced_list<std::vector<T>>(list);

		size_t rows = row_vec.size();
		size_t cols = row_vec[0].size();
		// check if all rows have equal size
		bool cols_equal = true;
		for (auto& row : row_vec)
			cols_equal = cols_equal && (row.size() == cols);

		if(!cols_equal)
			throw parser_error("noma::typa::matrix<T>::operator>>(): error: rows have differing lengths, check your input list.");
		DEBUG_ONLY( std::cout << "Parsed matrix size: " << rows << " x " << cols << std::endl; )
		// fill matrix
		m.resize(rows, cols);
		for (size_t i = 0; i < rows; ++i) {
			for (size_t j = 0; j < cols; ++j) {
				m.at(i, j) = row_vec[i][j];
			}
		}
		DEBUG_ONLY( std::cout << "Parsed matrix from list: " << m << std::endl; )
	}
	else // handle as file name
	{
		std::string& filename = line;
		DEBUG_ONLY( std::cout << "Parsing matrix from file: " << filename << std::endl; )
		std::ifstream fs(filename);
		if (fs.fail())
			throw parser_error("noma::typa::matrix<T>::operator>>(): error: could not open file '" + filename + "'.");
		size_t rows, cols;
		fs >> rows >> cols;
		m.resize(rows, cols);
		for (size_t i = 0; i < rows; ++i) {
			for (size_t j = 0; j < cols; ++j) {
				fs >> m.at(i, j);
			}
		}
	}

	return in;
}

} // namespace typa
} // namespace noma

#endif // noma_typa_matrix_hpp
