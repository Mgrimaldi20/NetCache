#ifndef __NETCACHE_PROTOCOL_PARSER_H__
#define __NETCACHE_PROTOCOL_PARSER_H__

#include <string_view>

/*
* Class: Parser
* The main interface to represent a parser, breaks up parts of the message into logical components.
* 
*	Parse: Breaks up the message into a parsed command
*/
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
