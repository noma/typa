// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include "noma/typa/util.hpp"

#include <algorithm>

namespace noma {
namespace typa {

std::string& remove_whitespace(std::string& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	return str;
}

std::string remove_whitespace(const std::string& const_str)
{
	std::string str { const_str };
	return remove_whitespace(str);
}

} // namespace typa
} // namespace noma
