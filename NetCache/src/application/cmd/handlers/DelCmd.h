#ifndef __NETCACHE_APPLICATION_CMD_HANDLERS_DELCMD_H__
#define __NETCACHE_APPLICATION_CMD_HANDLERS_DELCMD_H__

#include <memory>

#include "application/KVStore.h"
#include "application/cmd/Cmd.h"

class DelCmd : public Cmd
{
public:
	DelCmd(std::shared_ptr<KVStore> kvstore);
	virtual ~DelCmd() = default;

	CmdDispatcher::CmdHandlerRetType operator()(const Parser::PayloadType &pl) override final;
};

#endif
