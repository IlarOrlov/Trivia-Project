#pragma once

#include "Stats.h"
#include <string>
#include <map>


struct getHighScoreResponse
{
	std::multimap<int, std::string> _statistics;
};

struct getPersonalStatsResponse
{
	Stats _statistics;
};