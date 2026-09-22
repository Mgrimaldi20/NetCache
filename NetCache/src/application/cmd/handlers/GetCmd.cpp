#include "protocol/netcache/Response.h"

#include "GetCmd.h"

GetCmd::GetCmd(std::shared_ptr<KVStore> kvstore)
	: Cmd(kvstore)
{
}

CmdDispatcher::CmdHandlerRetType GetCmd::operator()(const Parser::PayloadType &pl)
{
	CmdDispatcher::CmdHandlerRetType ret = kvstore->Get(pl[0]);

	if (!ret)
	{
		return Response()
			.ProtocolName("NC")
			.Version(1)
			.RemainingLength(sizeof(Response::StatusCode))
			.Status(Response::StatusCode::NotFound)
			.Build();
	}

	return Response()
		.ProtocolName("NC")
		.Version(1)
		.RemainingLength(sizeof(Response::StatusCode) + static_cast<std::uint32_t>(ret->size()))
		.Status(Response::StatusCode::Ok)
		.Payload(*ret)
		.Build();
}
