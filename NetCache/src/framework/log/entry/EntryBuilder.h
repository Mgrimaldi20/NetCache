#ifndef __NETCACHE_FRAMEWORK_LOG_ENTRY_ENTRYBUILDER_H__
#define __NETCACHE_FRAMEWORK_LOG_ENTRY_ENTRYBUILDER_H__

#include <memory>

#include "framework/log/driver/Driver.h"
#include "Entry.h"

/*
* Class: EntryBuilder
* Constructs the entry fluidly and should forward to a driver.
* 
*	Name: Sets the name of the log message
*	Level: Sets the log level of the entry
*	Message: The message to write to the driver
*	Timestamp: The timestamp of the message
*	SourceLocation: The source code location where the message originates
*	Stacktrace: The stack trace of the log message
*/
class EntryBuilder : private Entry
{
public:
	EntryBuilder(
		std::weak_ptr<Driver> driver,
		std::source_location srcloc = std::source_location::current(),
		std::chrono::system_clock::time_point time = std::chrono::system_clock::now()
	);

	EntryBuilder(const EntryBuilder &) = delete;
	EntryBuilder &operator=(const EntryBuilder &) = delete;

	EntryBuilder(EntryBuilder &&other) noexcept;
	EntryBuilder &operator=(EntryBuilder &&other) noexcept = default;

	~EntryBuilder();

	EntryBuilder &Name(std::string_view name) &;
	EntryBuilder &&Name(std::string_view name) &&;

	EntryBuilder &Level(Entry::Level entrylevel) &;
	EntryBuilder &&Level(Entry::Level entrylevel) &&;

	EntryBuilder &Message(std::string msg) &;
	EntryBuilder &&Message(std::string msg) &&;

	EntryBuilder &Timestamp(std::chrono::system_clock::time_point time) &;
	EntryBuilder &&Timestamp(std::chrono::system_clock::time_point time) &&;

	EntryBuilder &SourceLocation(std::source_location srcloc) &;
	EntryBuilder &&SourceLocation(std::source_location srcloc) &&;

	EntryBuilder &Stacktrace(std::stacktrace trace) &;
	EntryBuilder &&Stacktrace(std::stacktrace trace) &&;

private:
	std::weak_ptr<Driver> driver;
};

#endif
