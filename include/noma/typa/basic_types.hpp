// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_basic_types_hpp
#define noma_typa_basic_types_hpp

#include <complex>
#include <iostream>
#include <string>

#include "noma/typa/util.hpp"

namespace noma {
namespace typa {

/**
 * Returns a regular expression string for an integer literal (C++11 modified ECMAScript).
 * NOTE: as a function for definied initalisation order.
 * R"()"  .. bracing for a raw string literal
 * (?:)   .. sourrounding makes sure no submatches are generated
 * [-+]?  .. zero or one of '+' and '-'
 * [0-9]+ .. at least one digit
 */
const std::string& integer_literal();

/**
 * Specialisation for integer types.
 */
template<class T>
struct type_to_regexp<T, typename std::enable_if<std::is_integral<T>::value>::type>
{
	static const std::string& exp_str() { return integer_literal(); }
};


/**
 * Returns a regular expression string for a floating point literal (C++11 modified ECMAScript).
 * NOTE: as a function for definied initalisation order.
 * R"()"  .. bracing for a raw string literal
 * (?:)   .. sourrounding makes sure no submatches are generated
 *
 * (?:
 * [-+]?  .. zero or one of '+' and '-'
 * (?:    .. group for alternatives
 * (?:    .. first alternative: handles all cases but those with no digits after the dot
 * [0-9]* .. any number of digits
 * \.?    .. optional decimal dot (no dot between the above and below still is a bunch of digits)
 * [0-9]+ .. more digits but at least one
 * )      .. end of first alternative
 * |      .. alternative operator
 * (?:    .. second alternative: handles all cases with no digits after the dot, e.g. '1.e4'
 * [0-9]+ .. at least one digit
 * \.     .. non-optional dot
 * )      .. end ot second alternative
 * )      .. end of alternatives
 * (?:[eE][-+]?[0-9]+)? .. optional exponent (non marking subexpression due to '?:')
 */
const std::string& real_literal();

template<typename T>
struct type_to_regexp<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	static const std::string& exp_str() { return real_literal(); }
};

/**
 * Returns a regular expression string for a C++11 complex literal (C++11 modified ECMAScript).
 * NOTE: as a function for definied initalisation order.
 * NOTE: No whitespaces allowed in match.
 * Basically: (real_literal,real_literal) or just a single real_literal
 * (?:) makes sure not submatches are generated
 */
const std::string& complex_literal();

template<typename T>
struct type_to_regexp<std::complex<T>>
{
	static const std::string& exp_str() { return complex_literal(); }
};

/**
 * Returns a regular expression matching anystring that does not contain any of "{}(),",
 * as those are used for braced lists and pairs.
 */
const std::string& string_literal();

template<>
struct type_to_regexp<std::string>
{
	static const std::string& exp_str() { return string_literal(); }
};

// Examples: regular expression objects used for matching
//const std::regex real_literal_exp { real_literal() };
//const std::regex complex_literal_exp { complex_literal() };
//const std::regex braced_real_list_exp { make_braced_list(real_literal()) };
//const std::regex braced_complex_list_exp { make_braced_list(complex_literal()) };
//const std::regex nested_braced_real_list_exp { make_braced_list(make_braced_list(real_literal())) };
//const std::regex nested_braced_complex_list_exp { make_braced_list(make_braced_list(complex_literal())) };

} // namespace typa
} // namespace noma

#endif // noma_typa_basic_types_hpp
