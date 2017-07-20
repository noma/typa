// Copyright (c) 2016 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_parser_error_hpp
#define noma_typa_parser_error_hpp

#include <string>
#include <boost/lexical_cast.hpp>

namespace noma {
namespace typa {

/**
 * Parser exception type to propagate parsing errors.
 */
//class parse_error : public std::runtime_error
//{
//public:
//	parse_error(const std::string& msg) : runtime_error(msg) { };
//};

/**
 * Parser exception type to propagate parsing errors.
 * This implementation is compatible with boost program options which
 * catches bad_lexical_cast, but sadly, doesn't allow to transport a
 * custom message.
 */
class parser_error : public boost::bad_lexical_cast
{
public:
	parser_error(const std::string& msg) {};// : runtime_error(msg) { };
};

} // namespace typa
} // namespace noma

#endif // noma_typa_parser_error_hpp
