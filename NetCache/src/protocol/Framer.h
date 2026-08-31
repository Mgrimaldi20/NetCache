#ifndef __NETCACHE_PROTOCOL_FRAMER_H__
#define __NETCACHE_PROTOCOL_FRAMER_H__

/*
* Class: Framer
* The main interface to represent a framer, ensures that all packets for a message arrive and form a proper message.
*/
class Framer
{
	Framer() = default;
	virtual ~Framer() = default;
};

#endif
