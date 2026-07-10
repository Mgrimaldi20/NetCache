#include <utility>

#include "LevelPolicy.h"

LevelPolicy::LevelPolicy(Entry::Level level)
	: policyname("LevelPolicy"),
	level(level)
{
}

bool LevelPolicy::Transform(Entry &entry)
{
	if (std::to_underlying(entry.entrylevel) >= std::to_underlying(level))
		return true;

	return false;
}

std::string &LevelPolicy::GetName()
{
	return policyname;
}
