#ifndef __NETCACHE_APPLICATION_CMD_HANDLERS_GETCMD_H__
#define __NETCACHE_APPLICATION_CMD_HANDLERS_GETCMD_H__


#include "application/cmd/Cmd.h"

class GetCmd : public Cmd
{
public:
	GetCmd(std::shared_ptr<KVStore> kvstore);
	virtual ~GetCmd() = default;

	CmdDispatcher::CmdHandlerRetType operator()(const Parser::PayloadType &pl) override final;
};

#endif
