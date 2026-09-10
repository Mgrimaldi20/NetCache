#include "Driver.h"

Driver::Driver(std::string drivername, std::vector<std::shared_ptr<Sink>> sinks)
	: drivername(std::move(drivername)),
	sinks(sinks)
{
}

void Driver::Submit(Entry &entry)
{
	for (auto &sink : sinks)
		sink->Emit(entry);
}

std::string &Driver::GetName()
{
	return drivername;
}

std::vector<Driver::SinkConfig> Driver::GetSinkConfig() const
{
	std::vector<Driver::SinkConfig> sinkcfg;

	for (const auto &sink: sinks)
	{
		Driver::SinkConfig config;

		config.sink = sink->GetName();
		
		for (const auto &p : sink->GetPolicyConfig())
			config.policies.push_back(p);

		sinkcfg.push_back(config);
	}

	return sinkcfg;
}
