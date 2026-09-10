#ifndef __NETCACHE_FRAMEWORK_LOG_DRIVER_DRIVER_H__
#define __NETCACHE_FRAMEWORK_LOG_DRIVER_DRIVER_H__

#include <memory>
#include <vector>
#include <string>

#include "framework/log/sink/Sink.h"
#include "framework/log/entry/Entry.h"

/*
* Class: Driver
* A wrapper around the different config options that a logger can ingest and use.
* Basically a big configuration wrapper for logging, add sinks and policies.
* 
*	Submit: Perform all of the policy application and sink writing applicable
*	GetName: Gets the name of the driver class, mainly for observability
*	GetSinks: Returns a const look into the underlying sinks
*	GetPolicies: Returns a const look into the underlying policies
*/
class Driver
{
public:
	struct SinkConfig
	{
		std::string sink;
		std::vector<std::string> policies;
	};

	Driver(std::string drivername = {}, std::vector<std::shared_ptr<Sink>> sinks = {});
	~Driver() = default;

	void Submit(Entry &entry);

	std::string &GetName();

	std::vector<Driver::SinkConfig> GetSinkConfig() const;

private:
	std::string drivername;

	std::vector<std::shared_ptr<Sink>> sinks;
};

#endif
