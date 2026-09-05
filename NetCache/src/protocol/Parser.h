#ifndef __NETCACHE_PROTOCOL_PARSER_H__
#define __NETCACHE_PROTOCOL_PARSER_H__

#include <string_view>
#include <span>

/*
* Class: Parser
* The main interface to represent a parser, breaks up parts of the message into logical components.
* 
*	Parse: Breaks up the message into a parsed command, returns the command ID with an args list
*/
class Parser
{
public:
	using PayloadType = std::span<const std::string_view>;

	struct ParsedCmd
	{
		std::string_view cmdid;
		PayloadType args;
	};

	Parser() = default;
	virtual ~Parser() = default;

	virtual Parser::ParsedCmd Parse(std::string_view frame) = 0;
};

#endif
