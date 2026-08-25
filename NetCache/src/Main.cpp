#include <iostream>
#include <print>
#include <charconv>
#include <string_view>
#include <system_error>
#include <memory>
#include <exception>
#include <vector>

#include "framework/Asio.h"
#include "framework/Server.h"
#include "framework/CmdDispatcher.h"
#include "framework/ChannelManager.h"

#include "framework/log/Log.h"
#include "framework/log/driver/Driver.h"
#include "framework/log/entry/Entry.h"
#include "framework/log/sink/text/console/ConsoleSink.h"
#include "framework/log/sink/text/file/FileSink.h"
#include "framework/log/formatter/text/basic/BasicTextFormatter.h"
#include "framework/log/policy/level/LevelPolicy.h"
#include "framework/log/policy/trace/StacktracePolicy.h"
#include "framework/log/policy/trace/SourceLocationPolicy.h"

#include "protocol/Parser.h"

constexpr asio::ip::port_type NET_DEFAULT_PORT = 5001;

static asio::ip::port_type serverport = NET_DEFAULT_PORT;

static bool ValidateOptions(int argc, char **argv);

int main(int argc, char **argv)
{
	try
	{
		if (!ValidateOptions(argc, argv))
			return 1;

		std::shared_ptr<Log> log = std::make_shared<Log>(
			"NetCache",
			std::make_shared<Driver>(
				"NetCacheMainLogDriver",
				std::vector<std::shared_ptr<Sink>>
				{
					std::make_shared<ConsoleSink>(std::make_unique<BasicTextFormatter>())
				},
				std::vector<std::shared_ptr<Policy>>
				{
					std::make_shared<LevelPolicy>(Entry::Level::Debug),
					std::make_shared<StacktracePolicy>(Entry::Level::Fatal),
					std::make_shared<SourceLocationPolicy>(Entry::Level::Debug)
				}
			)
		);

		std::shared_ptr<CmdDispatcher> dispatcher = std::make_shared<CmdDispatcher>(log);

		// TODO: Prolly not needed
		std::shared_ptr<ChannelManager> chmanager = std::make_shared<ChannelManager>(log);

		// TODO: Register commands here within the system
		log->Info("Registered protocol commands in the CmdSystem");

		// TODO: Impl this interface
		std::shared_ptr<Parser> parser = nullptr;

		// single thread hint
		asio::io_context ioctx(1);

		Server server(ioctx, serverport, log, dispatcher, parser);

		log->Info("Started NetCache");

		ioctx.run();

		log->Info("NetCache server is exiting...");
	}

	catch (const std::exception &e)
	{
		std::println(std::cerr, "{} :: Fatal Exception :: {}", typeid(e).name(), e.what());
		return 1;
	}

	return 0;
}

bool ValidateOptions(int argc, char **argv)
{
	for (int i=1; i<argc; i++)
	{
		if (argv[i][0] != '-')
			continue;

		switch (argv[i][1])
		{
			case 'p':
			{
				std::string_view portstr(argv[i] + 2);

				if (portstr.empty() || portstr[0] != ':')
				{
					std::println("Invalid port number command line format: {}", argv[i]);
					return false;
				}

				std::from_chars_result result = std::from_chars(
					portstr.data() + 1,
					portstr.data() + portstr.size(),
					serverport
				);

				if (result.ec != std::errc())
				{
					std::println("Invalid port number: {}", portstr.substr(1));
					return false;
				}

				std::println("Server port set to: {}", serverport);

				break;
			}

			case '?':
				std::println("\nUsage:\n");
				std::println("NetCache [-p:<port>] [-?]");
				std::println("------------------------------------------------------------");
				std::println("-p:<port>              Specify the port number of the server");
				std::println("-?                     Prints out this help message and exit");

				return false;

			default:
				std::println("Unknown command line options flag: {}", argv[i]);
				return false;
		}
	}

	return true;
}
