#include <chrono>
#include <iterator>

#include "entry/EntryBuilderImpl.h"

#include "Log.h"

Log::Log(std::string logname, std::shared_ptr<Driver> driver)
	: logname(std::move(logname)),
	driver(driver)
{
	Info("Attached new driver: {}", driver->GetName());

	for (const auto &sinkname : driver->GetSinkConfig())
		Info("Attached sink: {}", sinkname);

	for (const auto &policyname : driver->GetPolicyConfig())
		Info("Attached policy: {}", policyname);

	Info("Logger started: {}", this->logname);
}

Log::~Log()
{
	Info("Shutting down the Logger: {}", logname);
}

EntryBuilder &Log::Debug(std::string msg, std::source_location loc)
{
	return EntryBuilderImpl(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Debug)
		.Message(msg);
}

EntryBuilder &Log::Info(std::string msg, std::source_location loc)
{
	return EntryBuilderImpl(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Info)
		.Message(msg);
}

EntryBuilder &Log::Warn(std::string msg, std::source_location loc)
{
	return EntryBuilderImpl(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Warn)
		.Message(msg);
}

EntryBuilder &Log::Error(std::string msg, std::source_location loc)
{
	return EntryBuilderImpl(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Error)
		.Message(msg);
}

EntryBuilder &Log::Fatal(std::string msg, std::source_location loc)
{
	return EntryBuilderImpl(driver, loc)
		.Name(logname)
		.Level(Entry::Level::Fatal)
		.Message(msg);
}
