#ifndef __NETCACHE_FRAMEWORK_CVARSYSTEM_H__
#define __NETCACHE_FRAMEWORK_CVARSYSTEM_H__

#include <iostream>
#include <memory>
#include <unordered_map>
#include <variant>
#include <string>
#include <functional>
#include <filesystem>
#include <utility>
#include <charconv>

#include "StringHash.h"
#include "log/Log.h"

namespace CvarConcepts
{
	template<typename T>
	concept ValidPair = requires (T t)
	{
		t.first;
		t.second;
	};
}

template<typename ...Ts> struct Overloaded : Ts... { using Ts::operator()...; };

class CvarSystem
{
public:
	using CvarValue = std::variant<bool, int, float, std::string>;

	struct Cvar
	{
		CvarValue value;
	};

	CvarSystem(std::filesystem::path fullpath, std::shared_ptr<Log> log, bool strictfilechecks = false);
	~CvarSystem();

	CvarSystem &Register(std::string name, Cvar defvalue);
	void Register(std::vector<std::pair<std::string, Cvar>> elems);

	template<CvarConcepts::ValidPair ...Pairs>
	void Register(Pairs && ...pairs);

	static std::pair<std::string, CvarSystem::Cvar> MakeCvarPair(std::string name, CvarSystem::Cvar value);

private:
	void Deserialize(std::istream &istrm);
	void Serialize(std::ostream &ostrm) const;

	template<typename T>
	static bool TryParse(std::string_view sv, T &out);

	std::unordered_map<std::string, Cvar, StringHash, std::equal_to<>> cvars;

	std::filesystem::path fullpath;
	std::shared_ptr<Log> log;
};

template<CvarConcepts::ValidPair ...Pairs>
inline void CvarSystem::Register(Pairs && ...pairs)
{
	(Register(std::forward<Pairs>(pairs).first, std::forward<Pairs>(pairs).second), ...);
}

template<typename T>
inline bool CvarSystem::TryParse(std::string_view sv, T &out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc() && ptr == sv.data() + sv.size();
}

#endif
