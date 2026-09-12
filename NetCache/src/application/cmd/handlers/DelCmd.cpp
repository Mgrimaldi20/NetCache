#include "DelCmd.h"

DelCmd::DelCmd(std::shared_ptr<KVStore> kvstore)
	: Cmd(kvstore)
{
}

CmdDispatcher::CmdHandlerRetType DelCmd::operator()(const Parser::PayloadType &pl)
{
	bool res = kvstore->Del(pl[0]);

	if (res)
		return std::format("Successfully deleted key: [{}]", pl[0]);

	return std::format("Failed to delete key: [{}]", pl[0]);
}
