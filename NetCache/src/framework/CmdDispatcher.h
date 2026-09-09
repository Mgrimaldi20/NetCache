#ifndef __NETCACHE_FRAMEWORK_CMDDISPATCHER_H__
#define __NETCACHE_FRAMEWORK_CMDDISPATCHER_H__

#include <memory>
#include <functional>
#include <string>
#include <initializer_list>
#include <utility>
#include <unordered_map>
#include <optional>
#include <string_view>

#include "network/Client.h"

#include "protocol/Parser.h"

#include "StringHash.h"
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
	using CmdHandlerRetType = std::optional<std::string>;
	using CmdHandlerFn = std::function<CmdHandlerRetType(const Parser::PayloadType &)>;

	CmdDispatcher(std::shared_ptr<Log> log);
	~CmdDispatcher();

	void Register(const std::string &cmdid, CmdHandlerFn fn);
	void Register(std::initializer_list<std::pair<std::string, CmdHandlerFn>> elems);

	template<typename ...Args>
	void Register(Args && ...args);

	CmdHandlerRetType Dispatch(const Parser::ParsedCmd &parsedcmd);

private:
	std::unordered_map<std::string, CmdHandlerFn, StringHash, std::equal_to<>> handlers;

	std::shared_ptr<Log> log;
};

template<typename ...Args>
inline void CmdDispatcher::Register(Args && ...args)
{
	static_assert(sizeof...(Args) % 2 == 0, "Register requires an even number of arguments!");
	Register({ { std::forward<Args>(args)... } });
}

#endif
