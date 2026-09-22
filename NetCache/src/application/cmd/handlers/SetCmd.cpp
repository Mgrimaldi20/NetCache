#include "protocol/netcache/Response.h"

#include "SetCmd.h"

SetCmd::SetCmd(std::shared_ptr<KVStore> kvstore)
	: Cmd(kvstore)
{
}

CmdDispatcher::CmdHandlerRetType SetCmd::operator()(const Parser::PayloadType &pl)
{
	KVStore::SetResult res = kvstore->Set(pl[0], pl[1]);

	if (res == KVStore::SetResult::Inserted || res == KVStore::SetResult::Updated)
	{
		return Response()
			.ProtocolName("NC")
			.Version(1)
			.RemainingLength(sizeof(Response::StatusCode))
			.Status(Response::StatusCode::Ok)
			.Build();
	}

	return Response()
		.ProtocolName("NC")
		.Version(1)
		.RemainingLength(sizeof(Response::StatusCode))
		.Status(Response::StatusCode::InternalError)
		.Build();
}
