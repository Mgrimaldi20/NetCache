#ifndef __NETCACHE_PROTOCOL_NETCACHE_RESPONSE_H__
#define __NETCACHE_PROTOCOL_NETCACHE_RESPONSE_H__

#include <string>
#include <cstdint>
#include <concepts>
#include <type_traits>
#include <bit>

namespace
{
	template<typename T>
	concept ValidUIntType =
		std::same_as<T, uint8_t> ||
		std::same_as<T, uint16_t> ||
		std::same_as<T, uint32_t> ||
		std::same_as<T, uint64_t>;

	template<typename T>
	concept ValidUIntUnderlyingType = std::is_enum_v<T>
		&& ValidUIntType<std::underlying_type_t<T>>;
}

/*
* Class: Response
* Represents a command response, will get built as a complex object for whichever fields are required.
* All data is and should be moved into the response payload fields to avoid copying.
*
*	ProtocolName: Sets the protocol name in the response
*	Version: Sets the protocol version which this response is for
*	RemainingLength: Sets the remaining length of the response payload not inclusive of this field
*	Status: Set the responses status code to indicate the success or fail reason
*	Payload: Set the actual response data to be consumed
*	Build: Builds and moves out the fully built response bytes
*/
class Response
{
public:
	enum class StatusCode : std::uint8_t
	{
		Ok = 0x00,
		NotFound = 0x01,
		InvalidRequest = 0x02,
		InvalidCommand = 0x03,
		InternalError = 0x04
	};

	Response() = default;
	~Response() = default;

	Response &ProtocolName(std::string pname) &;
	Response &&ProtocolName(std::string pname) &&;

	Response &Version(std::uint8_t ver) &;
	Response &&Version(std::uint8_t ver) &&;

	Response &RemainingLength(std::uint32_t rlen) &;
	Response &&RemainingLength(std::uint32_t rlen) &&;

	Response &Status(Response::StatusCode sc) &;
	Response &&Status(Response::StatusCode sc) &&;

	Response &Payload(std::string pl) &;
	Response &&Payload(std::string pl) &&;

	std::string Build() & = delete;
	std::string Build() &&;

private:
	template<ValidUIntType T>
	static void WriteUInt(std::string &buffer, T value);

	template<ValidUIntUnderlyingType T>
	static void WriteUInt(std::string &buffer, T value);

	std::string protoname;
	std::uint8_t version;
	std::uint32_t remaininglen;
	Response::StatusCode status;
	std::string payload;
};

template<ValidUIntType T>
inline void Response::WriteUInt(std::string &buffer, T value)
{
	constexpr std::size_t VAL_SIZE = sizeof(value);

	if constexpr (VAL_SIZE > 1 && std::endian::native == std::endian::little)
		value = std::byteswap(value);

	const std::size_t oldsize = buffer.size();

	buffer.resize_and_overwrite(oldsize + VAL_SIZE, [&](char *buf, std::size_t count)
	{
		std::construct_at(reinterpret_cast<decltype(value) *>(buf + oldsize), value);
		return count;
	});
}

template<ValidUIntUnderlyingType T>
inline void Response::WriteUInt(std::string &buffer, T value)
{
	return WriteUInt(buffer, static_cast<std::underlying_type_t<T>>(value));
}

#endif
