#include "StatisticsManager.h"

using std::multimap;
using std::string;



//? Methods ?//


/*
	-- Returns the top 5 players scores with their name  --
	* input: None
	* output: the top 5 players scores with their name
*/
multimap<int, string> StatisticsManager::getTopScores()
{
    return _database->getTopScores();
}
/*
    -- get user's statistics --
    * input: user's username
    * output: set of the statistics
*/
Stats StatisticsManager::getUserStatistics(const string& username)
{
    
    return { _database->getPlayerAverageAnswerTime(username),
             _database->getNumOfCorrectAnswers(username),
             _database->getNumOfTotalAnswers(username),
             _database->getNumOfPlayerGames(username)
    };
}