#ifndef __NETCACHE_APPLICATION_KVSTORE_H__
#define __NETCACHE_APPLICATION_KVSTORE_H__

#include <memory>
#include <flat_map>
#include <string>
#include <shared_mutex>
#include <string_view>
#include <optional>
#include <initializer_list>
#include <utility>

#include "Entry.h"

#include "framework/StringHash.h"
#include "framework/log/Log.h"

class KVStore
{
public:
	KVStore(std::shared_ptr<Log> log);
	~KVStore();

	std::optional<std::string> Get(std::string_view key);
	std::optional<std::string> Get(std::initializer_list<std::string_view> keys);

	template<typename ...Args>
	std::optional<std::string> Get(Args && ...args);

	bool Set(std::string_view key, std::string_view value);
	bool Set(std::initializer_list<std::pair<std::string_view, std::string_view>> elems);

	template<typename ...Args>
	bool Set(Args && ...args);

	bool Del(std::string_view key);
	bool Del(std::initializer_list<std::string_view> keys);

	template<typename ...Args>
	bool Del(Args && ...args);

private:
	std::flat_map<std::string, Entry, StringHash, std::equal_to<>> kvstore;

	mutable std::shared_mutex mtx;

	std::shared_ptr<Log> log;
};

template<typename ...Args>
inline std::optional<std::string> KVStore::Get(Args && ...args)
{
	return Get({ { std::forward<Args>(args)... } });
}

template<typename ...Args>
inline bool KVStore::Set(Args && ...args)
{
	static_assert(sizeof...(Args) % 2 == 0, "Set requires an even number of arguments!");
	return Set({ { std::forward<Args>(args)... } });
}

template<typename ...Args>
inline bool KVStore::Del(Args && ...args)
{
	return Del({ { std::forward<Args>(args)... } });
}

#endif
