#include "SqliteDatabase.h"
#include <vector>
#include <set>
#include <iostream>
#include "SqliteException.h"
#include "TriviaQuestion.h"
#include <io.h>


using std::string;
using std::stringstream;



//? Constructors ?//


/* -- C'tor -- */
SqliteDatabase::SqliteDatabase() : 
	_db(nullptr)
{
		
	// before openning/creating Database, find if the db file exist
	bool isDBAlreadyExist = !_access(string(DB_NAME + ".sqlite").c_str(), 0);
	
	if (!openDatabase())
		throw SqliteException("Can't create/connet to sqlite database");

	if (!isDBAlreadyExist)
		initializeDB();
}


//? Methods ?//


/* 
	-- find if the use exists or not by username --
	* input: user's username
	* output: exists or not
*/
bool SqliteDatabase::doesUserExists(const string& username)
{
	stringstream sql;
	bool exists = false;

	sql << "SELECT COUNT() FROM " << USERS_TABLE << " WHERE "
		<< USER_NAME_COLUMN << " = '" << username << "';";

	sqliteRequest(sql, booleanCallback, &exists);

	return exists;
}

/*
	-- find if the given password match the real password of the user --
	* input: username, password
	* output: bool: password match or not
*/
bool SqliteDatabase::doesPasswordMatch(const string& username,
	const string& password)
{
	stringstream sql;
	bool match = false;

	sql << "SELECT COUNT() FROM " << USERS_TABLE << " WHERE "
		<< USER_NAME_COLUMN << " = '" << username << "' AND "
		<< PASSWORD_COLUMN << " = '" << password << "';";
	
	sqliteRequest(sql, booleanCallback, &match);
	return match;
}


/*
	-- add user to USERS_TABLE --
	* input: user's data: name, password, email, etc...
*/
void SqliteDatabase::addNewUser(const string& name, const string& password,
	const string& email, const Address& address, const string& phone,
	const string& date)
{

	stringstream sql;
	
	sql << "INSERT INTO " << USERS_TABLE << " ( "
		<< USER_NAME_COLUMN << ", " << PASSWORD_COLUMN << ", " 
		<< EMAIL_COLUMN << ", " << CITY_COLUMN << ", " << STREET_COLUMN << ", " 
		<< APT_COLUMN << ", " << PHONE_COLUMN << ", " << DATE_COLUMN 
		<< " ) VALUES ( '" 
		<< name << "', '" << password << "', '" << email << "', '" 
		<< address._city  << "', '" << address._street << "', '" 
		<< address._apt << "', '" << phone << "', '" << date << "' );";

	sqliteRequest(sql);
}

std::set<TriviaQuestion> SqliteDatabase::getQuestions(const int& numOfQuestions) {
	std::set<TriviaQuestion> questions;
	TriviaQuestion helper;
	stringstream sql;

	sql << "SELECT " << QUESTION_COLUMN << ", " << OPTION_COLUMN << "1, " << OPTION_COLUMN << "2, " << OPTION_COLUMN << "3, " << OPTION_COLUMN << "4, " << ANSWERS_COLUMN <<
		" FROM " << QUESTIONS_TABLE << " LIMIT " << std::to_string(numOfQuestions);

	sqliteRequest(sql, questionsCallback, &questions);

	return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username) {
	std::stringstream sql;
	sql << "SELECT answers_time FROM " << USERS_TABLE << " WHERE username = '" << username << "'";

	float averageTime = 0.0f;
	sqliteRequest(sql, averageAnswerTimeCallback, &averageTime);

	return averageTime;
}

// Function to get the number of correct answers for a user
int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username) {
	std::stringstream sql;
	sql << "SELECT " << RIGHT_ANSWERS_COLUMN << " FROM " << USERS_TABLE << " WHERE " << USER_NAME_COLUMN << " = '" << username << "'";

	int numCorrectAnswers = 0;
	sqliteRequest(sql, intCallback, &numCorrectAnswers);

	return numCorrectAnswers;
}

// Function to get the total number of answers for a user
int SqliteDatabase::getNumOfTotalAnswers(const std::string& username) {
	std::stringstream sql;
	sql << "SELECT " << ANSWERS_COLUMN << " FROM " << USERS_TABLE << " WHERE " << USER_NAME_COLUMN << " = '" << username << "'";

	int numTotalAnswers = 0;
	sqliteRequest(sql, intCallback, &numTotalAnswers);

	return numTotalAnswers;
}

// Function to get the number of games played by a user
int SqliteDatabase::getNumOfPlayerGames(const std::string& username) {
	std::stringstream sql;
	sql << "SELECT " << GAMES_COLUMN << " FROM " << USERS_TABLE << " WHERE " << USER_NAME_COLUMN << " = '" << username << "'";

	int numPlayerGames = 0;
	sqliteRequest(sql, intCallback, &numPlayerGames);

	return numPlayerGames;
}

std::multimap<int, std::string> SqliteDatabase::getTopScores() {

		const int SCORE_BOARD_LEN = 5;
		std::multimap<int, string> scores;
		string username;
	
		auto start = scores.begin();
		
		if (scores.size() > SCORE_BOARD_LEN)
			std::advance(start, scores.size() - SCORE_BOARD_LEN);
	
		return { start, scores.end() };
}

//? Private Methods ?//


/*
	-- try openning the database --
	* input:
*/
bool SqliteDatabase::openDatabase()
{
	return !sqlite3_open(string(DB_NAME + ".sqlite").c_str(), &_db);
}

/* -- initialize Database - create new USER_TABLE -- */
void SqliteDatabase::initializeDB()
{
	stringstream sql;

	sql << "CREATE TABLE " << USERS_TABLE << " ( "
		<< USER_NAME_COLUMN << " TEXT PRIMARY KEY NOT NULL, "
		<< PASSWORD_COLUMN << " TEXT NOT NULL, "
		<< EMAIL_COLUMN << " TEXT NOT NULL, "
		<< CITY_COLUMN << " TEXT NOT NULL, "
		<< STREET_COLUMN << " TEXT NOT NULL, "
		<< PHONE_COLUMN << " TEXT NOT NULL, "
		<< DATE_COLUMN << " TEXT NOT NULL );";
	
	sqliteRequest(sql);

}

/*
   -- execute sql request --
   * input: sql command stringstream, callback function,
			data to pass to callback function
*/
void SqliteDatabase::sqliteRequest(const stringstream& sql,
	int(*callback)(void*, int, char**, char**), void* data)
{
	sqliteRequest(sql.str().c_str(), callback, data);
}

/*
  -- execute sql request --
  * input: sql command, callback function,
   data to pass to callback function
*/
void SqliteDatabase::sqliteRequest(const char* sql,
	int(*callback)(void*, int, char**, char**), void* data)
{
	char* errMsg = nullptr;
	if (sqlite3_exec(_db, sql, callback, data, &errMsg))
		throw SqliteException(errMsg);
}


/* Sqlite Boolean callback Function */
int SqliteDatabase::booleanCallback(void* boolean, int len, char** data, char** columnName)
{
	bool* b = static_cast<bool*>(boolean);

	*b = atoi(data[0]);
	
	return 0;
}

int SqliteDatabase::questionsCallback(void* data, int argc, char** argv, char** azColName) {
	std::set<TriviaQuestion>* questions = static_cast<std::set<TriviaQuestion>*>(data);
	TriviaQuestion tq;
	tq._question = argv[0] ? argv[0] : "";
	for (int i = 1; i <= 4; ++i) {
		if (argv[i]) tq._options.insert(argv[i]);
	}
	tq._answers.insert(argv[5] ? argv[5] : "");
	questions->insert(tq);
	return 0;
}

int SqliteDatabase::averageAnswerTimeCallback(void* data, int argc, char** argv, char** azColName) {
	if (argc == 1 && argv[0]) {
		float* averageTime = static_cast<float*>(data);
		*averageTime = std::stof(argv[0]);
	}
	return 0;
}

int SqliteDatabase::intCallback(void* data, int argc, char** argv, char** azColName) {
	if (argc == 1 && argv[0]) {
		int* result = static_cast<int*>(data);
		*result = std::stoi(argv[0]);
	}
	return 0;
}