#include <fstream>
#include <sstream>

#include "CvarSystem.h"

CvarSystem::CvarSystem(std::filesystem::path fullpath, std::shared_ptr<Log> log)
	: cvars(),
	fullpath(std::move(fullpath)),
	log(log)
{
	if (this->fullpath.empty())
		throw std::runtime_error("The full path provided to the Cvar System is empty");

	if (!this->fullpath.has_filename())
		throw std::runtime_error("The full path provided to the Cvar System has no file name");

	if (this->fullpath.has_parent_path())
		std::filesystem::create_directories(this->fullpath.parent_path());

	const auto &filename = this->fullpath.string();

	if (!std::filesystem::exists(filename))
		std::ofstream cf(filename);

	std::ifstream cfgfile(filename);

	if (!cfgfile.is_open())
		throw std::runtime_error(std::format("Failed to open config file for Deserialization: {}", filename));

	Deserialize(cfgfile);

	log->Info("Cvar System started: {}", filename);
}

CvarSystem::~CvarSystem()
{
	const auto &filename = fullpath.string();

	log->Info("Shutting down the Cvar System: {}", filename);

	std::ofstream cfgfile(filename);

	if (!cfgfile.is_open())
	{
		log->Error("Cvars discarded, failed to open config file for writing: {}", filename);
		return;
	}

	Serialize(cfgfile);
	log->Info("Wrote all archived Cvars to config file: {}", filename);

	std::stringstream sstrm;
	Serialize(sstrm);

	log->Debug("Dumping Cvars for debugging:\n{}", sstrm.str());
}

CvarSystem &CvarSystem::Register(std::string name, Cvar defvalue)
{
	auto [it, inserted] = cvars.try_emplace(std::move(name), std::move(defvalue));

	const auto &varname = it->first;
	const auto &value = it->second;

	const auto PrintVal = [](const auto &val)
	{
		return std::visit(
			Overloaded
			{
				[](const bool b)
				{
					std::string_view str;
					b ? str = "true" : str = "false";
					return std::string(str);
				},
				[](const auto scalar)
				{
					std::stringstream sstrm;
					sstrm << scalar;
					return sstrm.str();
				},
				[](const std::string &s) { return s; }
			},
			val
		);
	};

	if (inserted)
		log->Info("Registered Cvar: {} with value: {}", varname, PrintVal(value));

	else
		log->Info("Cvar: {} already existed with value: {}", varname, PrintVal(value));

	return *this;
}

void CvarSystem::Register(std::vector<std::pair<std::string, Cvar>> elems)
{
	for (auto &[key, val] : elems)
		Register(std::move(key), std::move(val));
}

void CvarSystem::Deserialize(std::istream &istrm)
{
	std::string line;
	while (std::getline(istrm, line))
	{
	}
}

void CvarSystem::Serialize(std::ostream &ostrm) const
{
	for (const auto &[name, cvar] : cvars)
	{
		ostrm << name << "=";

		std::visit(
			Overloaded
			{
				[&](bool b) { ostrm << (b ? "true" : "false"); },
				[&](int i) { ostrm << i; },
				[&](float f) { ostrm << f; },
				[&](const std::string &s) { ostrm << "\"" << s << "\""; }
			},
			cvar
		);

		ostrm << std::endl;
	}
}
