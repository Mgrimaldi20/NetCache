#ifndef __NETCACHE_FRAMEWORK_CMDDISPATCHER_H__
#define __NETCACHE_FRAMEWORK_CMDDISPATCHER_H__

#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <optional>
#include <string_view>
#include <algorithm>
#include <type_traits>
#include <concepts>

#include "network/Client.h"

#include "protocol/Parser.h"

#include "StringHash.h"
#include "log/Log.h"

namespace CmdConcepts
{
	using ValidReturnType = std::optional<std::string>;

	template<typename T>
	concept ValidPair = requires (T t)
	{
		t.first;
		t.second;
	};

	template<typename T>
	concept ValidCallable = std::invocable<T, const Parser::PayloadType> &&
		requires(T &&t, const Parser::PayloadType pl)
	{
		{ std::invoke(std::forward<T>(t), pl) } -> std::convertible_to<ValidReturnType>;
	};

	template<std::size_t N>
	struct CTS
	{
		constexpr CTS(const char(&str)[N])
		{
			std::copy_n(str, N, data);
		}

		constexpr operator std::string_view() const
		{
			return { data, N - 1 };
		}

		char data[N]{};
	};
}

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
	using CmdHandlerRetType = CmdConcepts::ValidReturnType;
	using CmdHandlerFn = std::move_only_function<CmdHandlerRetType(const Parser::PayloadType)>;

	CmdDispatcher(std::shared_ptr<Log> log);
	~CmdDispatcher();

	CmdDispatcher &Register(std::string cmdid, CmdHandlerFn fn);
	void Register(std::vector<std::pair<std::string, CmdHandlerFn>> elems);

	template<CmdConcepts::ValidPair ...Pairs>
	void Register(Pairs && ...pairs);

	CmdHandlerRetType Dispatch(const Parser::ParsedCmd parsedcmd);

	template<CmdConcepts::CTS CmdId, CmdConcepts::ValidCallable CmdTy, typename ...Args>
	static std::pair<std::string, CmdHandlerFn> MakeCmdPair(Args && ...args);

private:
	std::unordered_map<std::string, CmdHandlerFn, StringHash, std::equal_to<>> handlers;

	std::shared_ptr<Log> log;
};

template<CmdConcepts::ValidPair ...Pairs>
inline void CmdDispatcher::Register(Pairs && ...pairs)
{
	(Register(std::forward<Pairs>(pairs).first, std::forward<Pairs>(pairs).second), ...);
}

template<CmdConcepts::CTS CmdId, CmdConcepts::ValidCallable CmdTy, typename ...Args>
inline std::pair<std::string, CmdDispatcher::CmdHandlerFn> CmdDispatcher::MakeCmdPair(Args && ...args)
{
	if constexpr (std::is_constructible_v<CmdDispatcher::CmdHandlerFn, CmdTy, Args...>)
	{
		auto bound = CmdDispatcher::CmdHandlerFn(std::forward<Args>(args)...);
		return std::pair{ std::move(std::string(CmdId)), std::move(bound) };
	}

	else
	{
		auto cmd = std::make_unique<CmdTy>(std::forward<Args>(args)...);
		auto bound = [cmd = std::move(cmd)](const Parser::PayloadType &pl)
		{
			return (*cmd)(pl);
		};

		return std::pair{ std::move(std::string(CmdId)), std::move(bound) };
	}
}

#endif
