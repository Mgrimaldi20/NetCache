#ifndef __NETCACHE_APPLICATION_CMD_CMD_H__
#define __NETCACHE_APPLICATION_CMD_CMD_H__

#include <memory>

#include "application/KVStore.h"
#include "framework/CmdDispatcher.h"

/*
* Class: Cmd
* An abstract representation of a command handler, keeps a pointer to a KVStore.
*
*	operator(): Calls the handler and returns a response for the specified command
*/
class Cmd
{
public:
	Cmd(std::shared_ptr<KVStore> kvstore)
		: kvstore(kvstore)
	{
	}

	virtual ~Cmd() = default;

	virtual CmdDispatcher::CmdHandlerRetType operator()(const Parser::PayloadType pl) = 0;

protected:
	std::shared_ptr<KVStore> kvstore;
};

#endif
