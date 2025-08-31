//#include "IDatabase.h"
//
//
//#include <mongocxx/client.hpp>
//#include <mongocxx/instance.hpp>
//#include <bsoncxx/builder/stream/helpers.hpp>
//#include <bsoncxx/builder/stream/document.hpp>
//#include <bsoncxx/builder/stream/array.hpp>
//#include <iostream>
//#include <memory>
//#include <set>
//
//
//class Mongo : public IDatabase
//{
//
//
//public:
//
//	// Constructors
//	Mongo();
//	virtual ~Mongo() = default;
//
//
//	// methods for singleton
//	Mongo(const Mongo&) = delete;
//	Mongo& operator=(const Mongo&) = delete;
//
//	// methods for singleton
//	static std::shared_ptr<IDatabase> getInstance()
//	{
//		static std::shared_ptr<IDatabase> instance(new Mongo());
//
//		return instance;
//	}
//
//
//	// Methods
//
//	//  users Table Methods
//	virtual bool doesUserExists(const std::string& username) override;
//
//	virtual bool doesPasswordMatch(const std::string& username,
//		const std::string& password) override;
//
//	virtual void addNewUser(const std::string& name,
//		const std::string& password,
//		const std::string& email,
//		const Address& address,
//		const std::string& phone,
//		const std::string& date) override;
//
//
//	
//	virtual std::set<TriviaQuestion> getQuestions(const int& numOfQuestions) override;
//
//	//  statistics Table Methods 
//	virtual float getPlayerAverageAnswerTime(const std::string& username) override;
//	virtual int	getNumOfCorrectAnswers(const std::string& username) override;
//	virtual int getNumOfTotalAnswers(const std::string& username) override;
//	virtual int getNumOfPlayerGames(const std::string& username) override;
//	virtual std::multimap<int, std::string> getTopScores() override;
//	int calcPoints(const std::string& username);
//
//
//private:
//
//	const char* URI = "mongodb+srv://cpp-api:123qwe123qwe@cpptrivia.gmgdi.mongodb.net/Trivia?retryWrites=true&w=majority";
//
//	mongocxx::instance _instance;
//	mongocxx::client _connection;
//	mongocxx::database _db;
//
//	std::string to_str(const bsoncxx::v_noabi::document::element& element);
//	std::string to_str(const bsoncxx::v_noabi::array::element& element);
//
//	boost::optional<bsoncxx::v_noabi::document::value> getUser(std::string username);
//	int queryStat(std::string username, std::string stats_coll);
//};