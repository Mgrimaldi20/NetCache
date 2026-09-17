#include "CmdDispatcher.h"

CmdDispatcher::CmdDispatcher(std::shared_ptr<Log> log)
	: handlers(),
	log(log)
{
	log->Info("Command Dispatcher started");
}

CmdDispatcher::~CmdDispatcher()
{
	log->Info("Shutting down the Command Dispatcher");
}

CmdDispatcher &CmdDispatcher::Register(std::string cmdid, CmdHandlerFn fn)
{
	auto [it, inserted] = handlers.emplace(std::move(cmdid), std::move(fn));

	if (!inserted)
		log->Warn("Failed to register CmdHandler function");

	log->Info("Registered command with ID: {}", std::span<const char>(it->first));

	return *this;
}

void CmdDispatcher::Register(std::vector<std::pair<std::string, CmdHandlerFn>> elems)
{
	for (auto &[key, val] : elems)
		Register(std::move(key), std::move(val));
}

CmdDispatcher::CmdHandlerRetType CmdDispatcher::Dispatch(const Parser::ParsedCmd &parsedcmd)
{
	auto handler = handlers.find(parsedcmd.cmdid);

	if (handler == handlers.end())
	{
		log->Warn("Command: \"{}\" does not exist... Cannot execute", std::span<const char>(parsedcmd.cmdid));
		return std::nullopt;
	}

	return handler->second(parsedcmd.args);
}
