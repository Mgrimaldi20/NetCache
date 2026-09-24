#ifndef __NETCACHE_APPLICATION_CMD_HANDLERS_GETCMD_H__
#define __NETCACHE_APPLICATION_CMD_HANDLERS_GETCMD_H__

#include "application/cmd/Cmd.h"

/*
* Class: GetCmd
* An implementation of the Cmd interface to get a value from the KV store from the supplied key.
*
*	operator(): Calls the handler to retrieve a value from the store based on the key supplied
*/
class GetCmd : public Cmd
{
public:
	GetCmd(std::shared_ptr<KVStore> kvstore);
	virtual ~GetCmd() = default;

	CmdDispatcher::CmdHandlerRetType operator()(const Parser::PayloadType pl) override final;
};

#endif
