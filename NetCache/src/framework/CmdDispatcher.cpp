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

void CmdDispatcher::Register(const std::string &cmdid, CmdHandlerFn fn)
{
	bool inserted = handlers.emplace(cmdid, fn).second;

	if (!inserted)
		log->Warn("Failed to register CmdHandler function");

	log->Info("Registered command with ID: {}", std::span<const char>(cmdid));
}

void CmdDispatcher::Register(std::initializer_list<std::pair<const std::string &, CmdHandlerFn>> elems)
{
	for (auto &[key, val] : elems)
		Register(key, val);
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
