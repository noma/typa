// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_braced_list_hpp
#define noma_typa_braced_list_hpp

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <utility>

#include <boost/lexical_cast.hpp>

#include "noma/typa/parser_error.hpp"
#include "noma/typa/util.hpp"

namespace noma {
namespace typa {

/**
 * Generate a regular expression string for a braced, comma separated list for a given
 * entry expression string (C++11 modified ECMAScript).
 * NOTE: No whitespaces allowed in match.
 */
std::string make_braced_list(const std::string& entry_exp);

/**
 * Parse a braced list into a std::vector for an entry type T.
 * Input Format: "{T, T, ...}"
 * T can be a braced list, too.
 */
template<typename T>
std::vector<T> parse_braced_list(const std::string& input)
{
	const std::string input_no_ws { remove_whitespace(input) };
	const std::regex entry_exp { type_to_regexp<T>::exp_str() };
	const std::regex list_exp { make_braced_list(type_to_regexp<T>::exp_str() ) };

	// check format
	if(!std::regex_match(input_no_ws, list_exp))
		throw parser_error("noma::typa::parse_braced_list(): error: malformed input, should be comma separated braced list, e.g. { T, T, ..}.");

	std::vector<T> result;

	std::smatch sm;
	std::string::const_iterator cit = input_no_ws.cbegin();
	while (std::regex_search(cit, input_no_ws.cend(), sm, entry_exp)) {
		//std::cout << "#Matches: " << sm.size() << std::endl;
		for (auto& m : sm) {
			//	std::cout << "\t\t" << m.str() << std::endl;
			result.push_back(string_to_value<T>::parse(m.str())); // TODO: catch exception here?
		}
		cit = sm[0].second; // start of remaining string
	}

	return result;
}

} // namespace typa
} // namespace noma

#endif // noma_typa_braced_list_hpp
