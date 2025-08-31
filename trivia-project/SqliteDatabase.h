#pragma once

#include "IDatabase.h"
#include "sqlite3.h"
#include <sstream>
#include <memory>
#include <set>
#include <map>
#include <iostream>

class SqliteDatabase : public IDatabase
{
public:
    // Singleton method
    static std::shared_ptr<IDatabase> getInstance()
    {
        static std::shared_ptr<IDatabase> instance(new SqliteDatabase());
        return instance;
    }

    // Constructors
    SqliteDatabase();
    virtual ~SqliteDatabase() = default;

    // Methods from IDatabase
    virtual bool doesUserExists(const std::string& username) override;
    virtual bool doesPasswordMatch(const std::string& username, const std::string& password) override;
    virtual void addNewUser(const std::string& name,
        const std::string& password,
        const std::string& email,
        const Address& address,
        const std::string& phone,
        const std::string& date) override;

    virtual std::set<TriviaQuestion> getQuestions(const int& numOfQuestions) override;
    virtual float getPlayerAverageAnswerTime(const std::string& username) override;
    virtual int getNumOfCorrectAnswers(const std::string& username) override;
    virtual int getNumOfTotalAnswers(const std::string& username) override;
    virtual int getNumOfPlayerGames(const std::string& username) override;
    virtual std::multimap<int, std::string> getTopScores() override;

    // sqlite callback Methods
    static int booleanCallback(void* boolean, int len, char** data, char** columnName); 
    static int questionsCallback(void* data, int argc, char** argv, char** azColName);
    static int averageAnswerTimeCallback(void* data, int argc, char** argv, char** azColName);
    static int intCallback(void* data, int argc, char** argv, char** azColName);


private:
    bool openDatabase();
    void initializeDB();
    void sqliteRequest(const std::stringstream& sql,
        int(*callback)(void*, int, char**, char**) = nullptr, void* data = nullptr);
    void sqliteRequest(const char* sql,
        int(*callback)(void*, int, char**, char**) = nullptr, void* data = nullptr);

    sqlite3* _db;
};