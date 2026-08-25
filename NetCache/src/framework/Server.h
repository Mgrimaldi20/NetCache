#ifndef __NETCACHE_FRAMEWORK_SERVER_H__
#define __NETCACHE_FRAMEWORK_SERVER_H__

#include <memory>
#include <functional>

#include "Asio.h"
#include "CmdDispatcher.h"

#include "log/Log.h"

#include "../protocol/Parser.h"

/*
* Class: Server
* The main server class, responsible for accepting incoming connections and spawning sessions.
* The server will run on a single thread, and coroutines will be used to handle async accepts.
*/
class Server
{
public:
	Server(
		asio::io_context &ioctx,
		asio::ip::port_type port,
		std::shared_ptr<Log> log,
		std::shared_ptr<CmdDispatcher> dispatcher,
		std::shared_ptr<Parser> parser
	);

	Server(const Server &) = delete;
	Server &operator=(const Server &) = delete;

	Server(Server &&) = default;
	Server &operator=(Server &&) = default;

	~Server();

private:
	asio::awaitable<void> Listener(
		asio::ip::port_type port,
		std::shared_ptr<CmdDispatcher> dispatcher,
		std::shared_ptr<Parser> parser
	);

	void RegisterSignals();

	std::reference_wrapper<asio::io_context> ioctx;
	std::shared_ptr<Log> log;

	asio::signal_set signals;
};

#endif
