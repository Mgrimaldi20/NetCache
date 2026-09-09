#ifndef __NETCACHE_FRAMEWORK_STRINGHASH_H__
#define __NETCACHE_FRAMEWORK_STRINGHASH_H__

#include <string>
#include <string_view>

struct StringHash
{
	using is_transparent = void;

	size_t operator()(std::string_view sv) const;
	size_t operator()(const std::string &s) const;
};

#endif
