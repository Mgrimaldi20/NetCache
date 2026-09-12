#include "SetCmd.h"

SetCmd::SetCmd(std::shared_ptr<KVStore> kvstore)
	: Cmd(kvstore)
{
}

CmdDispatcher::CmdHandlerRetType SetCmd::operator()(const Parser::PayloadType &pl)
{
	KVStore::SetResult res = kvstore->Set(pl[0], pl[1]);

	if (res == KVStore::SetResult::Inserted)
		return std::format("Successfully inserted value: [{}] into key: [{}]", pl[0], pl[1]);

	if (res == KVStore::SetResult::Updated)
		return std::format("Successfully updated to value: [{}] for key: [{}]", pl[0], pl[1]);

	return std::format("Failed to insert value: [{}] into key: [{}]", pl[0], pl[1]);
}
