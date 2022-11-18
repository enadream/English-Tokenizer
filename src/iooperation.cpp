#include <fstream>      // std::fstream
#include <stdio.h>

// User defined libs
#include "iooperation.hpp"
#include "datatypes.hpp"
#include "log.hpp"
#include "utility.hpp"
#include "textdata.hpp"

#include "verb.hpp"

#define COMMAND_LINE_LENGTH 300
#define PARAMETER_VALUE_LENGTH 100
#define IOO_BUFFER_SIZE 100000
#define IOO_SIZE_INCREASE_COEFFICIENT 2


IOOperation::IOOperation() {
	io_buffer = nullptr;
	buffer_capacity = 0;
}

IOOperation::~IOOperation() {
	DeleteBuffer();
}

inline void IOOperation::DeleteBuffer() {
	delete[] io_buffer; // Free buffer area
	buffer_capacity = 0; // Update capacity
}

void IOOperation::IncreaseCapacity() {
	if (buffer_capacity > 0) { // If buffer already initialized and contains data
		// copy old pointer
		char* p_old_buffer = io_buffer;
		// create new heap area
		io_buffer = new char[buffer_capacity * IOO_SIZE_INCREASE_COEFFICIENT];
		// copy values 
		CopyData(*io_buffer, *p_old_buffer, buffer_capacity);
		// Increase new capacity
		buffer_capacity *= IOO_SIZE_INCREASE_COEFFICIENT;
		// free old heap area
		delete[] p_old_buffer;
	}
	else {
		io_buffer = new char[IOO_BUFFER_SIZE];
		buffer_capacity = IOO_BUFFER_SIZE;
	}
}


void IOOperation::StartCLI() {
	char line[COMMAND_LINE_LENGTH]; // Console max read size
	char params_and_data[COMMAND_LINE_LENGTH];
	char param_value[PARAMETER_VALUE_LENGTH];
	char param_value1[PARAMETER_VALUE_LENGTH];
	char param_value2[PARAMETER_VALUE_LENGTH];

	verb::VerbHandler verb;

	auto printHeader = []() {
		SetColor(3);
		std::cout << "__________________ Welcome to the ChatBot (V0.3 @enadream) __________________\n\n" << std::endl;
		SetColor(7);
	};

	printHeader();

	while (true) {
		SetColor(3);
		std::cout << "ECLI $ ";
		SetColor(7);
		std::cin.getline(line, COMMAND_LINE_LENGTH);

		//if (line[0] != '/') { // Error commands have to start with '/' letter
		//	Log::Error("Command Line has to start with '/' char.") << "\n\n";
		//	continue;
		//}

		if (DoesContain(*"exit ", line[0])) {
			break;
		}
		else if (DoesContain(*"delete ", line[0])) {
			ParseParameters(line[7], *params_and_data); // Parse all parameters

			if (DoesContainParameter(*"command_parse", *params_and_data, *param_value))
				Log::Info("COMMAND PARSE: ") << params_and_data << "\n";
			if (DoesContainParameter(*"verb", *params_and_data, *param_value)) {
				if (DoesContainParameter(*"all", *params_and_data, *param_value)) {
					if (verb.DeleteAll() == 0)
						Log::Info("All the verbs deleted successfully.\n");
				}
			}
			if (DoesContainParameter(*"io_buffer", *params_and_data, *param_value)) {
				DeleteBuffer();
				Log::Info("IO buffer deleted successfully.\n");
			}

			std::cout << "\n";
		}
		else if (DoesContain(*"read ", line[0])) {
			ParseParameters(line[5], *params_and_data); // Parse all parameters

			if (DoesContainParameter(*"command_parse", *params_and_data, *param_value))
				Log::Info("COMMAND PARSE: ") << params_and_data << "\n";
			if (DoesContainParameter(*"dir", *params_and_data, *param_value)) {

				if (DoesContainParameter(*"all", *params_and_data, *param_value1)) {

				}
				else if (DoesContainParameter(*"verb", *params_and_data, *param_value1)) {
					if (ReadFromDisk(*param_value) == 0) {
						verb.MultipleVerbAdder(*io_buffer);
					}
				}
				else if (DoesContainParameter(*"noun", *params_and_data, *param_value1)) {

				}
				else {
					Log::Warning("No type parameter found in the command!\n");
				}
			}
			else {
				Log::Warning("No -dir parameter found in the command!\n");
			}
			std::cout << "\n";
		}
		else if (DoesContain(*"parse ", line[0])) { // /parse
			ParseParameters(line[6], *params_and_data); // Parse all parameters

			if (DoesContainParameter(*"command_parse", *params_and_data, *param_value))
				Log::Info("COMMAND PARSE: ") << params_and_data << "\n";

			if (DoesContainParameter(*"verb", *params_and_data, *param_value)) {
				if (buffer_capacity == 0) { // Capacity Check
					IncreaseCapacity();
				}
				switch (verb.ParseVerb(*param_value, *io_buffer, true))
				{
				case 1:
					Log::Info("Verb has found:\n") << io_buffer;
					break;
				case 2:
					Log::Warning("The input is empty.\n");
					break;
				case -1:
					Log::Warning("No verb has found.\n");
					break;
				case -2:
					Log::Warning("The input contains some characters which is not alphabetic.\n");
					break;
				case -3:
					Log::Warning("The first character of the verb cannot be hyphen.\n");
					break;
				case -4:
					Log::Warning("The verb cannot be less than 2 characters.\n");
					break;
				case -5:
					Log::Warning("Character size exceeds ") << VERB_CHAR_SIZE << " size.\n";
					break;
				default:
					break;
				}
			}
			else if (DoesContainParameter(*"noun", *params_and_data, *param_value)) {
				Log::Info("NOUN: ") << param_value << "\n";
			}


			std::cout << "\n";
		}
		else if (DoesContain(*"find ", line[0])) { // /find
			ParseParameters(line[5], *params_and_data); // Parse all parameters

			if (DoesContainParameter(*"command_parse", *params_and_data, *param_value)) // Show Cli
				Log::Info("COMMAND PARSE: ") << params_and_data << "\n";
			if (DoesContainParameter(*"verb", *params_and_data, *param_value)) {
				if (buffer_capacity == 0) { // Capacity Check
					IncreaseCapacity();
				}
				switch (verb.ParseVerb(*param_value, *io_buffer))
				{
				case 1:
					Log::Info("Verb has found:\n") << io_buffer;
					break;
				case 2:
					Log::Warning("The input is empty.\n");
					break;
				case -1:
					Log::Warning("No verb has found.\n");
					break;
				case -2:
					Log::Warning("The input contains some characters which is not alphabetic.\n");
					break;
				case -3:
					Log::Warning("The first character of the verb cannot be hyphen.\n");
					break;
				case -4:
					Log::Warning("The verb cannot be less than 2 characters.\n");
					break;
				case -5:
					Log::Warning("Character size exceeds ") << VERB_CHAR_SIZE << " size.\n";
					break;
				default:
					break;
				}
			}

			std::cout << "\n";
		}
		else if (DoesContain(*"print ", line[0])) {
			ParseParameters(line[6], *params_and_data); // Parse all parameters

			if (DoesContainParameter(*"command_parse", *params_and_data, *param_value)) {
				Log::Info("COMMAND PARSE: ") << params_and_data << "\n";
			}
			if (DoesContainParameter(*"io_buffer", *params_and_data, *param_value)) {
				if (buffer_capacity > 0) {
					Log::Info("IO BUFFER:\n") << io_buffer << "\n";
				}
				else {
					Log::Info("IO BUFFER CAPACITY:") << buffer_capacity << "\n";
					Log::Warning("IO BUFFER is empty.\n");
				}
			}
			else if (DoesContainParameter(*"irregulars", *params_and_data, *param_value)) {
				if (buffer_capacity == 0) { // Capacity Check
					IncreaseCapacity();
				}
				if (verb.GetAllIrregularVerbs(*io_buffer) > 0)
					Log::Info("[IRREGULAR VERB LIST]: \n") << io_buffer << "\n";
				else
					Log::Warning("No irregular verb has found.\n");
			}
			std::cout << "\n";
		}
		else if (DoesContain(*"clear ", line[0])) {
			system("CLS");
			printHeader();
		}
		else if (DoesContain(*"help ", line[0])) {
			ParseParameters(line[5], *params_and_data, '/'); // Parse all parameters

			if (DoesContainParameter(*"tenses", *params_and_data, *param_value)) {
				Log::Info("Time Tenses :\n") << HELP_TIME_TENSES << "\n";
			}
			else {
				Log::Info("All the existing commands are; \n") << HELP_MAIN << "\n";
			}

		}
		else {
			Log::Error(HELP_ERROR_COMMAND_NFOUND) << "\n";
		}
	}
}

int8 IOOperation::ReadFromDisk(const char& file_name) {
	FILE* file_ptr;
	file_ptr = fopen(&file_name, "r");

	if (file_ptr == NULL)
	{
		Log::Error("No such file found with the name \"") << &file_name << "\"\n";
		if (buffer_capacity > 0)
			DeleteBuffer();
		return -1;
	}

	uint32 lastIndex = -1;
	do {
		lastIndex += 1;
		if (lastIndex + 1 > buffer_capacity)  // If capacity is not enough
			IncreaseCapacity();

		// Increase Last index and copy character
		io_buffer[lastIndex] = fgetc(file_ptr);

		// If it is EOF stop reading.
	} while (io_buffer[lastIndex] != EOF);

	// Change ending char
	io_buffer[lastIndex] = '\0';

	// Close the file
	fclose(file_ptr);

	Log::Info("The file has readed successfully.\n");
	return 0;
}

void IOOperation::WriteToDisk(const char& in_file) const {
	std::fstream fs;
	fs.open("output.txt", std::fstream::out); // std::fstream::in | std::fstream::out | std::fstream::app
	fs << in_file;
	fs.close();
}

void IOOperation::ParseParameters(const char& line, char& out_params,
	const char param_char) const { // i.e; t16/sv/n="Alice"/v="copy"/\0

	bool open_writing = false;
	bool bracket_opened = false;
	int lastIndex = 0;

	for (int i = 0; i < COMMAND_LINE_LENGTH; i++) {
		if ((&line)[i] == '\0')
			break;

		if ((&line)[i] == param_char) { // param_char default is '-'
			if (open_writing) {
				if (bracket_opened) { // When you got the - inside of brackets
					(&out_params)[lastIndex++] = (&line)[i];
				}
			}
			else
				open_writing = true;
		}
		else if ((&line)[i] == ' ') {
			if (open_writing) {
				if (bracket_opened) { // Filling the value of parameter
					(&out_params)[lastIndex++] = (&line)[i];
				}
				else {
					open_writing = false;
					(&out_params)[lastIndex++] = '/';
				}
			}
		}
		else if ((&line)[i] == '"') {
			if (!bracket_opened) { // brackets has just appeared
				bracket_opened = true;

				if (lastIndex < 1) { // The last index has to be bigger than 0
					break;
				}

				if (open_writing) {// User forget to put space between value and param i.e -v"run"
					(&out_params)[lastIndex++] = '=';
				}
				else { // User put space between value and parameter
					(&out_params)[lastIndex - 1] = '=';
				}

				(&out_params)[lastIndex++] = '"';
				open_writing = true;
			}
			else { // End brackets
				(&out_params)[lastIndex++] = '"';
				(&out_params)[lastIndex++] = '/';
				bracket_opened = false;
				open_writing = false;
			}
		}
		else if (open_writing)
			(&out_params)[lastIndex++] = (&line)[i];

	}

	if (bracket_opened && lastIndex > 0) { // When the user forget to put end brackets
		(&out_params)[lastIndex++] = '"';
	}
	else if (open_writing) {
		(&out_params)[lastIndex++] = '/';
	}

	(&out_params)[lastIndex++] = '\0';
}

bool IOOperation::DoesContainParameter(const char& param, const char& parameters, char& out_param_value) const {
	bool startChecking = true;
	int param_index = 0;
	uint32 length = SizeOfStr(parameters); // Get the size of str

	// i.e param; t16\0 , i.e parameters; t16/sv/n="Alice"/v="copy"/\0  
	for (uint32 i = 0; i < length; i++) {
		if (startChecking) {
			if ((&param)[param_index] != '\0' && (&param)[param_index] == (&parameters)[i]) {
				param_index += 1;
			}
			else if ((&param)[param_index] == '\0') {
				if ((&parameters)[i] == '/' || (&parameters)[i] == '\0') {// parameter without value i.e "sv/"
					(&out_param_value)[0] = '\0'; // Parameter doesn't have value
					return true;
				}
				else if ((&parameters)[i] == '=') // parameter with value i.e n="Alice"
				{
					int index = 0;
					for (; (&parameters)[index + i + 2] != '"' && index < PARAMETER_VALUE_LENGTH; index++)
						(&out_param_value)[index] = (&parameters)[index + i + 2];
					(&out_param_value)[index] = '\0';
					return true;
				}
				else {
					startChecking = false;
					param_index = 0;
				}
			}
			else
			{
				startChecking = false;
				param_index = 0;
			}
		}
		else {
			if ((&parameters)[i] == '/') {
				startChecking = true;
			}
		}
	}

	return false;
}


