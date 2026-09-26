#include <fstream>
#include <sstream>

#include "CvarSystem.h"

CvarSystem::CvarSystem(std::filesystem::path fullpath, std::shared_ptr<Log> log)
	: cvars(),
	fullpath(std::move(fullpath)),
	log(log)
{
	Deserialize();

	log->Info("Cvar System started: {}", this->fullpath.c_str());
}

CvarSystem::~CvarSystem()
{
	log->Info("Shutting down the Cvar System: {}", fullpath.c_str());

	Serialize();
}

CvarSystem &CvarSystem::Register(std::string name, CvarType defvalue, std::string description)
{
	auto [it, inserted] = cvars.insert_or_assign(
		std::move(name),
		CvarSystem::Cvar
		{
			.value = std::move(defvalue),
			.description = std::move(description)
		}
	);

	if (inserted)
		log->Info("Registered Cvar: {} with value: {}", it->first, it->second.value);

	else
		log->Info("Assigned Cvar: {} with new value: {}", it->first, it->second.value);

	return *this;
}

void CvarSystem::Deserialize()
{
	if (fullpath.empty())
		throw std::runtime_error("The full path provided to the Logger is empty");

	if (!fullpath.has_filename())
		throw std::runtime_error("The full path provided to the Logger has no file name");

	if (fullpath.has_parent_path())
		std::filesystem::create_directories(fullpath.parent_path());

	auto filename = fullpath.c_str();

	std::ifstream cfgfile(filename);

	if (!cfgfile.is_open())
	{
		log->Error("Failed to open config file for reading: {}", filename);
		throw std::runtime_error(std::format("Failed to open config file for Deserialization: {}", filename));
	}

	std::string line;
	while (std::getline(cfgfile, line))
	{
	}
}

void CvarSystem::Serialize() const
{
	auto filename = fullpath.c_str();

	std::ofstream cfgfile(filename);

	if (!cfgfile.is_open())
	{
		log->Error("Failed to open config file for writing: {}", filename);
		return;
	}

	for (const auto &[name, cvar] : cvars)
	{
		cfgfile << name;

		std::visit(
			Overloaded
			{
				[&](bool b) { cfgfile << " b " << b; },
				[&](int i) { cfgfile << " i " << i; },
				[&](float f) { cfgfile << " f " << f; },
				[&](const std::string &s) { cfgfile << " a " << s; }
			},
			cvar.value
		);

		cfgfile << std::endl;
	}
}
