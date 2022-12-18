#include "simple_types.hpp"
#include "misc/util.hpp"

#include "log.hpp"

#define MIN_WORD_AMOUNT 128
#define WORD_INCR_COEF 2

namespace basic {
	BasicType::BasicType() {
		words = new Word[MIN_WORD_AMOUNT];
		amount = 0;
		capacity = MIN_WORD_AMOUNT;
	}
	BasicType::~BasicType() {
		FreeAll();
	}
	void BasicType::FreeAll() {
		delete[] words;
		words = nullptr;
		amount = 0;
		capacity = 0;
	}

	void BasicType::IncreaseSpace() {
		// Increase capacity value
		if (capacity == 0) {
			capacity = MIN_WORD_AMOUNT;
		}
		else {
			capacity *= WORD_INCR_COEF;
		}

		// Hold old pointer
		Word* old = words;
		// Create new space
		words = new Word[capacity];
		// Copy old data to new one
		util::MemCpy(words, old, amount);
		// Free old space
		delete[] old;
	}

	int8 BasicType::AddWord(const char* word_chars, const uint32& length) {
		// return 2   : Size is smaller than 1
		// return 1  :  The word added successfully
		// return -1  : There is some charachter which is not alphabetic
		// return -2  : Character size exceeds WORD_CHAR_SIZE size
		// return -4  : The verb is already exist

		char temp[WORD_CHAR_SIZE];
		uint8 lastTempIndex = 0;
		bool commandLine = false;

		for (uint32 i = 0; i < length; i++) { // Read all chars
			char letter = util::ToLowerCase(word_chars[i]);
			if (letter != 0) {
				temp[lastTempIndex] = letter;
				lastTempIndex += 1; // Increase length by one
			}
			else if (word_chars[i] == '-') {
				temp[lastTempIndex] = letter;
				lastTempIndex += 1; // Increase length by one
			}
			else if (word_chars[i] == ' ' || word_chars[i] == '\t') {
				continue;
			}
			else if (word_chars[i] == '#') {
				commandLine = true;
				break;
			}
			else if (word_chars[i] == '\0' || word_chars[i] == '\n') {
				break;
			}
			else {
				return -1; // There is some charachter which is not alphabetic
			}
		}

		if (lastTempIndex > WORD_CHAR_SIZE) { // Size control
			return -2;
		}
		else if (lastTempIndex < 1) { // Size control
			return 2;
		}
		else if (FindWord(temp, lastTempIndex) != -1) { // Existence control
			return -4;
		}

		if (amount + 1 > capacity) { // If capacity is not enough
			IncreaseSpace();
		}
		// Copy data to words array
		util::MemCpy(words[amount].chars, temp, lastTempIndex);
		words[amount].length = lastTempIndex;

		// Increase amount by one
		amount += 1;

		return 1;
	}

	void BasicType::MultipleAdder(const char* file, const uint64& line_length, const char* type) {
		uint32 successfull = 0;
		uint32 nonAlphabetic = 0;
		uint32 charExceedsSize = 0;
		uint32 charSizeSmaller1 = 0;
		uint32 notUnique = 0;

		String nonAlphabeticLines;
		String exceedsSizeLines;
		String sizeSmaller1CharLines;
		String nonUniqueLines;

		auto calculateLineLength = [](const char& line) {
			uint32 i = 0;
			while ((&line)[i] != '\r' && (&line)[i] != '\n' && (&line)[i] != '\0') {
				i++;
			}
			return i;
		};

		uint32 lineNumber = 1;
		uint32 lineLength = 0;

		for (uint64 i = 0; i < line_length; i++) {

			lineLength = calculateLineLength(file[i]);
			if (lineLength == 0)
				continue;

			switch (AddWord(&file[i], lineLength)) {
			case 1:
				successfull++;
				break;
			case -1: // There is some charachter which is not alphabetic
				nonAlphabetic++;
				util::IntToStr(nonAlphabeticLines, lineNumber);
				nonAlphabeticLines += ',';
				break;
			case -2: // Character size exceeds WORD_CHAR_SIZE size
				charExceedsSize++;
				util::IntToStr(exceedsSizeLines, lineNumber);
				exceedsSizeLines += ',';
				break;
			case -3: // Character size smaller than 1 characters
				charSizeSmaller1++;
				util::IntToStr(sizeSmaller1CharLines, lineNumber);
				sizeSmaller1CharLines += ',';
				break;
			case -4: // The verb is already exist
				notUnique++;
				util::IntToStr(nonUniqueLines, lineNumber);
				nonUniqueLines += ',';
				break;
			default:
				break;
			}

			i += lineLength;
			lineNumber++;
		}

		// Print Results
		if (successfull != 0) {
			Log::Info(successfull) << " " << type << " added successfully.\n";
		}
		if (nonAlphabetic != 0) {
			Log::Error(nonAlphabetic) << " " << type << " contain non-alphabetic characters.\n" <<
				"These lines; " << nonAlphabeticLines.EndString().Chars() << "\n";
		}
		if (charExceedsSize != 0) {
			Log::Error(charExceedsSize) << " " << type << " exceed " << WORD_CHAR_SIZE << " characters.\n" <<
				"These lines; " << exceedsSizeLines.EndString().Chars() << "\n";
		}
		if (charSizeSmaller1 != 0) {
			Log::Error(charSizeSmaller1) << " " << type << " are smaller than 2 characters.\n" <<
				"These lines; " << sizeSmaller1CharLines.EndString().Chars() << "\n";
		}
		if (notUnique != 0) {
			Log::Error(notUnique) << " " << type << " are not unique.\n" <<
				"These lines; " << nonUniqueLines.EndString().Chars() << "\n";
		}
	}

	int32 BasicType::FindWord(const char* word_chars, const uint8& length) {
		for (uint32 i = 0; i < amount; i++) { // Check all words
			if (length == words[i].length) { // If they have same length
				if (util::IsSameArray(word_chars, words[i].chars, length)) {
					return i;
				}
			}
		}
		return -1;
	}

	int32 BasicType::ParseWord(const char* raw_word, const uint8& length, String& out_string) {
		// return  1  : word found
		// return -1  : The word has not found
		// return -2  : Character size exceeds WORD_CHAR_SIZE size
		// return -3  : Character size smaller than 1 characters. Empty.
		// return -4  : There is some character which is not alphabetic

		char temp[WORD_CHAR_SIZE];
		uint8 lastTempIndex = 0;

		for (uint32 i = 0; i < length; i++) { // Read all chars
			char letter = util::ToLowerCase(raw_word[i]);
			if (letter != 0) {
				temp[lastTempIndex] = letter;
				lastTempIndex += 1; // Increase length by one
			}
			else if (raw_word[i] == '-') {
				temp[lastTempIndex] = letter;
				lastTempIndex += 1; // Increase length by one
			}
			else if (raw_word[i] == ' ' || raw_word[i] == '\t') {
				continue;
			}
			else if (raw_word[i] == '#' || raw_word[i] == '\0' || raw_word[i] == '\n') {
				break;
			}
			else {
				return -4; // There is some charachter which is not alphabetic
			}
		}

		if (lastTempIndex > WORD_CHAR_SIZE) { // Size control
			return -2;
		}
		else if (lastTempIndex < 1) { // Size control
			return -3;
		}


		int32 id = FindWord(temp, lastTempIndex);
		if (id != -1) {
			out_string += "1. ";
			out_string.Append(words[id].chars, words[id].length);
			out_string += '\n';
			out_string.EndString();

			return 1;
		}
		else
			return -1;
	}

}