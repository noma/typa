// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_vector_wrapper_hpp
#define noma_typa_vector_wrapper_hpp

#include <iostream>
#include <string>
#include <vector>

#include "noma/typa/typa.hpp"
#include "noma/typa/wrapper.hpp"

namespace noma {
namespace typa {

template<typename T>
using vector_wrapper = wrapper<std::vector<T>>;

template<typename T>
struct type_to_regexp<vector_wrapper<T>>
{
	static const std::string& exp_str();
};

template<typename T>
const std::string& type_to_regexp<vector_wrapper<T>>::exp_str()
{
	static const std::string& value { make_braced_list(type_to_regexp<T>::exp_str()) };
	return value;
};

/**
 * This recursive specialisation allows arbitrary nesting of vector_wrapper.
 */
template<typename T>
struct string_to_value<vector_wrapper<T>>
{
	static vector_wrapper<T> parse(const std::string& input)
	{
		return parse_braced_list<T>(input);
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const vector_wrapper<T>& vec)
{
	//std::cout << "printing vector_wrapper" << std::endl;
	const auto vec_size = vec.get().size();
	out << '{';
	for (size_t i = 0; i < vec_size; ++i) {
		out << vec.get()[i];
		if (i < (vec_size - 1))
			out << ", ";
	}
	out << '}';
	return out;
}

template<typename T>
std::istream& operator>>(std::istream& in, vector_wrapper<T>& vec)
{
	//std::cout << "parsing vector_wrapper" << std::endl;
	std::string value;
	std::getline(in, value);

	vec.get() = parse_braced_list<T>(value);

	return in;
}

} // namespace typa
} // namespace noma

#endif // noma_typa_vector_wrapper_hpp
