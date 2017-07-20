// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include "noma/typa/pair.hpp"

namespace noma {
namespace typa {

std::string make_pair(const std::string& first_exp, const std::string& second_exp)
{
	return R"(\((?:)" + first_exp + R"(),(?:)" + second_exp + R"()\))";
}

} // namespace typa
} // namespace noma
