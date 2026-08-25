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
		log->Warn("Failed to register CmdHandler");

	log->Info("Registered command with ID: {}", cmdid);
}

void CmdDispatcher::Register(std::initializer_list<std::pair<std::string, CmdHandlerFn>> elems)
{
	for (auto &[key, val] : elems)
		Register(key, val);
}

void CmdDispatcher::Dispatch(std::shared_ptr<Client> client, const Parser::ParsedCmd &parsedcmd)
{
	auto handler = handlers.find(parsedcmd.cmdid);

	if (!handler)
		return;

	handler(client, parsedcmd);
}
