#ifndef __NETCACHE_FRAMEWORK_CVARSYSTEM_H__
#define __NETCACHE_FRAMEWORK_CVARSYSTEM_H__

#include <memory>
#include <unordered_map>
#include <variant>
#include <string>
#include <functional>
#include <filesystem>

#include "StringHash.h"
#include "log/Log.h"

template<typename ...Ts> struct Overloaded : Ts... { using Ts::operator()...; };

class CvarSystem
{
public:
	using CvarType = std::variant<bool, int, float, std::string>;

	struct Cvar
	{
		CvarType value;
		std::string description;
	};

	CvarSystem(std::filesystem::path fullpath, std::shared_ptr<Log> log);
	~CvarSystem();

	CvarSystem &Register(std::string name, CvarType defvalue, std::string description);

private:
	void Deserialize();
	void Serialize() const;

	std::unordered_map<std::string, Cvar, StringHash, std::equal_to<>> cvars;

	std::filesystem::path fullpath;
	std::shared_ptr<Log> log;
};

#endif
