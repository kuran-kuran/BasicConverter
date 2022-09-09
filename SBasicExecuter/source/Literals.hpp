#ifndef LITERALS_HPP
#define LITERALS_HPP

// User defined literals
dms::String operator "" _s(const char* str, std::size_t length);
dms::Variable operator "" _n(unsigned long long x);
dms::Variable operator "" _f( long double value );

#endif
