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
	using UnderlyingOrSelf = std::conditional_t<std::is_enum_v<T>, std::underlying_type_t<T>, T>;

	template<typename T>
	concept ValidUIntType = []<typename U = UnderlyingOrSelf<T>>()
	{
		return std::same_as<T, uint8_t>
			|| std::same_as<T, uint16_t>
			|| std::same_as<T, uint32_t>
			|| std::same_as<T, uint64_t>;
	}();
}

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
	static void WriteUInt(std::string &buffer, const T value);

	std::string protoname;
	std::uint8_t version;
	std::uint32_t remaininglen;
	Response::StatusCode status;
	std::string payload;
};

template<ValidUIntType T>
inline void Response::WriteUInt(std::string &buffer, const T value)
{
	auto rawval = static_cast<UnderlyingOrSelf<T>>(value);

	constexpr std::size_t RV_SIZE = sizeof(rawval);

	if constexpr (RV_SIZE > 1 && std::endian::native == std::endian::little)
		rawval = std::byteswap(rawval);

	const std::size_t oldsize = buffer.size();

	buffer.resize_and_overwrite(oldsize + RV_SIZE, [&](char *buf, std::size_t count)
	{
		*reinterpret_cast<decltype(rawval) *>(buf + oldsize) = rawval;
		return count;
	});
}

#endif
