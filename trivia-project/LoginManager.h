#pragma once

#include "LoginRequests.h"
#include "IDatabase.h"
#include "Address.h"

#include <memory>
#include <set>


class LoginManager
{
	
public:


	// methods for singleton
	LoginManager(const LoginManager&) = delete;
	LoginManager& operator=(const LoginManager&) = delete;

	static LoginManager& getInstance(std::shared_ptr<IDatabase>& database)
	{
		static LoginManager instance(database);

		return instance;
	}


	// Methods
	void signup(const SignupRequest& signupRequest);
	void login (const LoginRequest& loginRequest);
	void logout(const std::string& name);



private:


	// Constructors
	LoginManager(std::shared_ptr<IDatabase>& database);
	~LoginManager() = default;


	// Input Validation Methods
	void isInputValid(const SignupRequest& input);
	
	bool isPasswordValid(const std::string& password);
	bool isEmailValid(const std::string& email);
	bool isAddressValid(const Address& adress);
	bool isPhoneValid(const std::string& phone);
	bool isDateValid(const std::string& date);


	// Fields
	std::set<std::string> _loggedUsers;
	std::shared_ptr<IDatabase> _database;

};

