// User defined libs
#include "main_handler.hpp"
#include "log.hpp"
#include "misc/util.hpp"

#include "clock.hpp"

namespace handle {

	void MainHandler::Test(String& dir) {
		Buffer buffer;

		if (FileSystem::ReadFromDisk(buffer, dir.EndString().Chars()) == -1) { // If file doesn't exist
			return;
		}

		uint32 lineNumber = 1;
		uint32 successfulParse = 0;
		uint32 failedParse = 0;
		String out_str(buffer.capacity);
		Clock timer;
		String tempLine(256);

		auto getNextLine = [&](const char* file, uint64& index, const uint64& capacity) {
			uint32 i = 0;
			bool entered = false;
			tempLine.SetLength(0);

			for (; i + index < capacity; i++) {
				if (file[i + index] == '\r' || file[i + index] == '\0') {
					tempLine.Replace(&file[index], i); // Copy data until carriage return
					entered = true;
					break;
				}
			}
			index += i + 1; // Returning the position of /n
			return entered; // Returning true if placed new word
		};

		WordToken word;

		timer.StartTimer();
		for (uint64 i = 0; i < buffer.capacity; i++) {
			// Read line by line
			if (!getNextLine(buffer.As<char>(), i, buffer.capacity)) {
				break;
			}

			if (tempLine.Length() > 0) {
				// Parse each line
				ParseMultiple(tempLine, false, word);
				// Write word to file
				util::IntToStr(out_str, lineNumber);
				out_str.Append("- ", 2);
				out_str += tempLine;
				out_str.Append(" = ", 3);
				WordToStr(word, out_str);
				out_str += '\n';

				if (word.types.back().type == Undefined)
					failedParse++;
				else
					successfulParse++;

				word.types.clear();
			}

			// Increase line Number
			lineNumber++;
		}
		timer.EndTimer();

		lineNumber--;
		// Print result
		float64 duration = (double)timer.GetDuration(*"ns") / 1'000'000'000;
		Log::Info(lineNumber) << " words parsed, " << successfulParse << " words parsed successfully, " << failedParse << " words failed. " <<
			"The operation completed in " << duration << " seconds. (" << duration / lineNumber << " sec / word)\n";

		// Write file to the disk
		dir.RemoveLast(4);
		dir += "_out.txt";
		FileSystem::WriteToDisk(out_str.Chars(), dir.EndString().Chars(), out_str.Length());
	}

	void MainHandler::Read(const WordType type, String& dir) {
		Buffer buffer;

		if (FileSystem::ReadFromDisk(buffer, dir.EndString().Chars()) == -1) {
			return;
		}
		switch (type)
		{
		case Noun:
			noun.MultipleAdder(buffer.As<char>(), buffer.capacity);
			return;
		case Verb:
			verb.MultipleVerbAdder(buffer.As<char>(), buffer.capacity);
			return;
		case Pronoun:
			pronoun.MultipleAdder(buffer.As<char>(), buffer.capacity, "pronouns");
			return;
		case Adverb:
			adv.MultipleAdder(buffer.As<char>(), buffer.capacity, "adverbs");
			return;
		case Adjective:
			adj.MultipleAdder(buffer.As<char>(), buffer.capacity, "adjectives");
			return;
		case Preposition:
			prepos.MultipleAdder(buffer.As<char>(), buffer.capacity, "prepositions");
			return;
		case Conjunction:
			conj.MultipleAdder(buffer.As<char>(), buffer.capacity, "conjunctions");
			return;
		case Interjection:
			interj.MultipleAdder(buffer.As<char>(), buffer.capacity, "interjections");
			return;
		case AuxiliaryVerb:
			aux_verb.MultipleAdder(buffer.As<char>(), buffer.capacity, "auxiliary verbs");
			return;
		case Determiner:
			det.MultipleAdder(buffer.As<char>(), buffer.capacity, "determiners");
			return;
		default:
			return;
		}
	}

	void MainHandler::Delete(const WordType type) {
		switch (type)
		{
		case Noun:
			noun.Free();
			return;
		case Verb:
			verb.Free();
			return;
		case Pronoun:
			pronoun.Free();
			return;
		case Adverb:
			adv.Free();
			return;
		case Adjective:
			adj.Free();
			return;
		case Preposition:
			prepos.Free();
			return;
		case Conjunction:
			conj.Free();
			return;
		case Interjection:
			interj.Free();
			return;
		case AuxiliaryVerb:
			aux_verb.Free();
			return;
		case Determiner:
			det.Free();
			return;
		default:
			return;
		}
	}

	void MainHandler::ParseWithType(const WordType type, const String& str) { // Parse with known type
		int32 result;
		String out_str;
		TypeAndSuffixes word;

		switch (type)
		{
		case Noun:
			result = noun.ParseNoun(str, word, out_str, true);
			PrintResult(Noun, result, out_str);
			return;
		case Verb:
			result = verb.ParseVerb(str, word, out_str, true);
			PrintResult(Verb, result, out_str);
			return;
		case AuxiliaryVerb:
			result = aux_verb.ParseWord(str, word, out_str, true);
			PrintResult(AuxiliaryVerb, result, out_str);
			return;
		case Pronoun:
			result = pronoun.ParseWord(str, out_str, true);
			PrintResult(Pronoun, result, out_str);
			return;
		case Adverb:
			result = adv.ParseWord(str, out_str, true);
			PrintResult(Adverb, result, out_str);
			return;
		case Adjective:
			result = adj.ParseWord(str, out_str, true);
			PrintResult(Adjective, result, out_str);
			return;
		case Preposition:
			result = prepos.ParseWord(str, out_str, true);
			PrintResult(Preposition, result, out_str);
			return;
		case Conjunction:
			result = conj.ParseWord(str, out_str, true);
			PrintResult(Conjunction, result, out_str);
			return;
		case Interjection:
			result = interj.ParseWord(str, out_str, true);
			PrintResult(Interjection, result, out_str);
			return;
		case Determiner:
			result = det.ParseWord(str, out_str, true);
			PrintResult(Determiner, result, out_str);
			return;
		default:
			return;
		}
	}

	void MainHandler::ParseMultiple(const String& raw_word, const bool print_result, WordToken& word) { // Multithread parser
		util::Array<int32, 11> result;
		String outStrs[result.GetSize()];

		// Found types
		TypeAndSuffixes found_words[3];

		ParseNoun(&raw_word, &result[0], &found_words[0], &outStrs[0], print_result);
		ParseVerb(&raw_word, &result[1], &found_words[1], &outStrs[1], print_result);
		ParseAuxVerb(&raw_word, &found_words[2], &result[2], &outStrs[2], print_result);
		ParsePronoun(&raw_word, &result[3], &outStrs[3], print_result);
		ParseAdv(&raw_word, &result[4], &outStrs[4], print_result);
		ParseAdj(&raw_word, &result[5], &outStrs[5], print_result);
		ParsePrepos(&raw_word, &result[6], &outStrs[6], print_result);
		ParseConj(&raw_word, &result[7], &outStrs[7], print_result);
		ParseInterj(&raw_word, &result[8], &outStrs[8], print_result);
		ParseDet(&raw_word, &result[9], &outStrs[9], print_result);
		ParsePunc(&raw_word, &result[10]);

		// Printing Part
		if (print_result) {
			int found = 0;
			for (int32 i = 0; i < result.GetSize(); i++) { // Print results
				if (result[i] == 1) {
					found++;
					SetColor(14);
					std::cout << "\n[RESULT " << found << "]: \n";
					SetColor(7);
					PrintResult((WordType)(i + 1), result[i], outStrs[i]);
				}
			}
			if (!found) {
				Log::Warning("No result has found.\n");
			}
		}
		else { // Fill the words type
			for (int8 i = 0; i < result.GetSize(); i++) { // Print results
				if (result[i] == 1) {
					word.types.push_back(TypeAndSuffixes());
					word.types.back().type = (WordType)(i + 1);

					if (i > -1 && i < 3) { // Noun or verb and aux verb
						word.types.back().suffixes = found_words[i].suffixes;
					}
				}
			}
			if (word.types.empty()) {
				word.types.push_back(TypeAndSuffixes());
				word.types.back().type = WordType::Undefined;
			}
		}

	}

	void MainHandler::ParseSentence(const String& str) {
		String out_str(10000);

		std::vector<WordToken> words;
		// Tokenize input
		tokenize.ParseString(str);
		// Parse each sentence 
		for (uint32 i = 0; tokenize.sentences[i].Tokens() != nullptr; i++) {
			words.reserve(tokenize.sentences[i].Amount());
			// Parse each token
			for (uint32 j = 0; j < tokenize.sentences[i].Amount(); j++) {
				// Copy adress of word to words
				words.push_back(WordToken());
				words.back().data = &tokenize.sentences[i][j];
				// Parse token
				ParseMultiple(tokenize.sentences[i][j], false, words.back());
			}
			// Words to string
			out_str += "\n\x1b[38;5;198m[SENTENCE ";
			util::IntToStr(out_str, i + 1);
			out_str += "]: \n\x1b[0m";
			WordsToStr(words, out_str);

			// Reduce parse

			// Delete words in sentence
			words.clear();
		}

		// Print result
		std::cout << out_str.EndString().Chars();
		// Free tokens
		tokenize.FreeAll();
	}

	void MainHandler::ParseNoun(const String* raw_word, int32* result, TypeAndSuffixes* suffix,
		String* out_string, const bool write_result) {
		*result = noun.ParseNoun(*raw_word, *suffix, *out_string, write_result);
	}
	void MainHandler::ParseVerb(const String* raw_word, int32* result, TypeAndSuffixes* suffix,
		String* out_string, const bool write_result) {
		*result = verb.ParseVerb(*raw_word, *suffix, *out_string, write_result);
	}
	void MainHandler::ParseAuxVerb(const String* raw_word, TypeAndSuffixes* word,
		int32* result, String* out_string, const bool write_result) {
		*result = aux_verb.ParseWord(*raw_word, *word, *out_string, write_result);
	}
	void MainHandler::ParsePronoun(const String* raw_word, int32* result, String* out_string, const bool write_result) {
		*result = pronoun.ParseWord(*raw_word, *out_string, write_result);
	}
	void MainHandler::ParseAdv(const String* raw_word, int32* result, String* out_string, const bool write_result) {
		*result = adv.ParseWord(*raw_word, *out_string, write_result);
	}
	void MainHandler::ParseAdj(const String* raw_word, int32* result, String* out_string, const bool write_result) {
		*result = adj.ParseWord(*raw_word, *out_string, write_result);
	}
	void MainHandler::ParsePrepos(const String* raw_word, int32* result, String* out_string, const bool write_result) {
		*result = prepos.ParseWord(*raw_word, *out_string, write_result);
	}
	void MainHandler::ParseConj(const String* raw_word, int32* result, String* out_string, const bool write_result) {
		*result = conj.ParseWord(*raw_word, *out_string, write_result);
	}
	void MainHandler::ParseInterj(const String* raw_word, int32* result, String* out_string, const bool write_result) {
		*result = interj.ParseWord(*raw_word, *out_string, write_result);
	}
	void MainHandler::ParseDet(const String* raw_word, int32* result, String* out_string, const bool write_result) {
		*result = det.ParseWord(*raw_word, *out_string, write_result);
	}

	void MainHandler::ParsePunc(const String* raw_word, int32* result) {
		// result = 1 : if punctuation
		// result = 0 : not punc

		bool punctuation = false;

		for (uint32 i = 0; i < raw_word->Length(); i++) { // Read all chars
			if (util::IsEndingPunctuation((*raw_word)[i]) || util::IsPunctuation((*raw_word)[i])) {
				punctuation = true;
			}
			else {
				*result = 0;
				return;
			}
		}

		if (punctuation) {
			*result = 1;
		}
	}




	void MainHandler::WordsToStr(std::vector<WordToken>& words, String& out_str) {
		auto printWordType = [&](const TypeAndSuffixes& type) {
			switch (type.type)
			{
			case WordType::Undefined:
				out_str += "\x1b[38;5;3mUnknown\x1b[0m";
				break;
			case WordType::Noun:
				out_str += "\x1b[38;5;43mNoun\x1b[0m";
				break;
			case WordType::Verb:
				out_str += "\x1b[38;5;43mVerb\x1b[0m";
				break;
			case WordType::Pronoun:
				out_str += "\x1b[38;5;43mPronoun\x1b[0m";
				break;
			case WordType::Adverb:
				out_str += "\x1b[38;5;43mAdverb\x1b[0m";
				break;
			case WordType::Adjective:
				out_str += "\x1b[38;5;43mAdjective\x1b[0m";
				break;
			case WordType::Preposition:
				out_str += "\x1b[38;5;43mPreposition\x1b[0m";
				break;
			case WordType::Conjunction:
				out_str += "\x1b[38;5;43mConjunction\x1b[0m";
				break;
			case WordType::Interjection:
				out_str += "\x1b[38;5;43mInterjection\x1b[0m";
				break;
			case WordType::AuxiliaryVerb:
				out_str += "\x1b[38;5;43mAux_verb\x1b[0m";
				break;
			case WordType::Determiner:
				out_str += "\x1b[38;5;43mDeterminer\x1b[0m";
				break;
			case WordType::Punctuation:
				out_str += "\x1b[38;5;43mPunctuation\x1b[0m";
				break;
			default:
				break;
			}

			// Print suffixes if exist
			for (uint8 i = 0; i < type.suffixes.size(); i++) {
				switch (type.suffixes[i])
				{
				case verb::IrregularVerb_V2:
					out_str += "\x1b[95m:V2\x1b[0m";
					break;
				case verb::IrregularVerb_V3:
					out_str += "\x1b[95m:V3\x1b[0m";
					break;
				case noun::BaseForm:
					// out_str += "\x1b[38;5;43m:B\x1b[0m";
					break;
				case noun::S_Parsed:
					out_str += "\x1b[95m:s\x1b[0m";
					break;
				case verb::BaseForm:
					// out_str += "\x1b[38;5;43m:B\x1b[0m";
					break;
				case verb::ED_Parsed:
					out_str += "\x1b[95m:ed\x1b[0m";
					break;
				case verb::S_Parsed:
					out_str += "\x1b[95m:s\x1b[0m";
					break;
				case verb::ING_Parsed:
					out_str += "\x1b[95m:ing\x1b[0m";
					break;
				case WordType::Negative:
					out_str += "\x1b[95m:Neg\x1b[0m";
					break;
				case noun::CliticS:
					out_str += "\x1b[95m:'s\x1b[0m";
					break;
				case noun::PluralPoss:
					out_str += "\x1b[95m:s'\x1b[0m";
					break;
				default:
					out_str += "\x1b[31mUnknownType:";
					util::IntToStr(out_str, type.suffixes[i]);
					out_str += "\x1b[0m";
					break;
				}
				if (i != type.suffixes.size() - 1)
					out_str += "|";
			}

		};

		for (uint32 i = 0; i < words.size(); i++) {
			util::IntToStr(out_str, i + 1);

			out_str += "- [\x1b[38;5;231m";
			out_str += *words.at(i).data;
			out_str += "\x1b[0m]: ";
			for (uint32 j = 0; j < words[i].types.size(); j++) {
				printWordType(words[i].types[j]);
				if (j != words[i].types.size() - 1) { // If not last index
					out_str += ", ";
				}
				else {
					out_str += '.';
				}
			}
			out_str += '\n';
		}
	}

	void MainHandler::WordToStr(WordToken& word, String& out_str) {
		auto printWordType = [&](const TypeAndSuffixes& type) {
			switch (type.type)
			{
			case WordType::Undefined:
				out_str += "?";
				break;
			case WordType::Noun:
				out_str += "Noun";
				break;
			case WordType::Verb:
				out_str += "Verb";
				break;
			case WordType::Pronoun:
				out_str += "Pronoun";
				break;
			case WordType::Adverb:
				out_str += "Adverb";
				break;
			case WordType::Adjective:
				out_str += "Adjective";
				break;
			case WordType::Preposition:
				out_str += "Preposition";
				break;
			case WordType::Conjunction:
				out_str += "Conjunction";
				break;
			case WordType::Interjection:
				out_str += "Interjection";
				break;
			case WordType::AuxiliaryVerb:
				out_str += "Aux_verb";
				break;
			case WordType::Determiner:
				out_str += "Determiner";
				break;
			case WordType::Punctuation:
				out_str += "Punctuation";
				break;
			default:
				break;
			}
			// Print suffixes if exist
			for (uint8 i = 0; i < type.suffixes.size(); i++) {
				switch (type.suffixes[i])
				{
				case verb::IrregularVerb_V2:
					out_str += ":V2";
					break;
				case verb::IrregularVerb_V3:
					out_str += ":V3";
					break;
				case noun::BaseForm:
					// out_str += ":B";
					break;
				case noun::S_Parsed:
					out_str += ":s";
					break;
				case verb::BaseForm:
					// out_str += ":B";
					break;
				case verb::ED_Parsed:
					out_str += ":ed";
					break;
				case verb::S_Parsed:
					out_str += ":s";
					break;
				case verb::ING_Parsed:
					out_str += ":ing";
					break;
				case WordType::Negative:
					out_str += ":Neg";
					break;
				case noun::CliticS:
					out_str += ":'s";
					break;
				case noun::PluralPoss:
					out_str += ":s'";
					break;
				default:
					out_str += "UnknownType:";
					util::IntToStr(out_str, type.suffixes[i]);
					break;
				}
				if (i != type.suffixes.size() - 1)
					out_str += "|";
			}
		};

		for (uint32 i = 0; i < word.types.size(); i++) {
			printWordType(word.types[i]);
			if (i != word.types.size() - 1) { // If not last index
				out_str += ", ";
			}
			else {
				out_str += '.';
			}
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
		case AuxiliaryVerb:
			switch (result)
			{
			case 1:
				Log::Info("Auxiliary Verb has been found:\n") << str.Chars();
				break;
			case -1:
				Log::Warning("No Auxiliary Verb has been found.\n");
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
		case Determiner:
			switch (result)
			{
			case 1:
				Log::Info("Determiner has been found:\n") << str.Chars();
				break;
			case -1:
				Log::Warning("No determiner has been found.\n");
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
		case Punctuation:
			Log::Info("Punctuation has been found:\n");
			return;
		default:
			return;
		}
	}

}