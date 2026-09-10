#include "KVStore.h"

KVStore::KVStore(std::shared_ptr<Log> log)
	: kvstore(),
	mtx(),
	log(log)
{
	log->Info("Creating KVStore");
}

KVStore::~KVStore()
{
	log->Info("Destroying KVStore");
}

std::optional<std::string> KVStore::Get(std::string_view key)
{
	std::shared_lock lock(mtx);

	auto it = kvstore.find(key);

	if (it == kvstore.end())
	{
		log->Warn("Key: \"{}\" not found in KVStore", key);
		return std::nullopt;
	}

	return it->second.value;
}

bool KVStore::Set(std::string_view key, std::string_view value)
{
	std::unique_lock lock(mtx);

	try
	{
		kvstore.insert_or_assign(std::string(key), KVStore::Entry { .value = std::string(value) });
	}

	catch (...)
	{
		return false;
	}

	return true;
}

bool KVStore::Del(std::string_view key)
{
	std::unique_lock lock(mtx);

	return kvstore.erase(key) > 0;
}
