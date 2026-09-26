#include "EntryBuilder.h"

EntryBuilder::EntryBuilder(
	std::weak_ptr<Driver> driver,
	std::source_location srcloc,
	std::chrono::system_clock::time_point time
)
	: driver(driver)
{
	location = srcloc;
	timestamp = time;
}

EntryBuilder::EntryBuilder(EntryBuilder &&other) noexcept
	: Entry(std::move(other)),
	driver(std::move(other.driver))
{
	other.driver.reset();
}

EntryBuilder::~EntryBuilder()
{
	if (std::shared_ptr<Driver> dest = driver.lock())
		dest->Submit(*this);
}

EntryBuilder &EntryBuilder::Name(std::string_view name) &
{
	logname = name;
	return *this;
}

EntryBuilder &&EntryBuilder::Name(std::string_view name) &&
{
	logname = name;
	return std::move(*this);
}

EntryBuilder &EntryBuilder::Level(Entry::Level entrylevel) &
{
	level = entrylevel;
	return *this;
}

EntryBuilder &&EntryBuilder::Level(Entry::Level entrylevel) &&
{
	level = entrylevel;
	return std::move(*this);
}

EntryBuilder &EntryBuilder::Message(std::string msg) &
{
	message = std::move(msg);
	return *this;
}

EntryBuilder &&EntryBuilder::Message(std::string msg) &&
{
	message = std::move(msg);
	return std::move(*this);
}

EntryBuilder &EntryBuilder::Timestamp(std::chrono::system_clock::time_point time) &
{
	timestamp = time;
	return *this;
}

EntryBuilder &&EntryBuilder::Timestamp(std::chrono::system_clock::time_point time) &&
{
	timestamp = time;
	return std::move(*this);
}

EntryBuilder &EntryBuilder::SourceLocation(std::source_location srcloc) &
{
	location = srcloc;
	return *this;
}

EntryBuilder &&EntryBuilder::SourceLocation(std::source_location srcloc) &&
{
	location = srcloc;
	return std::move(*this);
}

EntryBuilder &EntryBuilder::Stacktrace(std::stacktrace trace) &
{
	stacktrace = trace;
	return *this;
}

EntryBuilder &&EntryBuilder::Stacktrace(std::stacktrace trace) &&
{
	stacktrace = trace;
	return std::move(*this);
}
