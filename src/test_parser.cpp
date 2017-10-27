// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "noma/typa/typa.hpp"

using namespace noma::typa;
using real_t = double;
using int_t = int;

template<typename T>
bool test_match_and_parse_strings(const std::vector<std::string>& strings, const std::string& exp_str, const std::string& type_str = typeid(T).name(), bool expect_failure = false)
{
	size_t valid_count = 0;
	for (auto& input : strings) {
		const std::string input_no_ws { remove_whitespace(input) };
		const std::regex exp { exp_str };

		// check format
		if (!std::regex_match(input_no_ws, exp)) {
			if (!expect_failure)
				std::cout << "Could not match: '" << input << "' as valid " << type_str << " literal." << std::endl;
		} else {
			bool parsable = true;
			// try parsing
			try {
				T parse = boost::lexical_cast<T>(input_no_ws);
			} catch(...) {
				std::cout << "Could not parse: '" << input << "' as " << type_str << "." << std::endl;
				parsable = false;
			}
			if (parsable) {
				if (expect_failure)
					std::cout << "Could (unexpectedly) match: '" << input << "' as valid " << type_str << " literal." << std::endl;
				valid_count++;
			}
		}
	} // for

	if ((!expect_failure && valid_count == strings.size()) || (expect_failure && valid_count == 0)) {
		std::cout << "Valid " << type_str << " literals test: passed." << std::endl;
		return true;
	} else {
		std::cout << "Valid " << type_str << " type_str literals test: failed." << std::endl;
	}
	return false;
}


int main(int argc, char* argv[])
{
	// test integer literals
	const std::vector<std::string> valid_integer_literals {
		"0", // one digit
	    "+0", // signed digit
	    "-0", // negative signed digit
		"10", // more digits
		"+100",  // signed, more digits
		"-100" // negative, more digits
	};
	test_match_and_parse_strings<int_t>(valid_integer_literals, type_to_regexp<int_t>::exp_str(), "valid int_t");

	// test valid floating point literals
	const std::vector<std::string> valid_real_literals {
		".0",
		"0.",
		".1",
		"0.e5",
		"1.e5",
		".0e5",
		".1e5",
		"-0e10",
		"+1e10",
		"1e10",
	};
	test_match_and_parse_strings<real_t>(valid_real_literals, type_to_regexp<real_t>::exp_str(), "valid real_t");

	// test invalid floating point literals
	const std::vector<std::string> invalid_real_literals {
		".",
		"-.e",
		"+.e"
	};
	test_match_and_parse_strings<real_t>(invalid_real_literals, type_to_regexp<real_t>::exp_str(), "invalid real_t", true);

	// test complex literals

	// test list

	// test vector

	// test matrix

	return 0;
}

