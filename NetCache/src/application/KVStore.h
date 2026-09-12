#ifndef __NETCACHE_APPLICATION_KVSTORE_H__
#define __NETCACHE_APPLICATION_KVSTORE_H__

#include <memory>
#include <unordered_map>
#include <string>
#include <shared_mutex>
#include <string_view>
#include <optional>
#include <utility>

#include "framework/StringHash.h"
#include "framework/log/Log.h"

class KVStore
{
public:
	enum class SetResult
	{
		Inserted,
		Updated,
		Error
	};

	struct Entry
	{
		std::string value;
	};

	KVStore(std::shared_ptr<Log> log);
	~KVStore();

	std::optional<std::string> Get(std::string_view key);
	KVStore::SetResult Set(std::string_view key, std::string_view value);
	bool Del(std::string_view key);

private:
	std::unordered_map<std::string, KVStore::Entry, StringHash, std::equal_to<>> kvstore;

	mutable std::shared_mutex mtx;

	std::shared_ptr<Log> log;
};

#endif
