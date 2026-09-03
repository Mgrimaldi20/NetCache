#ifndef __NETCACHE_NETWORK_SERVER_H__
#define __NETCACHE_NETWORK_SERVER_H__

#include <memory>
#include <functional>

#include "Asio.h"

#include "framework/CmdDispatcher.h"
#include "framework/log/Log.h"

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
		std::shared_ptr<CmdDispatcher> dispatcher
	);

	Server(const Server &) = delete;
	Server &operator=(const Server &) = delete;

	Server(Server &&) = default;
	Server &operator=(Server &&) = default;

	~Server();

private:
	asio::awaitable<void> Listener();

	void RegisterSignals();
	void Stop();

	std::reference_wrapper<asio::io_context> ioctx;
	asio::ip::port_type port;
	std::shared_ptr<Log> log;
	std::shared_ptr<CmdDispatcher> dispatcher;

	asio::signal_set signals;
	asio::ip::tcp::acceptor acceptor;
};

#endif
