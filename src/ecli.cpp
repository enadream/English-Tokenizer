
// User defined libs
#include "ecli.hpp"
#include "log.hpp"
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

	int8 paramId = 0;
	int8 commandAmount = 0;

	auto resetParams = [&](int8 amount) {
		for (int8 i = 0; i < amount; i++) {
			params[i].name.SetLength(0);
			params[i].value.SetLength(0);
		}
	};

	auto printHeader = []() {
		SetColor(3);
		std::cout << "__________________ Welcome to the ChatBot (V0.90 @enadream) __________________\n\n" << std::endl;
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

		if (GetParamIdWName("clear", params, commandAmount) == 0) {
			system("CLS");
			printHeader();
		}
		else if (GetParamIdWName("delete", params, commandAmount) == 0) {
			if (GetParamIdWName("all", params, commandAmount) > 0) {

				mainHandler.tokenize.FreeAll();
				Log::Info("Tokens deleted successfully.\n");

				mainHandler.Delete(WordType::Noun);
				Log::Info("Nouns deleted successfully.\n");

				mainHandler.Delete(WordType::Verb);
				Log::Info("Verbs deleted successfully.\n");

				mainHandler.Delete(WordType::Pronoun);
				Log::Info("Pronouns deleted successfully.\n");

				mainHandler.Delete(WordType::Adverb);
				Log::Info("Adverbs deleted successfully.\n");

				mainHandler.Delete(WordType::Adjective);
				Log::Info("Adjectives deleted successfully.\n");

				mainHandler.Delete(WordType::Preposition);
				Log::Info("Prepositions deleted successfully.\n");

				mainHandler.Delete(WordType::Conjunction);
				Log::Info("Conjunctions deleted successfully.\n");

				mainHandler.Delete(WordType::Interjection);
				Log::Info("Interjections deleted successfully.\n");

				mainHandler.Delete(WordType::AuxiliaryVerb);
				Log::Info("Auxiliary verbs deleted successfully.\n");

				mainHandler.Delete(WordType::Determiner);
				Log::Info("Determiners deleted successfully.\n");
			}
			else if (GetParamIdWName("-tokens", params, commandAmount) > 0) {
				mainHandler.tokenize.FreeAll();
				Log::Info("Tokens deleted successfully.\n");
			}
			else if (GetParamIdWName("-nouns", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::Noun);
				Log::Info("Nouns deleted successfully.\n");
			}
			else if (GetParamIdWName("-verbs", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::Verb);
				Log::Info("Verbs deleted successfully.\n");
			}
			else if (GetParamIdWName("-pronouns", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::Pronoun);
				Log::Info("Pronouns deleted successfully.\n");
			}
			else if (GetParamIdWName("-adverbs", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::Adverb);
				Log::Info("Adverbs deleted successfully.\n");
			}
			else if (GetParamIdWName("-adjectives", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::Adjective);
				Log::Info("Adjectives deleted successfully.\n");
			}
			else if (GetParamIdWName("-prepositions", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::Preposition);
				Log::Info("Prepositions deleted successfully.\n");
			}
			else if (GetParamIdWName("-conjunctions", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::Conjunction);
				Log::Info("Conjunctions deleted successfully.\n");
			}
			else if (GetParamIdWName("-interjections", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::Interjection);
				Log::Info("Interjections deleted successfully.\n");
			}
			else if (GetParamIdWName("-aux_verbs", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::AuxiliaryVerb);
				Log::Info("Auxiliary verbs deleted successfully.\n");
			}
			else if (GetParamIdWName("-determiners", params, commandAmount) > 0) {
				mainHandler.Delete(WordType::Determiner);
				Log::Info("Determiners deleted successfully.\n");
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
			if (GetParamIdWName("clear", params, commandAmount) > 0) {
				Log::Info(HELP_PARAM_CLEAR) << "\n\n";
			}
			else if (GetParamIdWName("delete", params, commandAmount) > 0) {
				Log::Info(HELP_PARAM_DELETE) << "\n\n";
			}
			else if (GetParamIdWName("exit", params, commandAmount) > 0) {
				Log::Info(HELP_PARAM_EXIT) << "\n\n";
			}
			else if (GetParamIdWName("parse", params, commandAmount) > 0) {
				Log::Info(HELP_PARAM_PARSE) << "\n\n";
			}
			else if (GetParamIdWName("print", params, commandAmount) > 0) {
				Log::Info(HELP_PARAM_PRINT) << "\n\n";
			}
			else if (GetParamIdWName("read", params, commandAmount) > 0) {
				Log::Info(HELP_PARAM_READ) << "\n\n";
			}
			else if (GetParamIdWName("sr_parse", params, commandAmount) > 0) {
				Log::Info(HELP_PARAM_SR_PARSE) << "\n\n";
			}
			else if (GetParamIdWName("tester", params, commandAmount) > 0) {
				Log::Info(HELP_PARAM_TESTER) << "\n\n";
			}
			else if (GetParamIdWName("tokenize", params, commandAmount) > 0) {
				Log::Info(HELP_PARAM_TOKENIZE) << "\n\n";
			}
			else {
				Log::Info("All the existing commands are; \n") << HELP_MAIN << "\n";
			}

		}
		else if (GetParamIdWName("parse", params, commandAmount) == 0) { // /parse
			if (paramId = GetParamIdWName("-noun", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::Noun, params[paramId].value);
			}
			else if (paramId = GetParamIdWName("-verb", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::Verb, params[paramId].value);
			}
			else if (paramId = GetParamIdWName("-pronoun", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::Pronoun, params[paramId].value);
			}
			else if (paramId = GetParamIdWName("-adverb", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::Adverb, params[paramId].value);
			}
			else if (paramId = GetParamIdWName("-adjective", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::Adjective, params[paramId].value);
			}
			else if (paramId = GetParamIdWName("-preposition", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::Preposition, params[paramId].value);
			}
			else if (paramId = GetParamIdWName("-conjunction", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::Conjunction, params[paramId].value);
			}
			else if (paramId = GetParamIdWName("-interjection", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::Interjection, params[paramId].value);
			}
			else if (paramId = GetParamIdWName("-aux_verb", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::AuxiliaryVerb, params[paramId].value);
			}
			else if (paramId = GetParamIdWName("-determiner", params, commandAmount) > 0) {
				mainHandler.ParseWithType(WordType::Determiner, params[paramId].value);
			}
			else {
				WordToken a;
				mainHandler.ParseMultiple(params[0].value, true, a);
			}

			std::cout << "\n";
		}
		else if (GetParamIdWName("print", params, commandAmount) == 0) {
			if (GetParamIdWName("-irregulars", params, commandAmount) > 0) {
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
					Log::Info("All tokens:\n") << output_str.Chars() << "\n";
				}
				else {
					Log::Warning("No token data found\n");
				}
			}
			std::cout << "\n";
		}
		else if (GetParamIdWName("q_generate", params, commandAmount) == 0) {
			mainHandler.QuGenerate(params[0].value);
			std::cout << "\n";
		}
		else if (GetParamIdWName("read", params, commandAmount) == 0) {
			if (paramId = GetParamIdWName("-dir", params, commandAmount) > 0) {
				if (GetParamIdWName("-noun", params, commandAmount) > 0) {
					mainHandler.Read(WordType::Noun, params[paramId].value.EndString());
				}
				else if (GetParamIdWName("-verb", params, commandAmount) > 0) {
					mainHandler.Read(WordType::Verb, params[paramId].value.EndString());
				}
				else if (GetParamIdWName("-adverb", params, commandAmount) > 0) {
					mainHandler.Read(WordType::Adverb, params[paramId].value.EndString());
				}
				else if (GetParamIdWName("-pronoun", params, commandAmount) > 0) {
					mainHandler.Read(WordType::Pronoun, params[paramId].value.EndString());
				}
				else if (GetParamIdWName("-adjective", params, commandAmount) > 0) {
					mainHandler.Read(WordType::Adjective, params[paramId].value.EndString());
				}
				else if (GetParamIdWName("-preposition", params, commandAmount) > 0) {
					mainHandler.Read(WordType::Preposition, params[paramId].value.EndString());
				}
				else if (GetParamIdWName("-conjunction", params, commandAmount) > 0) {
					mainHandler.Read(WordType::Conjunction, params[paramId].value.EndString());
				}
				else if (GetParamIdWName("-interjection", params, commandAmount) > 0) {
					mainHandler.Read(WordType::Interjection, params[paramId].value.EndString());
				}
				else if (GetParamIdWName("-aux_verb", params, commandAmount) > 0) {
					mainHandler.Read(WordType::AuxiliaryVerb, params[paramId].value.EndString());
				}
				else if (GetParamIdWName("-determiner", params, commandAmount) > 0) {
					mainHandler.Read(WordType::Determiner, params[paramId].value.EndString());
				}
				else {
					Log::Warning("No type parameter found in the command!\n");
				}
			}
			else if (paramId = GetParamIdWName("all", params, commandAmount) > 0) {
				mainHandler.Read(WordType::Noun, "dict/nouns.txt");
				mainHandler.Read(WordType::Verb, "dict/verbs.txt");
				mainHandler.Read(WordType::Adverb, "dict/advs.txt");
				mainHandler.Read(WordType::Pronoun, "dict/pronouns.txt");
				mainHandler.Read(WordType::Adjective, "dict/adjs.txt");
				mainHandler.Read(WordType::Preposition, "dict/prepos.txt");
				mainHandler.Read(WordType::Conjunction, "dict/conjs.txt");
				mainHandler.Read(WordType::Interjection, "dict/interjs.txt");
				mainHandler.Read(WordType::AuxiliaryVerb, "dict/verbs_aux.txt");
				mainHandler.Read(WordType::Determiner, "dict/dets.txt");
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
		else if (GetParamIdWName("tester", params, commandAmount) == 0) {
			if (paramId = GetParamIdWName("-dir", params, commandAmount) > 0) {
				mainHandler.Test(params[paramId].value);
			}
			else {
				Log::Warning("No -dir parameter found in the command!\n");
			}

			std::cout << "\n";
		}
		else if (GetParamIdWName("tokenize", params, commandAmount) == 0) {
			String output_str;

			mainHandler.tokenize.ParseString(params[0].value);
			mainHandler.tokenize.GetAllSentences(output_str);
			mainHandler.tokenize.FreeAll();
			output_str.EndString();

			Log::Info("Tokenized Text:\n") << output_str.Chars() << "\n";
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
