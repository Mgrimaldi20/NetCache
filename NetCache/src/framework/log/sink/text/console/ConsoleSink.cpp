#include <print>
#include <iostream>

#include "ConsoleSink.h"

ConsoleSink::ConsoleSink(std::unique_ptr<TextFormatter> formatter)
	: sinkname("STDOUT"),
	formatter(std::move(formatter))
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
