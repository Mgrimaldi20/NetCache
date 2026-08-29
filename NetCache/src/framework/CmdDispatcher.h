#ifndef __NETCACHE_FRAMEWORK_CMDDISPATCHER_H__
#define __NETCACHE_FRAMEWORK_CMDDISPATCHER_H__

#include <memory>
#include <functional>
#include <string>
#include <initializer_list>
#include <utility>
#include <unordered_map>

#include "protocol/Parser.h"

#include "log/Log.h"

/*
* Class: CmdDispatcher
* Responsible for registering user defined commands, and dispatching/calling parsed commands.
* Contains the ParsedCmd struct which is created by the client parser interface.
* A command handler is a function object that can store any type of functor, function, lambda object.
*
*	Register: Registers a command handler within the system by its id
*	Dispatch: Calls the registered handlers by id
*/
class CmdDispatcher
{
public:
	using CmdHandlerFn = std::function<void(const Parser::ParsedCmd &)>;

	CmdDispatcher(std::shared_ptr<Log> log);
	~CmdDispatcher();

	void Register(std::string cmdid, CmdHandlerFn fn);
	void Register(std::initializer_list<std::pair<std::string, CmdHandlerFn>> elems);

	void Dispatch(const Parser::ParsedCmd &parsedcmd);

private:
	struct StringHash
	{
		using is_transparent = void;
		size_t operator()(std::string_view sv) const { return std::hash<std::string_view>{}(sv); }
		size_t operator()(const std::string &s) const { return std::hash<std::string>{}(s); }
	};

	std::unordered_map<std::string, CmdHandlerFn, CmdDispatcher::StringHash, std::equal_to<>> handlers;

	std::shared_ptr<Log> log;
};

#endif
