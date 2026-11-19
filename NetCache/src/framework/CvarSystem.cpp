#include <fstream>
#include <system_error>
#include <print>

#include "CvarSystem.h"

CvarSystem::CvarSystem(std::filesystem::path fullpath, std::shared_ptr<Log> log, bool cvarmode)
	: cvars(),
	mtx(),
	fullpath(std::move(fullpath)),
	log(log),
	cvarmode(cvarmode)
{
	if (this->fullpath.empty())
		throw std::runtime_error("The full path provided to the Cvar System is empty");

	if (!this->fullpath.has_filename())
		throw std::runtime_error("The full path provided to the Cvar System has no file name");

	const auto &filename = this->fullpath.string();

	if (cvarmode)
	{
		if (this->fullpath.has_parent_path())
			std::filesystem::create_directories(this->fullpath.parent_path());

		if (!std::filesystem::exists(filename))
			std::ofstream cf(filename);
	}

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

	if (!cvarmode)
		return;

	log->Info("Wiring Cvars back to config file: {}", filename);

	std::filesystem::path tmpfile = fullpath;
	tmpfile.replace_extension(fullpath.extension().string() + ".tmp");

	std::error_code ec;

	auto RemoveFile = [&](const std::filesystem::path &fp, std::error_code &ec)
	{
		std::filesystem::remove(fp, ec);
		if (ec)
		{
			log->Error(
				"Failed to remove leftover temp file: {} :: {} : {}",
				tmpfile.string(),
				ec.category().name(),
				ec.message()
			);
		}
	};

	try
	{
		{
			std::ofstream cfgfile(tmpfile);

			if (!cfgfile.is_open())
			{
				log->Error("Config file could not be opened, Cvar changes will not be written back to config file");
				RemoveFile(tmpfile, ec);
				return;
			}

			Serialize(cfgfile);
		}

		std::filesystem::rename(tmpfile, fullpath);
	}

	catch (const std::exception &e)
	{
		log->Error("Error safely writing to config file {} :: {}", filename, e.what());
		RemoveFile(tmpfile, ec);
	}
}

CvarSystem &CvarSystem::Register(std::string name, Cvar defvalue)
{
	std::unique_lock lock(mtx);

	auto [it, inserted] = cvars.try_emplace(std::move(name), std::move(defvalue));

	const auto &varname = it->first;
	const auto &value = it->second;

	const auto FormatVal = [](const CvarSystem::Cvar &val)
	{
		return std::visit(
			Overloaded
			{
				[](const bool b)
				{
					return std::string(b ? "true" : "false");
				},
				[](const auto scalar)
				{
					std::to_string(scalar);
				},
				[](const std::string &s) { return s; }
			},
			val.value
		);
	};

	if (inserted)
		log->Info("Registered Cvar: [{}] with value: [{}]", varname, FormatVal(value));

	else
		log->Info("Cvar: [{}] already existed with value: [{}]", varname, FormatVal(value));

	return *this;
}

void CvarSystem::Register(std::vector<std::pair<std::string, Cvar>> elems)
{
	for (auto &[key, val] : elems)
		Register(std::move(key), std::move(val));
}

std::optional<std::reference_wrapper<const CvarSystem::Cvar>> CvarSystem::Fetch(std::string_view name)
{
	std::shared_lock lock(mtx);

	auto it = cvars.find(name);

	if (it == cvars.end())
	{
		log->Warn("Key: \"{}\" not found in Cvar System", name);
		return std::nullopt;
	}

	return it->second;
}

std::pair<std::string, CvarSystem::Cvar> CvarSystem::MakeCvarPair(std::string name, CvarSystem::Cvar value)
{
	return std::pair{ std::move(name), std::move(value) };
}

void CvarSystem::Deserialize(std::istream &istrm)
{
	auto TryParse = []<typename T>(std::string_view sv, T &out)
	{
		auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
		return ec == std::errc() && ptr == sv.data() + sv.size();
	};

	std::string line;
	while (std::getline(istrm, line))
	{
		std::string_view lineview(line);

		if (lineview.empty() || lineview.front() == '#')
			continue;

		std::size_t nvspos = lineview.find_first_of('=');
		if (nvspos == std::string_view::npos)
			continue;

		std::string_view name = lineview.substr(0, nvspos);
		std::string_view valstr = lineview.substr(nvspos + 1);

		CvarValue value;

		if (valstr == "true" || valstr == "false")
			value = (valstr == "true");

		else if (int i; TryParse(valstr, i))
			value = i;

		else if (float f; TryParse(valstr, f))
			value = f;

		else if (valstr.size() >= 2 && valstr.front() == '"' && valstr.back() == '"')
			value = std::string(valstr.substr(1, valstr.size() - 2));

		else
		{
			log->Error("Failed to parse value: '{}', skipping", valstr);
			continue;
		}

		Register(std::string(name), Cvar{ .value = std::move(value) });
	}
}

void CvarSystem::Serialize(std::ostream &ostrm) const
{
	for (const auto &[name, cvar] : cvars)
	{
		std::print(ostrm, "{}=", name);

		std::visit(
			Overloaded
			{
				[&](bool b) { print(ostrm, "{}", (b ? "true" : "false")); },
				[&](int i) { std::print(ostrm, "{}", i); },
				[&](float f) { std::print(ostrm, "{}", f); },
				[&](const std::string &s) { std::print(ostrm, "\"{}\"", s); }
			},
			cvar.value
		);

		std::print(ostrm, "\n");
	}
}
