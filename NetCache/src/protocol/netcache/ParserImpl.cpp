#include <stdexcept>

#include "ParserImpl.h"

ParserImpl::ParserImpl(std::shared_ptr<Log> log)
	: log(log)
{
	log->Info("New Parser created for Client");
}

ParserImpl::~ParserImpl()
{
	log->Info("Closing Parser for Client");
}

Parser::ParsedCmd ParserImpl::Parse(std::string_view frame)
{
	if (frame.size() < NC_HEADER_SIZE + NC_COMMAND_SIZE)
		throw std::runtime_error("Current frame does not contain a command");

	std::string_view cmd = frame.substr(NC_HEADER_SIZE, 1);
	std::string_view args = frame.substr(NC_HEADER_SIZE + NC_COMMAND_SIZE);

	std::size_t bytesread = 0;

	unsigned int idx = 0;
	while (bytesread < args.size())
	{
		std::uint32_t len = ReadUInt32(args);
		bytesread += 4;

		std::string_view prop = args.substr(bytesread, len);
		bytesread += prop.length();

		exargs[idx] = prop;
		idx++;
	}

	return ParsedCmd
	{
		.cmdid = cmd,
		.args = exargs
	};
}

std::uint32_t ParserImpl::ReadUInt32(std::string_view data)
{
	constexpr std::size_t UINT32_SIZE = sizeof(std::uint32_t);

	if (data.size() < UINT32_SIZE)
		throw std::runtime_error("Insufficient data for 32 bit integer read");

	std::array<char, UINT32_SIZE> bytes;

	for (std::size_t i=0; i<UINT32_SIZE; i++)
		bytes[i] = data[i];

	std::uint32_t value = std::bit_cast<std::uint32_t>(bytes);

	if constexpr (std::endian::native == std::endian::little)
		value = std::byteswap(value);

	return value;
}
