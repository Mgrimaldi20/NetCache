#ifndef __NETCACHE_FRAMEWORK_CVARSYSTEM_H__
#define __NETCACHE_FRAMEWORK_CVARSYSTEM_H__

#include <memory>
#include <unordered_map>
#include <variant>
#include <string>
#include <functional>
#include <filesystem>
#include <ostream>
#include <istream>

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
	using Cvar = std::variant<bool, int, float, std::string>;

	CvarSystem(std::filesystem::path fullpath, std::shared_ptr<Log> log);
	~CvarSystem();

	CvarSystem &Register(std::string name, Cvar defvalue);
	void Register(std::vector<std::pair<std::string, Cvar>> elems);

	template<CvarConcepts::ValidPair ...Pairs>
	void Register(Pairs && ...pairs);

private:
	void Deserialize(std::istream &istrm);
	void Serialize(std::ostream &ostrm) const;

	std::unordered_map<std::string, Cvar, StringHash, std::equal_to<>> cvars;

	std::filesystem::path fullpath;
	std::shared_ptr<Log> log;
};

template<CvarConcepts::ValidPair ...Pairs>
inline void CvarSystem::Register(Pairs && ...pairs)
{
	(Register(std::forward<Pairs>(pairs).first, std::forward<Pairs>(pairs).second), ...);
}

#endif
