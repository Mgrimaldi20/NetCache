#ifndef __NETCACHE_PROTOCOL_FRAMER_H__
#define __NETCACHE_PROTOCOL_FRAMER_H__

#include <string_view>
#include <functional>

/*
* Class: Framer
* The main interface to represent a framer, ensures that all packets for a message arrive.
* 
*	Feed: Gives the framer the next chunk of bytes from the net, creates a full frame from the data
*/
class Framer
{
public:
	using FrameCallbackFn = std::function<void(std::string_view)>;

	Framer() = default;
	virtual ~Framer() = default;

	virtual void Feed(std::string_view data, FrameCallbackFn cbfn) = 0;
};

#endif
