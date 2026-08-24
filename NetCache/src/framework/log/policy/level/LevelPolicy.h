#ifndef __NETCACHE_FRAMEWORK_LOG_POLICY_LEVEL_LEVELPOLICY_H__
#define __NETCACHE_FRAMEWORK_LOG_POLICY_LEVEL_LEVELPOLICY_H__

#include "../../entry/Entry.h"

#include "../Policy.h"

/*
* Class: LevelPolicy
* Checks the log level, if its the same as or higher than the set level, then write the log, else skip the write.
*
*	Transform: Checks if the level is the same or higher than the set level, writes if the level is higher
*	GetName: Returns the name of the policy
*/
class LevelPolicy : public Policy
{
public:
	LevelPolicy(Entry::Level level);
	virtual ~LevelPolicy() = default;

	bool Transform(Entry &entry) override final;
	std::string &GetName() override final;

private:
	std::string policyname;

	Entry::Level level;
};

#endif
