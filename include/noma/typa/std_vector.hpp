// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_std_vector_hpp
#define noma_typa_std_vector_hpp

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>

#include <boost/lexical_cast.hpp>

#include "noma/typa/parser_error.hpp"

namespace noma {
namespace typa {

// make std::vector streamable

// NOTE: std::vector does not work with boost program options, as it there is some special behaviour for
//       collecting arguments that can occur multiple times. This gets triggered when a the type of an option
//       happens to be std::vector, so we cannot do custom parsing for std::vector.

template<typename T>
struct type_to_regexp<std::vector<T>>
{
	static const std::string& exp_str();
};

template<typename T>
const std::string& type_to_regexp<std::vector<T>>::exp_str()
{
	static const std::string& value { make_braced_list(type_to_regexp<T>::exp_str()) };
	return value;
};

/**
 * This recursive specialisation allows arbitrary nesting of std::vector.
 */
template<typename T>
struct string_to_value<std::vector<T>>
{
	static std::vector<T> parse(const std::string& input)
	{
		return parse_braced_list<T>(input);
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec)
{
	//std::cout << "printing std::vector" << std::endl;
	const auto vec_size = vec.size();
	out << '{';
	for (size_t i = 0; i < vec_size; ++i) {
		out << vec[i];
		if (i < (vec_size - 1))
			out << ", ";
	}
	out << '}';
	return out;
}

template<typename T>
std::istream& operator>>(std::istream& in, std::vector<T>& vec)
{
	//std::cout << "parsing std::vector" << std::endl;
	std::string value;
	std::getline(in, value);

	vec = noma::typa::parse_braced_list<T>(value);

	return in;
}

} // namespace typa
} // namespace noma

#endif // noma_typa_std_vector_hpp
