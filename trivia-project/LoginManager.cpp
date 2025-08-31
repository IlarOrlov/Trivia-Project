#include "LoginManager.h"
#include "LoginException.h"
#include <iostream>
#include <regex>

using std::string;
using std::shared_ptr;
using std::regex_match;



//? Constructors ?//


/* -- C'tor -- */
LoginManager::LoginManager(shared_ptr<IDatabase>& database) :
	_database(database)
{
}




//? Methods ?//


/* -- sign up with username, password and email -- */
void LoginManager::signup(const SignupRequest& signupRequest)
{

	isInputValid(signupRequest);

	if (_database->doesUserExists(signupRequest._username))
		throw LoginException("username already exists");

	// add new user to database & _loggedUsers list
	_loggedUsers.insert(signupRequest._username);
	_database->addNewUser(signupRequest._username,
						  signupRequest._password,
						  signupRequest._email,
						  signupRequest._address,
						  signupRequest._phone,
						  signupRequest._date);

}

/* -- login with username and password -- */
void LoginManager::login(const LoginRequest& loginRequest)
{

	if (!_database->doesPasswordMatch(loginRequest._username, loginRequest._password))
		throw LoginException("invalid password/username");
		
	if (!_loggedUsers.insert(loginRequest._username).second)
		throw LoginException("user is already active");
}


/* -- logout a user with the given name -- */
void LoginManager::logout(const string& name)
{

	if (!_loggedUsers.erase(name))
		throw LoginException("username doesn't exist");

}


//? SignUp validation Methods ?//


/* -- checks if ALL input is valid -- */
void LoginManager::isInputValid(const SignupRequest& input)
{
	string err = "";

	if (!isPasswordValid(input._password))
		err = "password is invalid, must constain: 8+ chars 1+ number, uppercase, lowercase and unique";
	else if (input._username == "")
		err = "username can't be empty";
	else if ( !isEmailValid(input._email) )
		err = "email is invalid, must be in a regular email format";
	else if ( !isAddressValid(input._address) ) 
		err = "address is invalid, street & city must include no digit and apt vice versa";
	else if ( !isPhoneValid(input._phone) )
		err = "phone is invalid, must start with 0, include '-' after first two/three digits and 9-10 digits in total";
	else if ( !isDateValid(input._date) )
		err = "date is invalid, format: <DD><'.' or '/'><MM><'.' or '/'><YYYY>";
	
	else if (_database->doesUserExists(input._username))
		err = "Username Already Exists";


	if (!err.empty()) throw LoginException(err);

}

/* -- checks if the password is valid -- */
bool LoginManager::isPasswordValid(const string& password)
{

	// Password must contain 8 characters and at least one number,
	// one letter and one unique
	std::regex regex(R"((?=.*\d.*)(?=.*[a-zA-Z].*)(?=.*[\!\@\#\$\%\^\&\*].*).{8,})");

	return regex_match(password, regex);

}

/* -- checks if the email is valid -- */
bool LoginManager::isEmailValid(const string& email)
{
	// FORMAT: <a-zA-Z><@><a-zA-Z><.><a-zA-Z><.>{0,}<a-zA-Z>{0,}
	std::regex regex(R"([a-zA-Z0-9]+@[a-zA-Z0-9]+[.][a-zA-Z0-9]+([.][a-zA-Z0-9]+)?)");
	
	return regex_match(email, regex);

}

/* -- checks if the adress is valid -- */
bool LoginManager::isAddressValid(const Address& adress)
{

	std::regex onlyLettersRegex(R"([a-zA-Z]+)");
	std::regex onlyDigitRegex(R"([0-9]+)");

	return	regex_match(adress._street, onlyLettersRegex) &&
			regex_match(adress._apt, onlyDigitRegex) &&
			regex_match(adress._city, onlyLettersRegex);
}

/* -- checks if the phone number is valid -- */
bool LoginManager::isPhoneValid(const string& phone)
{
	
	// FORMAT: <0><0-9>{1-2}<-><0-9>{7-9}
	return regex_match(phone, std::regex(R"(^0[\d]{1,2}[-][\d]{7,9}$)"));

}

/* -- checks if the date is valid -- */
bool LoginManager::isDateValid(const string& date)
{
	// FORMAT: <DD>[.|/]<MM>[.|/]<YYYY>
	std::regex regex(R"(^[\d]{2}[.|/][\d]{2}[.|/][\d]{4}$)");
	
	return regex_match(date, regex);

}


