#pragma once

#include <string>
#include <map>

class StrictGeneratorOptions {
private:
	std::string error_;

public:

	StrictGeneratorOptions
		( const std::string& parameter
		);

	bool hasError
		( std::string* error
		) const;

	const std::map<std::string, std::string> vars
		() const;
};
