#include <chrono>
#include <iterator>

#include "Log.h"

Log::Log(std::string logname, std::shared_ptr<Driver> driver)
	: logname(std::move(logname)),
	driver(driver)
{
	Info("Attached new driver: {}", driver->GetName());

	for (const auto &sink : driver->GetSinkConfig())
	{
		Info("Attached sink: {}", sink.sink);

		for (const auto &policy : sink.policies)
			Info("\t- Attached policy: {}", policy);
	}

	Info("Logger started: {}", this->logname);
}

Log::~Log()
{
	Info("Shutting down the Logger: {}", logname);
}

EntryBuilder Log::Debug(std::string msg, std::source_location loc)
{
	return EntryBuilder(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Debug)
		.Message(std::move(msg));
}

EntryBuilder Log::Info(std::string msg, std::source_location loc)
{
	return EntryBuilder(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Info)
		.Message(std::move(msg));
}

EntryBuilder Log::Warn(std::string msg, std::source_location loc)
{
	return EntryBuilder(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Warn)
		.Message(std::move(msg));
}

EntryBuilder Log::Error(std::string msg, std::source_location loc)
{
	return EntryBuilder(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Error)
		.Message(std::move(msg));
}

EntryBuilder Log::Fatal(std::string msg, std::source_location loc)
{
	return EntryBuilder(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Fatal)
		.Message(std::move(msg));
}
