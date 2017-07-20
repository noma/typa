// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_pair_wrapper_hpp
#define noma_typa_pair_wrapper_hpp

#include <iostream>
#include <string>
#include <utility>

#include "noma/typa/typa.hpp"
#include "noma/typa/wrapper.hpp"

namespace noma {
namespace typa {

template<typename T1, typename T2>
using pair_wrapper = wrapper<std::pair<T1, T2>>;

template<typename T1, typename T2>
struct type_to_regexp<pair_wrapper<T1, T2>>
{
	static const std::string& exp_str();
};

template<typename T1, typename T2>
const std::string& type_to_regexp<pair_wrapper<T1, T2>>::exp_str()
{
	static const std::string& value { make_pair(type_to_regexp<T1>::exp_str(), type_to_regexp<T2>::exp_str()) };
	return value;
};

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const pair_wrapper<T1, T2>& p)
{
	//std::cout << "printing pair_wrapper" << std::endl;
	out << '(' << p.get().first << ", " << p.get().second << ')';
	return out;
}

template<typename T1, typename T2>
std::istream& operator>>(std::istream& in, pair_wrapper<T1, T2>& p)
{
	//std::cout << "parsing pair_wrapper" << std::endl;
	std::string value;
	std::getline(in, value);

	// parse
	p.get() = parse_pair<T1, T2>(value);

	return in;
}

} // namespace typa
} // namespace noma

#endif // noma_typa_pair_wrapper_hpp
