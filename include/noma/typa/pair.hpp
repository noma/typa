// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_pair_hpp
#define noma_typa_pair_hpp

#include <iostream>
#include <regex>
#include <string>
#include <utility>

#include "noma/typa/parser_error.hpp"
#include "noma/typa/util.hpp"

namespace noma {
namespace typa {

/**
 * Generate regular expression string for a pair of two other expressions
 * (C++11 modified ECMAScript).
 * NOTE: No whitespaces allowed in match
 */
std::string make_pair(const std::string& first_exp, const std::string& second_exp);

/**
 * Parse a pair into a std::pair for an entry types T1 and T2.
 * Input Format: "(T1, T2)"
 */
template<typename T1, typename T2>
std::pair<T1, T2> parse_pair(const std::string& input)
{
	const std::string input_no_ws { remove_whitespace(input) };
	const std::regex first_exp { type_to_regexp<T1>::exp_str() };
	const std::regex second_exp { type_to_regexp<T2>::exp_str() };
	const std::regex pair_exp { make_pair(type_to_regexp<T1>::exp_str(), type_to_regexp<T2>::exp_str()) };

	// check format
	if(!std::regex_match(input_no_ws, pair_exp))
		throw parser_error("noma::typa::parse_pair(): error: malformed input, should be comma separated pair, e.g. (T1, T2).");

	std::pair<T1, T2> result;

	std::smatch sm;
	std::string::const_iterator cit = input_no_ws.cbegin();

	// parse first
	std::regex_search(cit, input_no_ws.cend(), sm, first_exp);
	result.first = string_to_value<T1>::parse(sm[0].str());
	cit = sm[0].second;
	// parse second
	std::regex_search(cit, input_no_ws.cend(), sm, second_exp);
	result.second = string_to_value<T2>::parse(sm[0].str());

	return result;
}

} // namespace typa
} // namespace noma

#endif // noma_typa_pair_hpp
