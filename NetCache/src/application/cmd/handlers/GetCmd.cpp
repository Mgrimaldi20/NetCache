#include "GetCmd.h"

GetCmd::GetCmd(std::shared_ptr<KVStore> kvstore)
	: Cmd(kvstore)
{
}

CmdDispatcher::CmdHandlerRetType GetCmd::operator()(const Parser::PayloadType &pl)
{
	CmdDispatcher::CmdHandlerRetType ret = kvstore->Get(pl[0]);
	if (!ret)
		return std::format("Key: [{}] was not found", pl[0]);

	return ret;
}
