#ifndef __NETCACHE_FRAMEWORK_LOG_SINK_SINK_H__
#define __NETCACHE_FRAMEWORK_LOG_SINK_SINK_H__

#include <string>
#include <memory>
#include <vector>

#include "framework/log/entry/Entry.h"
#include "framework/log/policy/Policy.h"

/*
* Class: Sink
* An abstract interface designed to represent a logging destination.
* A sink can be anything, eg. console, file, db, rotating files, etc.
* For text based sinks, the TextSink derived interface can be used.
* 
*	Emit: Concrete function to actually write the log, calls the Write function to transform first
*	Write: Pure virtual function, designed to write an entry to the destination
*	Flush: Pure virtual function, should immediately flush the buffer and write
*	GetName: Pure virtual function, get the name of the sink, can be set to anything
*/
class Sink
{
public:
	Sink(std::vector<std::shared_ptr<Policy>> policies = {});
	virtual ~Sink() = default;

	virtual void Flush() = 0;
	virtual std::string &GetName() = 0;

	void Emit(Entry &entry);
	std::vector<std::string> GetPolicyConfig() const;

protected:
	virtual void Write(const Entry &entry) = 0;

private:
	std::vector<std::shared_ptr<Policy>> policies;
};

#endif
