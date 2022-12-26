
// User defined libs
#include "ecli.hpp"
#include "log.hpp"
#include "buffer.hpp"
#include "file_system.hpp"
#include "main_handler.hpp"

#include "misc/string.hpp"
#include "misc/data_types.hpp"
#include "text_data.hpp"

#define COMMAND_LINE_LENGTH 5000
#define MAX_PARAMETER_AMOUNT 10

void Ecli::Start() {
	String line(COMMAND_LINE_LENGTH); // Console max read size
	Parameter params[MAX_PARAMETER_AMOUNT];

	handle::MainHandler mainHandler;

	Buffer buffer;

	int8 lastParamId = 0;
	int8 commandAmount = 0;

	auto resetParams = [&](int8 amount) {
		for (int8 i = 0; i < amount; i++) {
			params[i].name.SetLength(0);
			params[i].value.SetLength(0);
		}
	};

	auto printHeader = []() {
		SetColor(3);
		std::cout << "__________________ Welcome to the ChatBot (V0.50 @enadream) __________________\n\n" << std::endl;
		SetColor(7);
	};

	printHeader();

	// Set length as capacity
	line.SetLength(line.Capacity());

	while (true) {
		SetColor(3);
		std::cout << "ECLI $ ";
		SetColor(7);
		// Read console
		std::cin.getline(line.GetPointer(), line.Capacity());
		// Reset param values
		resetParams(commandAmount);
		// Parse console input
		commandAmount = ParseParameters(line, params);

		if (GetParamIdWName("-command", params, commandAmount) > 0) {
			PrintParams(params, commandAmount);
		}

		if (GetParamIdWName("add", params, commandAmount) == 0) {
			lastParamId = GetParamIdWName("-verb", params, commandAmount);
			if (lastParamId > 0) {

			}
		}
		else if (GetParamIdWName("clear", params, commandAmount) == 0) {
			system("CLS");
			printHeader();
		}
		else if (GetParamIdWName("delete", params, commandAmount) == 0) {

			if (GetParamIdWName("-buffer", params, commandAmount) > 0) {
				buffer.Release();
				Log::Info("IO buffer deleted successfully.\n");
			}
			else if (GetParamIdWName("-tokens", params, commandAmount) > 0) {
				mainHandler.tokenize.FreeAll();
				Log::Info("Tokens deleted successfully.\n");
			}
			else if (GetParamIdWName("-nouns", params, commandAmount) > 0) {
				mainHandler.Delete(handle::Noun);
				Log::Info("Nouns deleted successfully.\n");
			}
			else if (GetParamIdWName("-verbs", params, commandAmount) > 0) {
				mainHandler.Delete(handle::Verb);
				Log::Info("All the verbs deleted successfully.\n");
			}
			else if (GetParamIdWName("-pronouns", params, commandAmount) > 0) {
				mainHandler.Delete(handle::Pronoun);
				Log::Info("Pronouns deleted successfully.\n");
			}
			else if (GetParamIdWName("-adverbs", params, commandAmount) > 0) {
				mainHandler.Delete(handle::Adverb);
				Log::Info("Adverbs deleted successfully.\n");
			}
			else if (GetParamIdWName("-adjectives", params, commandAmount) > 0) {
				mainHandler.Delete(handle::Adjective);
				Log::Info("Adjectives deleted successfully.\n");
			}
			else if (GetParamIdWName("-prepositions", params, commandAmount) > 0) {
				mainHandler.Delete(handle::Preposition);
				Log::Info("Prepositions deleted successfully.\n");
			}
			else if (GetParamIdWName("-conjunctions", params, commandAmount) > 0) {
				mainHandler.Delete(handle::Conjunction);
				Log::Info("Conjunctions deleted successfully.\n");
			}
			else if (GetParamIdWName("-interjections", params, commandAmount) > 0) {
				mainHandler.Delete(handle::Interjection);
				Log::Info("Interjections deleted successfully.\n");
			}
			else {
				Log::Error("Wrong or incompatible parameter!\n");
			}
			std::cout << "\n";
		}
		else if (GetParamIdWName("exit", params, commandAmount) == 0) {
			break;
		}
		else if (GetParamIdWName("help", params, commandAmount) == 0) {

			if (GetParamIdWName("tenses", params, commandAmount) > 0) {
				Log::Info("Time Tenses :\n") << HELP_TIME_TENSES << "\n";
			}
			else {
				Log::Info("All the existing commands are; \n") << HELP_MAIN << "\n";
			}

		}
		else if (GetParamIdWName("parse", params, commandAmount) == 0) { // /parse
			if (lastParamId = GetParamIdWName("-noun", params, commandAmount) > 0) {
				mainHandler.ParseWithType(handle::Noun, params[lastParamId].value);
			}
			else if (lastParamId = GetParamIdWName("-verb", params, commandAmount) > 0) {
				mainHandler.ParseWithType(handle::Verb, params[lastParamId].value);
			}
			else if (lastParamId = GetParamIdWName("-pronoun", params, commandAmount) > 0) {
				mainHandler.ParseWithType(handle::Pronoun, params[lastParamId].value);
			}
			else if (lastParamId = GetParamIdWName("-adverb", params, commandAmount) > 0) {
				mainHandler.ParseWithType(handle::Adverb, params[lastParamId].value);
			}
			else if (lastParamId = GetParamIdWName("-adjective", params, commandAmount) > 0) {
				mainHandler.ParseWithType(handle::Adjective, params[lastParamId].value);
			}
			else if (lastParamId = GetParamIdWName("-preposition", params, commandAmount) > 0) {
				mainHandler.ParseWithType(handle::Preposition, params[lastParamId].value);
			}
			else if (lastParamId = GetParamIdWName("-conjunction", params, commandAmount) > 0) {
				mainHandler.ParseWithType(handle::Conjunction, params[lastParamId].value);
			}
			else if (lastParamId = GetParamIdWName("-interjection", params, commandAmount) > 0) {
				mainHandler.ParseWithType(handle::Interjection, params[lastParamId].value);
			}
			else {
				mainHandler.ParseMultithread(params[0].value, true, nullptr);
			}

			std::cout << "\n";
		}
		else if (GetParamIdWName("print", params, commandAmount) == 0) {
			if (GetParamIdWName("-buffer", params, commandAmount) > 0) {
				if (buffer.capacity > 0) {
					Log::Info("IO BUFFER:\n") << buffer.As<char>() << "\n";
				}
				else {
					Log::Warning("IO BUFFER is empty.\n");
				}
			}
			else if (GetParamIdWName("-irregulars", params, commandAmount) > 0) {
				String output_str;
				if (mainHandler.verb.GetAllIrregularVerbs(output_str) > 0)
					Log::Info("[IRREGULAR VERB LIST]: \n") << output_str.Chars() << "\n";
				else
					Log::Warning("No irregular verb has found.\n");
			}
			else if (GetParamIdWName("-tokens", params, commandAmount) > 0) {
				String output_str;
				if (mainHandler.tokenize.GetAllSentences(output_str) == 1) {
					output_str.EndString();
					Log::Info("Parsed Text:\n") << output_str.Chars() << "\n";
				}
				else {
					Log::Warning("No token data found\n");
				}

			}
			std::cout << "\n";
		}
		else if (GetParamIdWName("read", params, commandAmount) == 0) {
			buffer.Release();
			bool print_suc = false;
			if (GetParamIdWName("-true", params, commandAmount) > 0) {
				print_suc = true;
			}
			if (lastParamId = GetParamIdWName("-dir", params, commandAmount) > 0) {
				if (GetParamIdWName("-all", params, commandAmount) > 0) {
					// Read all files with default namaes
				}
				else if (GetParamIdWName("-noun", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						mainHandler.Read(handle::Noun, buffer.As<char>(), buffer.capacity, print_suc);
					}
				}
				else if (GetParamIdWName("-verb", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						mainHandler.Read(handle::Verb, buffer.As<char>(), buffer.capacity, print_suc);
					}
				}
				else if (GetParamIdWName("-adverb", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						mainHandler.Read(handle::Adverb, buffer.As<char>(), buffer.capacity, print_suc);
					}
				}
				else if (GetParamIdWName("-pronoun", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						mainHandler.Read(handle::Pronoun, buffer.As<char>(), buffer.capacity, print_suc);
					}
				}
				else if (GetParamIdWName("-adjective", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						mainHandler.Read(handle::Adjective, buffer.As<char>(), buffer.capacity, print_suc);
					}
				}
				else if (GetParamIdWName("-preposition", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						mainHandler.Read(handle::Preposition, buffer.As<char>(), buffer.capacity, print_suc);
					}
				}
				else if (GetParamIdWName("-conjunction", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						mainHandler.Read(handle::Conjunction, buffer.As<char>(), buffer.capacity, print_suc);
					}
				}
				else if (GetParamIdWName("-interjection", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						mainHandler.Read(handle::Interjection, buffer.As<char>(), buffer.capacity, print_suc);
					}
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
		else if (GetParamIdWName("sr_parse", params, commandAmount) == 0) {
			mainHandler.ParseSentence(params[0].value);
			std::cout << "\n";
		}
		else if (GetParamIdWName("tokenize", params, commandAmount) == 0) {
			String output_str;

			mainHandler.tokenize.ParseString(params[0].value);
			mainHandler.tokenize.GetAllSentences(output_str);
			mainHandler.tokenize.FreeAll();
			output_str.EndString();

			Log::Info("Parsed Text:\n") << output_str.Chars() << "\n";
		}
		else {
			Log::Error(HELP_ERROR_COMMAND_NFOUND) << "\n";
		}
	}
}

void Ecli::PrintParams(Parameter* params, int8 amount) {
	for (int8 size = 0; size < amount; size++) {
		std::cout << "[NAME]: " << params[size].name.EndString().Chars();
		if (params[size].value.Length() != 0)
			std::cout << " [VALUE]: " << params[size].value.EndString().Chars();
		std::cout << std::endl;
	}
}

int8 Ecli::ParseParameters(const String& line, Parameter* out_params) const {
	bool next_param = false;
	bool open_value_w = false;
	int8 lastParamIndex = 0;

	for (int i = 0; i < line.Length(); i++) {
		if (line[i] == '\0')
			break;

		if (line[i] == ' ' || line[i] == '\t') {
			if (open_value_w) { // If the double quotes exist
				// Filling the value of parameter
				out_params[lastParamIndex].value += line[i];
			}
			else {
				if (next_param) {
					lastParamIndex++;
					if (lastParamIndex + 1 > MAX_PARAMETER_AMOUNT) {
						return MAX_PARAMETER_AMOUNT;
					}
					next_param = false;
				}
			}
		}
		else if (line[i] == '"') {
			if (!open_value_w) { // brackets has just appeared
				open_value_w = true;
				if (next_param == false && lastParamIndex != 0) {
					lastParamIndex--;
				}
			}
			else { // End brackets
				open_value_w = false;
				next_param = true;
			}
		}
		else if (open_value_w) {
			out_params[lastParamIndex].value += line[i];
		}
		else {
			out_params[lastParamIndex].name += line[i];
			next_param = true;
		}
	}

	return lastParamIndex + 1; // Returns the amount of parameters
}

int8 Ecli::GetParamIdWName(const char* param, const Parameter* params, int8 search_range) const {
	int param_index = 0;

	for (uint32 i = 0; i < search_range; i++) {
		if (params[i].name == param) {
			return i; // Return the index of parameter
		}
	}

	return -1;
}
