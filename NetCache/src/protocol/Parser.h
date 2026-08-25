#ifndef __NETCACHE_PROTOCOL_PARSER_H__
#define __NETCACHE_PROTOCOL_PARSER_H__

#include <string_view>

class Parser
{
public:
	struct ParsedCmd
	{
		std::string_view cmdid;
		std::string_view data;
	};

	Parser() = default;
	virtual ~Parser() = default;

	virtual Parser::ParsedCmd Parse(std::string_view data) = 0;
};

#endif
