#pragma once

#include <vector>
#include <string>
#include <set>

struct TriviaQuestion
{
	std::string _question;
	std::set<std::string> _options;
	std::set<std::string> _answers;

	bool operator<(const TriviaQuestion& other) const { return _question.size() < other._question.size(); };

};