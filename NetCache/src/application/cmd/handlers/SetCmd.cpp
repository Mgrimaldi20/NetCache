#include "SetCmd.h"

SetCmd::SetCmd(std::shared_ptr<KVStore> kvstore)
	: Cmd(kvstore)
{
}

CmdDispatcher::CmdHandlerRetType SetCmd::operator()(const Parser::PayloadType &pl)
{
	return CmdDispatcher::CmdHandlerRetType();
}
