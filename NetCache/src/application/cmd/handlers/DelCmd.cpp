#include "protocol/netcache/Response.h"

#include "DelCmd.h"

DelCmd::DelCmd(std::shared_ptr<KVStore> kvstore)
	: Cmd(kvstore)
{
}

CmdDispatcher::CmdHandlerRetType DelCmd::operator()(const Parser::PayloadType &pl)
{
	bool res = kvstore->Del(pl[0]);

	if (!res)
	{
		return Response()
			.ProtocolName("NC")
			.Version(1)
			.RemainingLength(sizeof(Response::StatusCode))
			.Status(Response::StatusCode::InternalError)
			.Build();
	}

	return Response()
		.ProtocolName("NC")
		.Version(1)
		.RemainingLength(sizeof(Response::StatusCode))
		.Status(Response::StatusCode::Ok)
		.Build();
}
