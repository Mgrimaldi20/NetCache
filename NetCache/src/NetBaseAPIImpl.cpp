#include "NetBaseAPIImpl.h"

NetBaseAPIImpl::NetBaseAPIImpl(
	std::shared_ptr<CmdDispatcher> dispatcher,
	std::shared_ptr<ChannelManager> chmanager,
	std::shared_ptr<Log> log
)
	: dispatcher(dispatcher),
	chmanager(chmanager),
	log(log)
{
}

CmdDispatcher *NetBaseAPIImpl::GetCmdDispatcher()
{
	return dispatcher.get();
}

ChannelManager *NetBaseAPIImpl::GetChannelManager()
{
	return chmanager.get();
}

Log *NetBaseAPIImpl::GetLogger()
{
	return log.get();
}
