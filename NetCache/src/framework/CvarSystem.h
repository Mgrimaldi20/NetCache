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
#include <string_view>
#include <optional>
#include <shared_mutex>

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

/*
* Class: CvarSystem
* Responsible for registering configuration variables sourced from the created resource .cfg file.
* The file is loaded from the specified full file path and has two modes of operation.
* The default with no strict file checks, a new file will be created and written to for runtime Cvar operation.
* With strict file checks, the config file must exist first, and will never be overwritten, no runtime Cvars.
*
*	Register: Registers a Cvar with its name and associated value, or as multiple pairs
*	Get: Grabs a const reference to a Cvar value from the Cvar system, it can only be retrieved, not changed
*	MakeCvarPair: Creates a Cvar pair that can be called by register for convenicence
*/
class CvarSystem
{
public:
	using CvarValue = std::variant<bool, int, float, std::string>;

	struct Cvar
	{
		CvarValue value;
	};

	CvarSystem(std::filesystem::path fullpath, std::shared_ptr<Log> log, bool cvarmode = true);
	~CvarSystem();

	CvarSystem &Register(std::string name, Cvar defvalue);
	void Register(std::vector<std::pair<std::string, Cvar>> elems);

	template<CvarConcepts::ValidPair ...Pairs>
	void Register(Pairs && ...pairs);

	const std::optional<const CvarSystem::Cvar &const> Get(std::string_view name) const;

	static std::pair<std::string, CvarSystem::Cvar> MakeCvarPair(std::string name, CvarSystem::Cvar value);

private:
	void Deserialize(std::istream &istrm);
	void Serialize(std::ostream &ostrm) const;

	template<typename T>
	static bool TryParse(std::string_view sv, T &out);

	std::unordered_map<std::string, Cvar, StringHash, std::equal_to<>> cvars;

	mutable std::shared_mutex mtx;

	std::filesystem::path fullpath;
	std::shared_ptr<Log> log;

	bool cvarmode;
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
