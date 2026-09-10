#ifndef __NETCACHE_FRAMEWORK_LOG_SINK_TEXT_TEXTSINK_H__
#define __NETCACHE_FRAMEWORK_LOG_SINK_TEXT_TEXTSINK_H__

#include <memory>

#include "framework/log/formatter/text/TextFormatter.h"
#include "framework/log/sink/Sink.h"

/*
* Class: Sink
* An abstract interface designed to represent a text based logging destination.
* A text sink can be anything text related, eg. files, consoles, debug windows, etc.
*
*	Write: Pure virtual function, designed to write an entry to the destination - from Sink interface
*	Flush: Flushes the buffer immediately - from Sink interface
*	GetName: Pure virtual function, get the name of the sink, can be set to anything - from Sink interface
*	SetFormatter: Pure virtual function, sets the text formatter for text based outputs
*/
class TextSink : public Sink
{
public:
	TextSink(std::vector<std::shared_ptr<Policy>> policies = {})
		: Sink(policies)
	{
	}

	virtual ~TextSink() = default;

	virtual void Flush() = 0;
	virtual std::string &GetName() = 0;

	virtual void SetFormatter(std::unique_ptr<TextFormatter> fmtter) = 0;

protected:
	virtual void Write(const Entry &entry) = 0;
};

#endif
