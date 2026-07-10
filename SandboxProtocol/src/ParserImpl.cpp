#include <charconv>
#include <string_view>
#include <system_error>
#include <format>

#include "ParserImpl.h"

ClientAPI::Parser::ParsedCmd ParserImpl::Parse(const char *data, std::size_t length)
{
	// just something silly to stop the warnings... do whatever you need to here
	std::string_view datasv(data, length);

	if (datasv.empty())
		throw std::runtime_error("Command is empty");

	while (!datasv.empty() && (datasv.back() == '\n' || datasv.back() == '\r'))
		datasv.remove_suffix(1);

	if (datasv.empty())
		throw std::runtime_error("Command contains only newlines");

	Parser::ParsedCmd cmd =
	{
		.cmdid = static_cast<std::uint_least16_t>(-1),
		.data = ""
	};

	std::from_chars_result result = std::from_chars(
		datasv.data(),
		datasv.data() + datasv.size(),
		cmd.cmdid
	);

	if (result.ec != std::errc())
		throw std::runtime_error(std::format("Could not parse command :: {}", datasv));

	std::size_t parsed = result.ptr - datasv.data();
	if (parsed < datasv.size())
	{
		std::string_view payload = datasv.substr(parsed);

		if (!payload.empty() && payload.front() == ' ')
			payload.remove_prefix(1);

		cmd.data = payload.data();
		cmd.length = payload.size();
	}

	return cmd;
}
