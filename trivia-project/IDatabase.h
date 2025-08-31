#pragma once

#include "Address.h"
#include "TriviaQuestion.h"
#include <string>
#include <map>
#include <set>

class IDatabase
{

public:
	
	// D'tor
	virtual ~IDatabase() = default;


	// Methods

	//  users Table Methods 
	virtual bool doesUserExists(const std::string& name) = 0;
	virtual bool doesPasswordMatch( const std::string& username, 
									const std::string& password) = 0;
	virtual void addNewUser( const std::string& name,
							 const std::string& password,	
							 const std::string& email,
							 const Address& address,
							 const std::string& phone,
							 const std::string& date) = 0;
	
	//  questions Table Methods
	virtual std::set<TriviaQuestion> getQuestions(const int& numOfQuestions) = 0;
	
	//  statistics Table Methods 
	virtual float getPlayerAverageAnswerTime(const std::string& username) = 0;
	virtual int	getNumOfCorrectAnswers(const std::string& username) = 0;
	virtual int getNumOfTotalAnswers(const std::string& username) = 0;
	virtual int getNumOfPlayerGames(const std::string& username) = 0;
	virtual std::multimap<int, std::string> getTopScores() = 0;


	// Database
	const std::string DB_NAME = "triviaDB";

	// users Table
	const std::string USERS_TABLE = "users";
	const std::string USER_NAME_COLUMN = "username";
	const std::string PASSWORD_COLUMN = "password";
	const std::string EMAIL_COLUMN = "email";
	const std::string CITY_COLUMN = "city";
	const std::string STREET_COLUMN = "street";
	const std::string APT_COLUMN = "apt";
	const std::string PHONE_COLUMN = "phone";
	const std::string DATE_COLUMN = "date";
	const std::string GAMES_COLUMN = "games";
	const std::string ANSWERS_COLUMN = "answer";
	const std::string RIGHT_ANSWERS_COLUMN = "right_answers";
	const std::string ANSWERS_TIME_COLUMN = "answers_time";
	const std::string RIGHT_ANSWERS_TIME_COLUMN = "right_answers_time";

	// questions Table
	const std::string QUESTIONS_TABLE = "questions";
	const std::string QUESTION_COLUMN = "question";
	const std::string OPTION_COLUMN = "option";

	//ANSWERS_COLUMN - already exists

protected:

	// Constructors
	IDatabase() = default;
};