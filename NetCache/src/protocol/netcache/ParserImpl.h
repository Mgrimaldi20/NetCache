#ifndef __NETCACHE_PROTOCOL_NETCACHE_PARSERIMPL_H__
#define __NETCACHE_PROTOCOL_NETCACHE_PARSERIMPL_H__

#include <array>

#include "framework/log/Log.h"

#include "protocol/Parser.h"

class ParserImpl : public Parser
{
public:
	ParserImpl(std::shared_ptr<Log> log);
	virtual ~ParserImpl();

	Parser::ParsedCmd Parse(std::string_view frame) override final;

private:
	static constexpr std::size_t NC_HEADER_SIZE = 7;
	static constexpr std::size_t NC_COMMAND_SIZE = 1;

	static constexpr std::size_t NC_MAX_ARGS = 256;

	std::uint32_t ReadUInt32(std::string_view data);

	std::shared_ptr<Log> log;

	std::array<std::string_view, NC_MAX_ARGS> exargs;
};

#endif
