#pragma once

#include <string>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include "strict-generator-options.hh"

class StrictGenerator
	: public google::protobuf::compiler::CodeGenerator
{
public:

	bool Generate
		( const google::protobuf::FileDescriptor*        file
		, const std::string&                             parameter
		, google::protobuf::compiler::GeneratorContext*  context
		, std::string*                                   error
		) const override;

};
