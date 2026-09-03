#ifndef __NETCACHE_PROTOCOL_NETCACHE_FRAMERIMPL_H__
#define __NETCACHE_PROTOCOL_NETCACHE_FRAMERIMPL_H__

#include <memory>
#include <cstdint>
#include <string_view>
#include <string>

#include "framework/log/Log.h"

#include "protocol/Framer.h"

class FramerImpl : public Framer
{
public:
	FramerImpl(std::shared_ptr<Log> log);
	virtual ~FramerImpl();

	void Feed(std::string_view data, FrameCallbackFn cbfn) override final;

private:
	static constexpr std::string_view NC_MAGIC = "NC";
	static constexpr std::uint8_t NC_VERSION = 1;
	static constexpr std::size_t NC_HEADER_SIZE = 11;
	static constexpr std::size_t NC_MAX_FRAME_SIZE = 64 * 1204 * 1024;

	std::uint64_t ReadUInt64(std::string_view data);
	std::size_t GetFrameSize(std::string_view data);

	std::shared_ptr<Log> log;

	std::string pending;
};

#endif
