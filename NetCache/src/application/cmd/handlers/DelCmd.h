#ifndef __NETCACHE_APPLICATION_CMD_HANDLERS_DELCMD_H__
#define __NETCACHE_APPLICATION_CMD_HANDLERS_DELCMD_H__

#include "application/cmd/Cmd.h"

/*
* Class: DelCmd
* An implementation of the Cmd interface to delete a key and value from the KV store for the supplied key.
*
*	operator(): Calls the handler to delete a value from the store based on the key supplied, deletes the entire entry
*/
class DelCmd : public Cmd
{
public:
	DelCmd(std::shared_ptr<KVStore> kvstore);
	virtual ~DelCmd() = default;

	CmdDispatcher::CmdHandlerRetType operator()(const Parser::PayloadType pl) override final;
};

#endif
