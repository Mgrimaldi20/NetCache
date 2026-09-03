#include <stdexcept>
#include <bit>
#include <array>
#include <format>

#include "FramerImpl.h"

FramerImpl::FramerImpl(std::shared_ptr<Log> log)
	: log(log),
	pending()
{
	log->Info("New Framer created for client");
}

FramerImpl::~FramerImpl()
{
	log->Info("Framer for client closing...");
}

void FramerImpl::Feed(std::string_view data, FrameCallbackFn cbfn)
{
	while (!data.empty())
	{
		if (!pending.empty())
		{
			if (pending.size() < NC_HEADER_SIZE)
			{
				const std::size_t req = NC_HEADER_SIZE - pending.size();
				const std::size_t amnt = std::min(req, data.size());

				pending.append(data, amnt);
				data.remove_prefix(amnt);

				if (pending.size() < NC_HEADER_SIZE)
					return;
			}

			const std::size_t framesize = GetFrameSize(pending);
			if (pending.size() < framesize)
			{
				const std::size_t req = NC_HEADER_SIZE - pending.size();
				const std::size_t amnt = std::min(req, data.size());

				pending.append(data, amnt);
				data.remove_prefix(amnt);

				if (pending.size() < framesize)
					return;
			}

			cbfn(std::string_view(pending.data(), framesize));

			pending.erase(0, framesize);

			continue;
		}

		if (data.size() < NC_HEADER_SIZE)
		{
			pending.assign(data);
			return;
		}

		const std::size_t framesize = GetFrameSize(data);
		if (data.size() < framesize)
		{
			pending.assign(data);
			return;
		}

		cbfn(data.substr(0, framesize));

		data.remove_prefix(framesize);
	}
}

std::uint64_t FramerImpl::ReadUInt64(std::string_view data, std::size_t offset)
{
	constexpr std::size_t UINT64_SIZE = sizeof(std::uint64_t);

	if (data.size() < UINT64_SIZE)
		throw std::runtime_error("Insufficient data for 64 bit integer read");

	std::array<char, UINT64_SIZE> bytes;

	for (std::size_t i=0; i<UINT64_SIZE; i++)
		bytes[i] = data[offset + i];

	std::uint64_t value = std::bit_cast<std::uint64_t>(bytes);

	if constexpr (std::endian::native == std::endian::little)
		return std::byteswap(value);

	return value;
}

std::size_t FramerImpl::GetFrameSize(std::string_view data)
{
	if (data.size() < NC_HEADER_SIZE)
		throw std::runtime_error("Insufficient data to read NC frame header");

	if (data[0] != NC_MAGIC[0] || data[1] != NC_MAGIC[1])
		throw std::runtime_error(std::format("Invalid magic, expected: {}, got: {}{}", NC_MAGIC, data[0], data[1]));

	std::uint8_t version = static_cast<std::uint8_t>(data[2]);
	if (version != NC_VERSION)
		throw std::runtime_error(std::format("Invalid version, expected: {}, got: {}", NC_VERSION, version));

	constexpr size_t START_REMAINING_LEN_OFFSET = 3;
	std::uint64_t remaininglen = ReadUInt64(data, START_REMAINING_LEN_OFFSET);

	if (remaininglen > std::numeric_limits<std::size_t>::max() - NC_HEADER_SIZE)
		throw std::runtime_error("Frame size overflow");

	const std::size_t framesize = NC_HEADER_SIZE + static_cast<std::size_t>(remaininglen);
	if (framesize > NC_MAX_FRAME_SIZE)
		throw std::runtime_error("Frame exceeds maximum size");

	return framesize;
}
