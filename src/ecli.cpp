
// User defined libs
#include "ecli.hpp"
#include "misc/data_types.hpp"
#include "misc/string.hpp"

#include "log.hpp"
#include "buffer.hpp"

#include "text_data.hpp"
#include "file_system.hpp"

#include "verb.hpp"
#include "noun.hpp"
#include "tokenizer.hpp"
#include "simple_types.hpp"

#define COMMAND_LINE_LENGTH 1000
#define MAX_PARAMETER_AMOUNT 10

void Ecli::Start() {
	String line(COMMAND_LINE_LENGTH); // Console max read size
	Parameter params[MAX_PARAMETER_AMOUNT];


	noun::NounHandler noun;
	basic::BasicType pronoun;
	basic::BasicType adj;
	basic::BasicType adv;
	verb::VerbHandler verb;
	basic::BasicType prepos;
	basic::BasicType conj;
	basic::BasicType interj;


	Buffer buffer;
	String output_str;
	tokenizer::Tokenizer tokenize;


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
		std::cout << "__________________ Welcome to the ChatBot (V0.3 @enadream) __________________\n\n" << std::endl;
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
		// Reset output str
		output_str.SetLength(0);
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

			if (GetParamIdWName("-verbs", params, commandAmount) > 0) {
				if (verb.DeleteAll() == 0)
					Log::Info("All the verbs deleted successfully.\n");
			}
			else if (GetParamIdWName("-buffer", params, commandAmount) > 0) {
				buffer.Release();
				Log::Info("IO buffer deleted successfully.\n");
			}
			else if (GetParamIdWName("-tokens", params, commandAmount) > 0) {
				tokenize.FreeAll();
				Log::Info("Tokens deleted successfully.\n");
			}
			else if (GetParamIdWName("-prepositions", params, commandAmount) > 0) {
				prepos.FreeAll();
				Log::Info("Prepositions deleted successfully.\n");
			}
			else if (GetParamIdWName("-nouns", params, commandAmount) > 0) {
				noun.DeleteAll();
				Log::Info("Nouns deleted successfully.\n");
			}
			else if (GetParamIdWName("-pronouns", params, commandAmount) > 0) {
				pronoun.FreeAll();
				Log::Info("Pronouns deleted successfully.\n");
			}
			else if (GetParamIdWName("-adjectives", params, commandAmount) > 0) {
				adj.FreeAll();
				Log::Info("Adjectives deleted successfully.\n");
			}
			else if (GetParamIdWName("-adverbs", params, commandAmount) > 0) {
				adv.FreeAll();
				Log::Info("Adverbs deleted successfully.\n");
			}
			else if (GetParamIdWName("-conjunctions", params, commandAmount) > 0) {
				conj.FreeAll();
				Log::Info("Conjunctions deleted successfully.\n");
			}
			else if (GetParamIdWName("-interjections", params, commandAmount) > 0) {
				interj.FreeAll();
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
		else if (GetParamIdWName("find", params, commandAmount) == 0) { // /find
			lastParamId = GetParamIdWName("-verb", params, commandAmount);

			if (lastParamId > 0) {
				switch (verb.ParseVerb(params[lastParamId].value, output_str))
				{
				case 1:
					Log::Info("Verb has found:\n") << output_str.Chars();
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
				switch (noun.ParseNoun(params[lastParamId].value.Chars(), params[lastParamId].value.Length(), output_str))
				{
				case 1:
					Log::Info("Noun has found:\n") << output_str.Chars();
					break;
				case 2:
					Log::Warning("The input is empty.\n");
					break;
				case -1:
					Log::Warning("No noun has found.\n");
					break;
				case -2:
					Log::Warning("The input contains some characters which is not alphabetic.\n");
					break;
				case -3:
					Log::Warning("The first character of the noun cannot be hyphen.\n");
					break;
				case -4:
					Log::Warning("The noun cannot be less than 1 characters.\n");
					break;
				case -5:
					Log::Warning("Character size exceeds ") << NOUN_CHAR_SIZE << " size.\n";
					break;
				default:
					break;
				}
			}
			else if (lastParamId = GetParamIdWName("-pronoun", params, commandAmount) > 0) {
				switch (pronoun.ParseWord(params[lastParamId].value.Chars(), params[lastParamId].value.Length(), output_str))
				{
				case 0:
					Log::Info("Pronoun has found:\n") << output_str.Chars();
					break;
				case -1:
					Log::Warning("No pronoun has found.\n");
					break;
				case -4:
					Log::Warning("The input contains some characters which is not alphabetic.\n");
					break;
				case -2:
					Log::Warning("Character size exceeds ") << WORD_CHAR_SIZE << " size.\n";
					break;
				case -3:
					Log::Warning("The input is empty\n");
					break;
				default:
					break;
				}
			}
			else if (lastParamId = GetParamIdWName("-adjective", params, commandAmount) > 0) {
				switch (adj.ParseWord(params[lastParamId].value.Chars(), params[lastParamId].value.Length(), output_str))
				{
				case 0:
					Log::Info("Adjective has found:\n") << output_str.Chars();
					break;
				case -1:
					Log::Warning("No adjective has found.\n");
					break;
				case -4:
					Log::Warning("The input contains some characters which is not alphabetic.\n");
					break;
				case -2:
					Log::Warning("Character size exceeds ") << WORD_CHAR_SIZE << " size.\n";
					break;
				case -3:
					Log::Warning("The input is empty\n");
					break;
				default:
					break;
				}
			}
			else if (lastParamId = GetParamIdWName("-adverb", params, commandAmount) > 0) {
				switch (adv.ParseWord(params[lastParamId].value.Chars(), params[lastParamId].value.Length(), output_str))
				{
				case 0:
					Log::Info("Adverb has found:\n") << output_str.Chars();
					break;
				case -1:
					Log::Warning("No adverb has found.\n");
					break;
				case -4:
					Log::Warning("The input contains some characters which is not alphabetic.\n");
					break;
				case -2:
					Log::Warning("Character size exceeds ") << WORD_CHAR_SIZE << " size.\n";
					break;
				case -3:
					Log::Warning("The input is empty\n");
					break;
				default:
					break;
				}
			}
			else if (lastParamId = GetParamIdWName("-verb", params, commandAmount) > 0) {
				switch (verb.ParseVerb(params[lastParamId].value, output_str, true))
				{
				case 1:
					Log::Info("Verb has found:\n") << output_str.Chars();
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
			else if (lastParamId = GetParamIdWName("-preposition", params, commandAmount) > 0) {
				switch (prepos.ParseWord(params[lastParamId].value.Chars(), params[lastParamId].value.Length(), output_str))
				{
				case 0:
					Log::Info("Preposition has found:\n") << output_str.Chars();
					break;
				case -1:
					Log::Warning("No preposition has found.\n");
					break;
				case -4:
					Log::Warning("The input contains some characters which is not alphabetic.\n");
					break;
				case -2:
					Log::Warning("Character size exceeds ") << WORD_CHAR_SIZE << " size.\n";
					break;
				case -3:
					Log::Warning("The input is empty\n");
					break;
				default:
					break;
				}
			}
			else if (lastParamId = GetParamIdWName("-conjunction", params, commandAmount) > 0) {
				switch (conj.ParseWord(params[lastParamId].value.Chars(), params[lastParamId].value.Length(), output_str))
				{
				case 0:
					Log::Info("Conjunction has found:\n") << output_str.Chars();
					break;
				case -1:
					Log::Warning("No conjunction has found.\n");
					break;
				case -4:
					Log::Warning("The input contains some characters which is not alphabetic.\n");
					break;
				case -2:
					Log::Warning("Character size exceeds ") << WORD_CHAR_SIZE << " size.\n";
					break;
				case -3:
					Log::Warning("The input is empty\n");
					break;
				default:
					break;
				}
			}
			else if (lastParamId = GetParamIdWName("-interjection", params, commandAmount) > 0) {
				switch (interj.ParseWord(params[lastParamId].value.Chars(), params[lastParamId].value.Length(), output_str))
				{
				case 0:
					Log::Info("Interjection has found:\n") << output_str.Chars();
					break;
				case -1:
					Log::Warning("No interjection has found.\n");
					break;
				case -4:
					Log::Warning("The input contains some characters which is not alphabetic.\n");
					break;
				case -2:
					Log::Warning("Character size exceeds ") << WORD_CHAR_SIZE << " size.\n";
					break;
				case -3:
					Log::Warning("The input is empty\n");
					break;
				default:
					break;
				}
			}
			else {
				Log::Error("No type identifier found!\n");
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
				if (verb.GetAllIrregularVerbs(output_str) > 0)
					Log::Info("[IRREGULAR VERB LIST]: \n") << output_str.Chars() << "\n";
				else
					Log::Warning("No irregular verb has found.\n");
			}
			else if (GetParamIdWName("-tokens", params, commandAmount) > 0) {
				if (tokenize.GetAllSentences(output_str) == 1) {
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
			if (lastParamId = GetParamIdWName("-dir", params, commandAmount) > 0) {
				if (GetParamIdWName("-all", params, commandAmount) > 0) {
					// Read all files with default namaes
				}
				else if (GetParamIdWName("-noun", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						noun.MultipleAdder(buffer.As<char>(), buffer.capacity);
					}
				}
				else if (GetParamIdWName("-pronoun", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						pronoun.MultipleAdder(buffer.As<char>(), buffer.capacity, "pronouns");
					}
				}
				else if (GetParamIdWName("-adjective", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						adj.MultipleAdder(buffer.As<char>(), buffer.capacity, "adjectives");
					}
				}
				else if (GetParamIdWName("-adverb", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						adv.MultipleAdder(buffer.As<char>(), buffer.capacity, "adverbs");
					}
				}
				else if (GetParamIdWName("-verb", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						verb.MultipleVerbAdder(buffer.As<char>(), buffer.capacity);
					}
				}
				else if (GetParamIdWName("-preposition", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						prepos.MultipleAdder(buffer.As<char>(), buffer.capacity, "prepositions");
					}
				}
				else if (GetParamIdWName("-conjunction", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						conj.MultipleAdder(buffer.As<char>(), buffer.capacity, "conjunctions");
					}
				}
				else if (GetParamIdWName("-interjection", params, commandAmount) > 0) {
					if (FileSystem::ReadFromDisk(buffer, params[lastParamId].value.EndString().Chars()) == 0) {
						interj.MultipleAdder(buffer.As<char>(), buffer.capacity, "interjections");
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
		else if (GetParamIdWName("tokenize", params, commandAmount) == 0) {
			tokenize.FreeAll();
			tokenize.ParseClause(params[0].value);
			tokenize.GetAllSentences(output_str);
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
