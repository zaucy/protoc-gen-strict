#include <google/protobuf/descriptor.pb.h>

#include "strict.pb.h"

#include "strict-generator.hh"
#include "strict-generator-options.hh"

using google::protobuf::FieldDescriptor;

bool checkMessageBinaryCompatibility
	( const google::protobuf::Descriptor* target
	, const google::protobuf::Descriptor* constrained
	, std::string* error
	)
{
	const auto targetFieldsCount = target->field_count();

	for(auto i=0; targetFieldsCount > i; ++i) {
		const auto& targetField = target->field(i);
		auto targetFieldNumber = targetField->number();
		auto constrainedField = constrained->FindFieldByNumber(targetFieldNumber);

		if(!constrainedField) {
			*error = "Constraint breached (binary_compatibility). Field number does not exist";
			return false;
		}

		auto targetFieldType = targetField->type();
		auto constrainedFieldType = constrainedField->type();

		if(targetFieldType != constrainedFieldType) {
			*error = "Constraint breached (binary_compatibility). Field number types do not match";
			return false;
		}

		// Additional checks will have to be done if we're a nested message.
		if(targetFieldType == FieldDescriptor::Type::TYPE_MESSAGE) {
			auto nestedTargetMessage = targetField->message_type();
			auto nestedConstrainedMessage = constrainedField->message_type();

			auto nestedCheckPass = checkMessageBinaryCompatibility(
				nestedTargetMessage,
				nestedConstrainedMessage,
				error
			);

			if(!nestedCheckPass) {
				return false;
			}
		}
	}

	return true;
}

bool checkMessageFieldNamesMatch
	( const google::protobuf::Descriptor* target
	, const google::protobuf::Descriptor* constrained
	, std::string* error
	)
{

	auto targetFieldCount = target->field_count();

	for(auto i=0; targetFieldCount > i; ++i) {
		auto field = target->field(i);
		const auto& fieldName = field->name();

		auto constrainedField = constrained->FindFieldByName(fieldName);

		if(!constrainedField) {
			*error = "Constraint breached. Field name '" + fieldName + "' is not present in '" + constrained->full_name() + "'";
			return false;
		}
	}

	return true;
}

bool StrictGenerator::Generate
	( const google::protobuf::FileDescriptor*        file
	, const std::string&                             parameter
	, google::protobuf::compiler::GeneratorContext*  context
	, std::string*                                   error
	) const
{

	StrictGeneratorOptions options(parameter);

	if(options.hasError(error)) {
		return false;
	}

	auto extensionCount = file->extension_count();

	for(auto i=0; extensionCount > i; ++i) {
		auto extension = file->extension(i);
	}

	auto messageTypeCount = file->message_type_count();

	for(auto i=0; messageTypeCount > i; ++i) {
		auto messageType = file->message_type(i);
		auto messageOptions = messageType->options();

		if(messageOptions.HasExtension(strict::fields)) {
			auto strictFieldOptions = messageOptions.GetExtension(strict::fields);

			const auto& messageName = strictFieldOptions.message();
			auto targetMessage = file->FindMessageTypeByName(messageName);

			if(!targetMessage) {
				*error = "Strict message target '" + messageName + "' could not be found";
				return false;
			}

			bool binaryCompatible = false;

			if(strictFieldOptions.has_binary_compatible()) {
				binaryCompatible = strictFieldOptions.binary_compatible();
			}

			bool checkPassed = false;

			if(binaryCompatible) {
				checkPassed = checkMessageBinaryCompatibility(
					targetMessage,
					messageType,
					error
				);

				if(!checkPassed) {
					return false;
				}
			}

			checkPassed = checkMessageFieldNamesMatch(
				targetMessage,
				messageType,
				error
			);

			if(!checkPassed) {
				return false;
			}
		}
	}

	return true;
}
