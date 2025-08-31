//#include "Mongo.h"
//#include <vector>
//#include <set>
//
//
//using std::set;
//using std::string;
//using std::map;
//using std::remove_const;
//using bsoncxx::builder::stream::document;
//using bsoncxx::builder::stream::finalize;
//using bsoncxx::document::value;
//using bsoncxx::v_noabi::document::view;
//using bsoncxx::v_noabi::document::element;
//using boost::optional;
//
//
//
////? Methods ?//
//
//
///*
//	-- find if the use exists or not by username --
//	* input: user's username
//	* output: exists or not
//*/
//Mongo::Mongo() :
//	_instance(), _connection({ mongocxx::uri { URI } }),
//	_db(_connection[DB_NAME])
//{
//}
//
//
///*
//	-- finds if the use exists or not by username --
//	* input: user's username
//	* output: exists or not
//*/
//bool Mongo::doesUserExists(const string& username)
//{
//	return bool(getUser(username));
//}
//
///*
//	-- find if the given password match the real password of the user --
//	* input: username, password
//	* output: bool: password match or not
//*/
//bool Mongo::doesPasswordMatch(const string& username, const string& password)
//{
//	return bool(_db[USERS_TABLE].find_one(
//		document{}
//		<< USER_NAME_COLUMN << username
//		<< PASSWORD_COLUMN << password
//		<< finalize
//	));
//}
//
//
//
///*
//	-- add user to USERS_TABLE --
//	* input: user's data: name, password, email, etc...
//*/
//void Mongo::addNewUser(const string& name, const string& password,
//	const string& email, const Address& address,
//	const string& phone, const string& date)
//{
//
//	// build new user
//	value newUser = document{}
//		<< USER_NAME_COLUMN << name
//		<< PASSWORD_COLUMN << password
//		<< CITY_COLUMN << address._city
//		<< STREET_COLUMN << address._street
//		<< APT_COLUMN << address._apt
//		<< PHONE_COLUMN << phone
//		<< GAMES_COLUMN << 0
//		<< ANSWERS_COLUMN << 0
//		<< ANSWERS_TIME_COLUMN << 0
//		<< RIGHT_ANSWERS_COLUMN << 0
//		<< RIGHT_ANSWERS_TIME_COLUMN << 0
//		<< finalize;
//
//
//	// insert new user
//	_db[USERS_TABLE].insert_one(newUser.view());
//
//}
//
///* -- get n questions -- */
//set<TriviaQuestion> Mongo::getQuestions(const int& n)
//{
//
//	set<TriviaQuestion> questions;
//	TriviaQuestion helper;
//
//	for (auto&& question : _db[QUESTIONS_TABLE].find({}))
//	{
//
//		helper._question = to_str(question[QUESTION_COLUMN]);
//
//		for (auto& option : question[OPTIONS_COLUMN].get_array().value)
//			helper._options.insert(to_str(option));
//
//		for (auto& answer : question[ANSWERS_COLUMN].get_array().value)
//			helper._answers.insert(to_str(answer));
//
//		questions.insert(helper);
//
//	}
//
//	return questions;
//}
//
///*
//	-- Returns the average time that a user took to answer --
//	* input: username
//	* output: float:  average time that a user took to answer
//*/
//float Mongo::getPlayerAverageAnswerTime(const string& username)
//{
//	float answer_time = (float)queryStat(username, ANSWERS_TIME_COLUMN);
//	// so no division by ;
//
//	return (float)getNumOfTotalAnswers(username) /
//		answer_time ? answer_time : 1;
//
//
//}
//
//int Mongo::getNumOfCorrectAnswers(const string& username)
//{
//	return queryStat(username, ANSWERS_COLUMN);
//}
//
///*
//	-- Returns the amount of answers that a user chose  --
//	* input: username
//	* output: int: amount of answers of a user
//*/
//int Mongo::getNumOfTotalAnswers(const string& username)
//{
//	return queryStat(username, ANSWERS_COLUMN);
//}
//
//
///*
//	-- Returns the amount of games that a user played  --
//	* input: username
//	* output: int: amount of games that a user played
//*/
//int Mongo::getNumOfPlayerGames(const string& username)
//{
//	return queryStat(username, GAMES_COLUMN);
//}
//
///*
//	-- Returns the top players scores with their name  --
//	* input: None
//	* output: map<string, int> :   the top players scores with their name
//*/
//std::multimap<int, string> Mongo::getTopScores()
//{
//
//	const int SCORE_BOARD_LEN = 5;
//	std::multimap<int, string> scores;
//	string username;
//	
//	for (auto&& user : _db[USERS_TABLE].find({}) )
//	{
//
//		username = to_str(user[USER_NAME_COLUMN]);
//	
//		scores.insert( {calcPoints(username), username} );
//	
//	}
//
//	auto start = scores.begin();
//	
//	if (scores.size() > SCORE_BOARD_LEN)
//		std::advance(start, scores.size() - SCORE_BOARD_LEN);
//
//	return { start, scores.end() };
//
//}
//
///*
//	-- claculate the points of the user --
//	* input: username
//	* output: points
//*/
//int Mongo::calcPoints(const string& username)
//{
//
//	int rightAnswers = queryStat(username, RIGHT_ANSWERS_COLUMN),
//		rightAnswersTime = queryStat(username, RIGHT_ANSWERS_TIME_COLUMN),
//		allAnswers = getNumOfCorrectAnswers(username);
//	
//	if (!rightAnswersTime) return 0;
//	
//	try {
//		
//		int res = 5 * (rightAnswers * 1.5 - (allAnswers - rightAnswers)) * 5 / rightAnswersTime;
//		return res < 0 ? 0 : res;
//
//	} catch (...) {
//		return 0;
//	}
//}
//
//
///* -- change element of string type to string -- */
//string Mongo::to_str(const bsoncxx::v_noabi::document::element& element)
//{
//	return string(element.get_utf8().value);
//}
//
///* -- change element of array type to string -- */
//string Mongo::to_str(const bsoncxx::v_noabi::array::element& element)
//{
//	return string(element.get_utf8().value);
//}
//
//
///* -- return a user object by his username -- */
//optional<bsoncxx::v_noabi::document::value> Mongo::getUser(string username)
//{
//	return _db[USERS_TABLE].find_one(
//
//		document{}
//		<< USER_NAME_COLUMN << username
//		<< finalize
//	);
//}
//
///* -- return a viewable user object -- */
//int Mongo::queryStat(string username, string stats_coll)
//{
//	try {
//		return getUser(username).value().view()[stats_coll].get_int32();
//	} catch (...) {}
//	
//	return 0;
//}