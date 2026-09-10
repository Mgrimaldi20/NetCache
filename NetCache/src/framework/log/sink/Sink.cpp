#include "Sink.h"

Sink::Sink(std::vector<std::shared_ptr<Policy>> policies)
	: policies(policies)
{
}

void Sink::Emit(Entry &entry)
{
	for (auto &policy : policies)
	{
		if (!policy->Transform(entry))
			return;
	}

	Write(entry);
}

std::vector<std::string> Sink::GetPolicyConfig() const
{
	std::vector<std::string> policycfg;

	for (const auto &policy: policies)
		policycfg.push_back(policy->GetName());

	return policycfg;
}
