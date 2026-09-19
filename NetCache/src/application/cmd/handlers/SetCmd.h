#ifndef __NETCACHE_APPLICATION_CMD_HANDLERS_SETCMD_H__
#define __NETCACHE_APPLICATION_CMD_HANDLERS_SETCMD_H__

#include "application/cmd/Cmd.h"

/*
* Class: SetCmd
* An implementation of the Cmd interface to set a value from the KV store for the supplied key.
* Will either set the key or create a new one if the key doesnt exist.
*
*	operator(): Calls the handler to set a value in the store based on the key supplied, will set or create the key
*/
class SetCmd : public Cmd
{
public:
	SetCmd(std::shared_ptr<KVStore> kvstore);
	virtual ~SetCmd() = default;

	CmdDispatcher::CmdHandlerRetType operator()(const Parser::PayloadType &pl) override final;
};

#endif
