// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_typa_wrapper_hpp
#define noma_typa_wrapper_hpp

namespace noma {
namespace typa {

/**
 * Generic wrapper class template to generate a new type from existing types
 * that can have its own overloaded stream operators and parsing routines.
 */
template<typename T>
class wrapper
{
public:
	wrapper() = default;
	wrapper(const T& wrappee) : wrappee_(wrappee) { };
	wrapper(T&& wrappee) : wrappee_(std::move(wrappee)) { };

	T& get() { return wrappee_; }
	const T& get() const { return wrappee_; }
private:
	T wrappee_;
};

} // namespace typa
} // namespace noma

#endif // noma_typa_wrapper_hpp
