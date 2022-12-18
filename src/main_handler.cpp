// Standart libs
#include <thread>

// User defined libs
#include "main_handler.hpp"
#include "log.hpp"
#include "misc/util.hpp"

namespace handle {

	void MainHandler::Read(const WordType type, const char* data, const uint64& length) {
		switch (type)
		{
		case Noun:
			noun.MultipleAdder(data, length);
			return;
		case Verb:
			verb.MultipleVerbAdder(data, length);
			return;
		case Pronoun:
			pronoun.MultipleAdder(data, length, "pronouns");
			return;
		case Adverb:
			adv.MultipleAdder(data, length, "adverbs");
			return;
		case Adjective:
			adj.MultipleAdder(data, length, "adjectives");
			return;
		case Preposition:
			prepos.MultipleAdder(data, length, "prepositions");
			return;
		case Conjunction:
			conj.MultipleAdder(data, length, "conjunctions");
			return;
		case Interjection:
			interj.MultipleAdder(data, length, "interjections");
			return;
		default:
			return;
		}
	}

	void MainHandler::Delete(const WordType type) {
		switch (type)
		{
		case Noun:
			noun.DeleteAll();
			return;
		case Verb:
			verb.DeleteAll();
			return;
		case Pronoun:
			pronoun.FreeAll();
			return;
		case Adverb:
			adv.FreeAll();
			return;
		case Adjective:
			adj.FreeAll();
			return;
		case Preposition:
			prepos.FreeAll();
			return;
		case Conjunction:
			conj.FreeAll();
			return;
		case Interjection:
			interj.FreeAll();
			return;
		default:
			return;
		}
	}

	void MainHandler::Parse(const WordType type, const String& str) { // Parse with known type
		int32 result = -1;
		String out_str;

		switch (type)
		{
		case Noun:
			ParseNoun(str.Chars(), str.Length(), &out_str, &result);
			PrintResult(Noun, result, out_str);
			return;
		case Verb:
			ParseVerb(&str, &out_str, &result);
			PrintResult(Verb, result, out_str);
			return;
		case Pronoun:
			ParsePronoun(str.Chars(), str.Length(), &out_str, &result);
			PrintResult(Pronoun, result, out_str);
			return;
		case Adverb:
			ParseAdv(str.Chars(), str.Length(), &out_str, &result);
			PrintResult(Adverb, result, out_str);
			return;
		case Adjective:
			ParseAdj(str.Chars(), str.Length(), &out_str, &result);
			PrintResult(Adjective, result, out_str);
			return;
		case Preposition:
			ParsePrepos(str.Chars(), str.Length(), &out_str, &result);
			PrintResult(Preposition, result, out_str);
			return;
		case Conjunction:
			ParseConj(str.Chars(), str.Length(), &out_str, &result);
			PrintResult(Conjunction, result, out_str);
			return;
		case Interjection:
			ParseInterj(str.Chars(), str.Length(), &out_str, &result);
			PrintResult(Interjection, result, out_str);
			return;
		default:
			return;
		}
	}

	void MainHandler::Parse(const String& str, const int32 word_id) { // Multithread parser

		int32 result[8];
		String out_str[8];

		// Start Threads
		std::thread noun_thread(&MainHandler::ParseNoun, this, str.Chars(), str.Length(), &out_str[0], &result[0]);
		std::thread verb_thread(&MainHandler::ParseVerb, this, &str, &out_str[1], &result[1]);
		std::thread pronoun_thread(&MainHandler::ParsePronoun, this, str.Chars(), str.Length(), &out_str[2], &result[2]);
		std::thread adv_thread(&MainHandler::ParseAdv, this, str.Chars(), str.Length(), &out_str[3], &result[3]);
		std::thread adj_thread(&MainHandler::ParseAdj, this, str.Chars(), str.Length(), &out_str[4], &result[4]);
		std::thread prepos_thred(&MainHandler::ParsePrepos, this, str.Chars(), str.Length(), &out_str[5], &result[5]);
		std::thread conj_thread(&MainHandler::ParseConj, this, str.Chars(), str.Length(), &out_str[6], &result[6]);
		std::thread interj_thread(&MainHandler::ParseInterj, this, str.Chars(), str.Length(), &out_str[7], &result[7]);

		// Wait for completion of threads
		noun_thread.join();
		verb_thread.join();
		pronoun_thread.join();
		adv_thread.join();
		adj_thread.join();
		prepos_thred.join();
		conj_thread.join();
		interj_thread.join();

		// Printing Part
		if (word_id == -1) {
			int found = 0;
			for (int32 i = 0; i < 8; i++) { // Print results
				if (result[i] == 1) {
					found++;
					SetColor(14);
					std::cout << "\n[RESULT " << found << "]: \n";
					SetColor(7);

					PrintResult((WordType)(i + 1), result[i], out_str[i]);
				}
			}
			if (!found) {
				Log::Warning("No result has found.\n");
			}
		}
		else {
			bool found = false;
			for (int32 i = 0; i < 8; i++) { // Print results
				if (result[i] == 1) {
					words.at(word_id).type.push_back((WordType)(i + 1));
					found = true;
				}
			}
			if (!found) {
				words.at(word_id).type.push_back(Undefined);
			}
		}

	}

	void MainHandler::ParseSentence(const String& str) {
		String out_str;

		// Tokenize input
		tokenize.ParseString(str);
		// Parse each sentence 
		for (uint32 i = 0; tokenize.sentences[i].Tokens() != nullptr; i++) {
			words.reserve(tokenize.sentences[i].Amount());
			// Parse each token
			for (uint32 j = 0; j < tokenize.sentences[i].Amount(); j++) {
				// Copy adress of word to words
				words.push_back(Word());
				words.at(j).data = &tokenize.sentences[i][j];
				// Parse token
				Parse(tokenize.sentences[i][j], j);
			}
			// Words to string
			out_str += "\n\x1b[38;5;198m[SENTENCE ";
			util::IntToStr(out_str, i + 1);
			out_str += "]: \n\x1b[0m";
			WordsToStr(out_str);
			// Delete words in sentence
			words.clear();
		}

		std::cout << out_str.EndString().Chars();
		// Free tokens
		tokenize.FreeAll();
	}

	void MainHandler::ParseNoun(const char* raw_word, const uint8 length, String* out_string, int32* result) {
		*result = noun.ParseNoun(raw_word, length, *out_string);
	}
	void MainHandler::ParseVerb(const String* raw_word, String* out_string, int32* result) {
		*result = verb.ParseVerb(*raw_word, *out_string, true);
	}
	void MainHandler::ParsePronoun(const char* raw_word, const uint8 length, String* out_string, int32* result) {
		*result = pronoun.ParseWord(raw_word, length, *out_string);
	}
	void MainHandler::ParseAdv(const char* raw_word, const uint8 length, String* out_string, int32* result) {
		*result = adv.ParseWord(raw_word, length, *out_string);
	}
	void MainHandler::ParseAdj(const char* raw_word, const uint8 length, String* out_string, int32* result) {
		*result = adj.ParseWord(raw_word, length, *out_string);
	}
	void MainHandler::ParsePrepos(const char* raw_word, const uint8 length, String* out_string, int32* result) {
		*result = prepos.ParseWord(raw_word, length, *out_string);
	}
	void MainHandler::ParseConj(const char* raw_word, const uint8 length, String* out_string, int32* result) {
		*result = conj.ParseWord(raw_word, length, *out_string);
	}
	void MainHandler::ParseInterj(const char* raw_word, const uint8 length, String* out_string, int32* result) {
		*result = interj.ParseWord(raw_word, length, *out_string);
	}

	void MainHandler::WordsToStr(String& out_str) {
		auto printWordType = [&](WordType type) {
			switch (type)
			{
			case handle::Undefined:
				out_str += "\x1b[38;5;3mUnknown\x1b[0m";
				break;
			case handle::Noun:
				out_str += "\x1b[38;5;43mNoun\x1b[0m";
				break;
			case handle::Verb:
				out_str += "\x1b[38;5;43mVerb\x1b[0m";
				break;
			case handle::Pronoun:
				out_str += "\x1b[38;5;43mPronoun\x1b[0m";
				break;
			case handle::Adverb:
				out_str += "\x1b[38;5;43mAdverb\x1b[0m";
				break;
			case handle::Adjective:
				out_str += "\x1b[38;5;43mAdjective\x1b[0m";
				break;
			case handle::Preposition:
				out_str += "\x1b[38;5;43mPrepostion\x1b[0m";
				break;
			case handle::Conjunction:
				out_str += "\x1b[38;5;43mConjunction\x1b[0m";
				break;
			case handle::Interjection:
				out_str += "\x1b[38;5;43mInterjection\x1b[0m";
				break;
			default:
				break;
			}
		};

		for (uint32 i = 0; i < words.size(); i++) {
			util::IntToStr(out_str, i + 1);

			out_str += "- [\x1b[38;5;231m";
			out_str += *words.at(i).data;
			out_str += "\x1b[0m]: ";
			for (uint32 j = 0; j < words.at(i).type.size(); j++) {
				printWordType(words.at(i).type.at(j));
				if (j != words.at(i).type.size() - 1) { // If not last index
					out_str += ", ";
				}
				else {
					out_str += '.';
				}
			}
			out_str += '\n';
		}
	}

	void MainHandler::PrintResult(const WordType type, const int32& result, const String& str) {
		switch (type)
		{
		case Noun:
			switch (result)
			{
			case 1:
				Log::Info("Noun has been found:\n") << str.Chars();
				break;
			case 2:
				Log::Warning("The input is empty.\n");
				break;
			case -1:
				Log::Warning("No noun has been found.\n");
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
			return;
		case Verb:
			switch (result)
			{
			case 1:
				Log::Info("Verb has been found:\n") << str.Chars();
				break;
			case 2:
				Log::Warning("The input is empty.\n");
				break;
			case -1:
				Log::Warning("No verb has been found.\n");
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
			return;
		case Pronoun:
			switch (result)
			{
			case 1:
				Log::Info("Pronoun has been found:\n") << str.Chars();
				break;
			case -1:
				Log::Warning("No pronoun has been found.\n");
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
			return;
		case Adverb:
			switch (result)
			{
			case 1:
				Log::Info("Adverb has been found:\n") << str.Chars();
				break;
			case -1:
				Log::Warning("No adverb has been found.\n");
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
			return;
		case Adjective:
			switch (result)
			{
			case 1:
				Log::Info("Adjective has been found:\n") << str.Chars();
				break;
			case -1:
				Log::Warning("No adjective has been found.\n");
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
			return;
		case Preposition:
			switch (result)
			{
			case 1:
				Log::Info("Preposition has been found:\n") << str.Chars();
				break;
			case -1:
				Log::Warning("No preposition has been found.\n");
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
			return;
		case Conjunction:
			switch (result)
			{
			case 1:
				Log::Info("Conjunction has been found:\n") << str.Chars();
				break;
			case -1:
				Log::Warning("No conjunction has been found.\n");
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
			return;
		case Interjection:
			switch (result)
			{
			case 1:
				Log::Info("Interjection has been found:\n") << str.Chars();
				break;
			case -1:
				Log::Warning("No interjection has been found.\n");
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
			return;
		default:
			return;
		}
	}

}