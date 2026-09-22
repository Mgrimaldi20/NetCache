#include <utility>

#include "StringHash.h"

size_t StringHash::operator()(std::string_view sv) const noexcept
{
	return std::hash<std::string_view>{}(sv);
}

size_t StringHash::operator()(const std::string &s) const noexcept
{
	return std::hash<std::string_view>{}(s);
}

size_t StringHash::operator()(const char *cc) const noexcept
{
	return std::hash<std::string_view>{}(cc);
}
