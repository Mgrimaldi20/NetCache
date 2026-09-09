#include <utility>

#include "StringHash.h"

size_t StringHash::operator()(std::string_view sv) const
{
	return std::hash<std::string_view>{}(sv);
}

size_t StringHash::operator()(const std::string &s) const
{
	return std::hash<std::string>{}(s);
}
