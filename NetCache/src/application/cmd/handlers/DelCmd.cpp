#include "DelCmd.h"

DelCmd::DelCmd(std::shared_ptr<KVStore> kvstore)
	: Cmd(kvstore)
{
}

CmdDispatcher::CmdHandlerRetType DelCmd::operator()(const Parser::PayloadType &pl)
{
	return CmdDispatcher::CmdHandlerRetType();
}
