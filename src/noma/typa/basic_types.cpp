// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include "noma/typa/basic_types.hpp"

namespace noma {
namespace typa {

// see header for explanation
const std::string& integer_literal()
{
	static const std::string value { R"((?:[-+]?[0-9]+))" };
	return value;
}

// see header for explanation
const std::string& real_literal()
{
	static const std::string value { R"((?:[-+]?(?:(?:[0-9]*\.?[0-9]+)|(?:[0-9]+\.))(?:[eE][-+]?[0-9]+)?))" };
	return value;
}

// see header for explanation
const std::string& complex_literal()
{
	static const std::string value { R"((?:\()" + real_literal() + R"(,)" + real_literal() + R"(\)|)" + real_literal() + R"())" };
	return value;
}

// see header for explanation
const std::string& string_literal()
{
	static const std::string value { R"((?:[^\(\)\{\}\,]+))" };  // nonempty, no '{', '}', '(', ')', ',' are allowed
	return value;
}

// NOTE: fully specialised templates go into the *.cpp to avoid introducing multiple definitions
//       which leads to linking errors.

//const std::string& type_to_regexp<int_t>::exp_str { integer_literal() };
//const std::string& type_to_regexp<real_t>::exp_str { real_literal() };
//const std::string& type_to_regexp<complex_t>::exp_str { complex_literal() };

} // namespace typa
} // namespace noma
