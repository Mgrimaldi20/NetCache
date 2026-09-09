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
	return std::optional<std::string>();
}

std::optional<std::string> KVStore::Get(std::initializer_list<std::string_view> keys)
{
	return std::optional<std::string>();
}

bool KVStore::Set(std::string_view key, std::string_view value)
{
	return false;
}

bool KVStore::Set(std::initializer_list<std::pair<std::string_view, std::string_view>> elems)
{
	return false;
}

bool KVStore::Del(std::string_view key)
{
	return false;
}

bool KVStore::Del(std::initializer_list<std::string_view> keys)
{
	return false;
}
