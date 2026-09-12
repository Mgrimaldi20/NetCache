#include "GetCmd.h"

GetCmd::GetCmd(std::shared_ptr<KVStore> kvstore)
	: Cmd(kvstore)
{
}

CmdDispatcher::CmdHandlerRetType GetCmd::operator()(const Parser::PayloadType &pl)
{
	return CmdDispatcher::CmdHandlerRetType();
}
