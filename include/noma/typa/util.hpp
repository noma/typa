// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_util_hpp
#define noma_typa_util_hpp

#include <string>

#include <boost/lexical_cast.hpp>

namespace noma {
namespace typa {

/**
 * Type trait to convert a type into a corresponding regular expression string suitable for
 * checking inputs and parsing.
 * Must be specialised for every type to be parsed.
 */
template<typename T, typename Enable = void>
struct type_to_regexp
{
};

/**
 * Conversion object to parse a type from a string value. Can be specialised if needed
 * Default string to value conversion uses boost::lexical_cast, which uses stream operators
 * if available.
 */
template<typename T>
struct string_to_value
{
	static T parse(const std::string& input)
	{
		return boost::lexical_cast<T>(input);
	}
};

/**
 * Remove all whitespaces from a string, by modifying it and returning the same reference.
 * This is called on every input before parsing to avoid bloating the regular expressions
 * with possible whitespace everywhere.
 */
std::string& remove_whitespace(std::string& str);

/**
 * Remove all whitespaces from a const string and return the result as a copy.
 */
std::string remove_whitespace(const std::string& const_str);

/**
 * Write comma separated list of map values to output stream.
 */
template<typename T> // T is indended to be a map
void write_map_value_list(const T& map, std::ostream& os)
{
	for (auto it = map.begin(); it != map.end(); ++it) {
		if(it != map.begin())
			os << ", ";
		os << it->second;
	}
}

template<typename T> // T is indended to be a map
std::string get_map_value_list_string(const T& map)
{
	std::stringstream ret;
	write_map_value_list(map, ret);
	return ret.str();
}

} // namespace typa
} // namespace noma

#endif // noma_typa_util_hpp
