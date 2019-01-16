#include "strict-generator-options.hh"

#include <algorithm>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>

using google::protobuf::compiler::ParseGeneratorParameter;

StrictGeneratorOptions::StrictGeneratorOptions
	( const std::string& parameter
	)
{
	// std::vector<std::pair<std::string, std::string>> options;
	// ParseGeneratorParameter(parameter, &options);

	// for(const auto& option : options) {
	//   const std::string& optKey = option.first;
	//   const std::string& optValue = option.second;

	//   // @TODO: Add parameter parsing
	// }
}

bool StrictGeneratorOptions::hasError
	( std::string* error
	) const
{
	if(!error_.empty()) {
		(*error) = error_;
		return true;
	}

	return false;
}

const std::map<std::string, std::string> StrictGeneratorOptions::vars
	(
	) const
{
	return {};
}
