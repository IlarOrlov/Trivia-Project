#pragma once

#include "IDatabase.h"
#include "Stats.h"
#include <memory>

class StatisticsManager
{

public:
	
	// D'tor
	virtual ~StatisticsManager() = default;


	// methods for singleton
	StatisticsManager(const StatisticsManager&) = delete;
	StatisticsManager& operator=(const StatisticsManager&) = delete;

	static StatisticsManager& getInstance(std::shared_ptr<IDatabase>& database)
	{
		static StatisticsManager instance(database);

		return instance;
	}


	// Methods 
	std::multimap<int, std::string> getTopScores();
	Stats getUserStatistics(const std::string& username);

private:

	// C'tor
	StatisticsManager(std::shared_ptr<IDatabase>& database) : _database(database) {};

	// Fields
	std::shared_ptr<IDatabase> _database;

};

