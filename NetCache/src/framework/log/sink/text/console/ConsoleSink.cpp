#include <print>
#include <iostream>

#include "ConsoleSink.h"

ConsoleSink::ConsoleSink(std::unique_ptr<TextFormatter> formatter, std::vector<std::shared_ptr<Policy>> policies)
	: sinkname("STDOUT"),
	formatter(std::move(formatter)),
	TextSink(policies)
{
}

void ConsoleSink::Write(const Entry &entry)
{
	if (formatter)
		std::print(std::cout, "{}", formatter->Format(entry));
}

void ConsoleSink::Flush()
{
	std::cout.flush();
}

std::string &ConsoleSink::GetName()
{
	return sinkname;
}

void ConsoleSink::SetFormatter(std::unique_ptr<TextFormatter> fmtter)
{
	formatter = std::move(fmtter);
}
