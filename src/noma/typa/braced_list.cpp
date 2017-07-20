// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include "noma/typa/braced_list.hpp"

namespace noma {
namespace typa {

// see header for explanation
std::string make_braced_list(const std::string& entry_exp)
{
	return R"(\{(?:)" + entry_exp + R"()(?:,)" + entry_exp + R"()*\})";
}

} // namespace typa
} // namespace noma
