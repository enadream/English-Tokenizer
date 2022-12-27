#include "simple_types.hpp"
#include "misc/util.hpp"

#include "log.hpp"

#define MIN_WORD_AMOUNT_UNIDEXED 128
#define MIN_WORD_AMOUNT_INDEXED 64
#define WORD_INCR_COEF 4


namespace basic {
#pragma region UnindexedList
	UnindexedList::UnindexedList() {
		words = new SimpleWord[MIN_WORD_AMOUNT_UNIDEXED];
		amount = 0;
		capacity = MIN_WORD_AMOUNT_UNIDEXED;
	}
	UnindexedList::~UnindexedList() {
		FreeAll();
	}
	void UnindexedList::FreeAll() {
		delete[] words;
		words = nullptr;
		amount = 0;
		capacity = 0;
	}

	void UnindexedList::IncreaseSpace() {
		// Increase capacity value
		if (capacity == 0) {
			capacity = MIN_WORD_AMOUNT_UNIDEXED;
		}
		else {
			capacity *= WORD_INCR_COEF;
		}

		// Hold old pointer
		SimpleWord* old = words;
		// Create new space
		words = new SimpleWord[capacity];
		// Copy old data to new one
		util::MemCpy(words, old, amount);
		// Free old space
		delete[] old;
	}

	int8 UnindexedList::AddWord(const char* word_chars, const uint32& length) {
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
				if (lastTempIndex < WORD_CHAR_SIZE) // Length control before adding the temp array
					temp[lastTempIndex++] = letter;
				else
					return -2;
			}
			else if (word_chars[i] == '-') {
				if (lastTempIndex < WORD_CHAR_SIZE) // Length control before adding the temp array
					temp[lastTempIndex++] = word_chars[i];
				else
					return -2;
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

	void UnindexedList::MultipleAdder(const char* file, const uint64& line_length, const char* type) {
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

	int32 UnindexedList::FindWord(const char* word_chars, const uint8& length) {
		for (uint32 i = 0; i < amount; i++) { // Check all words
			if (length == words[i].length) { // If they have same length
				if (util::IsSameArray(word_chars, words[i].chars, length)) {
					return i;
				}
			}
		}
		return -1;
	}

	int8 UnindexedList::ParseWord(const String& raw_string, String& out_string, const bool write_result) {
		// return  1  : word found
		// return -1  : The word has not found
		// return -2  : Character size exceeds WORD_CHAR_SIZE size
		// return -3  : Character size smaller than 1 characters. Empty.
		// return -4  : There is some character which is not alphabetic

		char temp[WORD_CHAR_SIZE];
		uint8 lastTempIndex = 0;

		for (uint32 i = 0; i < raw_string.Length(); i++) { // Read all chars
			char letter = util::ToLowerCase(raw_string[i]);
			if (letter != 0) {
				temp[lastTempIndex] = letter;
				lastTempIndex += 1; // Increase length by one
			}
			else if (raw_string[i] == '-') {
				temp[lastTempIndex] = letter;
				lastTempIndex += 1; // Increase length by one
			}
			else if (raw_string[i] == ' ' || raw_string[i] == '\t') {
				continue;
			}
			else if (raw_string[i] == '#' || raw_string[i] == '\0' || raw_string[i] == '\n') {
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
			if (write_result) {
				out_string += "1. ";
				out_string.Append(words[id].chars, words[id].length);
				out_string += '\n';
				out_string.EndString();
			}
			return 1;
		}
		else
			return -1;
	}

#pragma endregion UnindexedList

#pragma region IndexedList
	IndexedList::IndexedList() {
		wordLists = new WordList[WORD_COLUMN * WORD_ROW];

		for (uint32 row = 0; row < WORD_ROW; row++) {
			for (uint32 col = 0; col < WORD_COLUMN; col++) {
				wordLists[row * WORD_COLUMN + col].words = nullptr;
				wordLists[row * WORD_COLUMN + col].amount = 0;
				wordLists[row * WORD_COLUMN + col].capacity = 0;
				wordLists[row * WORD_COLUMN + col].indicator[0] = 'a' + row;

				if (col < 26)
					wordLists[row * WORD_COLUMN + col].indicator[1] = 'a' + col;
				else
					wordLists[row * WORD_COLUMN + col].indicator[1] = '-';
			}
		}
	}

	IndexedList::~IndexedList() {
		// Delete each word heap
		for (int row = 0; row < WORD_ROW; row++) {
			for (int col = 0; col < WORD_ROW; col++) {
				if (wordLists[row * WORD_COLUMN + col].words != nullptr)
					delete[] wordLists[row * WORD_COLUMN + col].words;
			}
		}
		// Delete container
		delete[] wordLists;
	}

	void IndexedList::FreeAll() {
		// Delete each word heap
		for (int row = 0; row < WORD_ROW; row++) {
			for (int col = 0; col < WORD_ROW; col++) {
				if (wordLists[row * WORD_COLUMN + col].words != nullptr) {
					delete[] wordLists[row * WORD_COLUMN + col].words;
					wordLists[row * WORD_COLUMN + col].capacity = 0;
					wordLists[row * WORD_COLUMN + col].amount = 0;
					wordLists[row * WORD_COLUMN + col].words = nullptr;
				}
			}
		}
	}

	int8 IndexedList::AddWord(const char* word_chars, const uint32& length) {
		// return 2   : Empty line
		// return 1   : The word added successfully
		// return -1  : There is some charachter which is not alphabetic
		// return -2  : Character size exceeds WORD_CHAR_SIZE size
		// return -3  : Character size smalle than 2
		// return -4  : The verb is already exist

		char temp[WORD_CHAR_SIZE];
		uint8 lastTempIndex = 0;
		bool commandLine = false;

		for (uint32 i = 0; i < length; i++) { // Read all chars
			char letter = util::ToLowerCase(word_chars[i]);
			if (letter != 0) {
				if (lastTempIndex < WORD_CHAR_SIZE) // Length control before adding the temp array
					temp[lastTempIndex++] = letter;
				else
					return -2;
			}
			else if (word_chars[i] == '-' && lastTempIndex > 0) {
				if (lastTempIndex < WORD_CHAR_SIZE) // Length control before adding the temp array
					temp[lastTempIndex++] = word_chars[i];
				else
					return -2;
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
		else if (lastTempIndex == 1) { // Size control
			return -3;
		}
		else if (lastTempIndex == 0) { // Size control
			return 2;
		}
		else if (FindWord(temp, lastTempIndex) != nullptr) { // Existence control
			return -4;
		}

		// Create word
		CreateWord(temp, lastTempIndex);

		return 1;
	}

	SimpleWord* IndexedList::CreateWord(const char* word_chars, const uint32& str_length) {
		// Detect the index
		uint32 row = word_chars[0] - 'a';
		uint32 col;

		if (word_chars[1] == '-')
			col = 26;
		else
			col = word_chars[1] - 'a';

		uint32 id = row * WORD_COLUMN + col;

		// Check capacity
		if (wordLists[id].amount + 1 > wordLists[id].capacity) {
			// If capacity is zero
			if (wordLists[id].capacity == 0) {
				if (col == 26) // index for '-' i.e "x-"
					wordLists[id].capacity = MIN_WORD_AMOUNT_INDEXED / 4;
				else
					wordLists[id].capacity = MIN_WORD_AMOUNT_INDEXED;
				// Create new heap
				wordLists[id].words = new SimpleWord[wordLists[id].capacity];
			}
			else {
				// Hold old pointer
				SimpleWord* old_words = wordLists[id].words;
				// Update Capacity
				wordLists[id].capacity *= WORD_INCR_COEF;
				// Create new heap
				wordLists[id].words = new SimpleWord[wordLists[id].capacity];
				// Copy old data
				util::MemCpy(wordLists[id].words, old_words, wordLists[id].amount);
				// Delete old heap
				delete[] old_words;
			}
		}

		// Copy data to last element
		util::MemCpy(wordLists[id].words[wordLists[id].amount].chars, word_chars, str_length);
		wordLists[id].words[wordLists[id].amount].length = str_length;
		wordLists[id].amount += 1;

		// Return the last element
		return &wordLists[id].words[wordLists[id].amount - 1];
	}

	void IndexedList::MultipleAdder(const char* file, const uint64& line_length, const char* type) {
		uint32 successfull = 0;
		uint32 nonAlphabetic = 0;
		uint32 charExceedsSize = 0;
		uint32 charSizeSmaller2 = 0;
		uint32 notUnique = 0;

		String nonAlphabeticLines;
		String exceedsSizeLines;
		String sizeSmaller2CharLines;
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
			case -3: // Character size smaller than 2 characters
				charSizeSmaller2++;
				util::IntToStr(sizeSmaller2CharLines, lineNumber);
				sizeSmaller2CharLines += ',';
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
		if (charSizeSmaller2 != 0) {
			Log::Error(charSizeSmaller2) << " " << type << " are smaller than 2 characters.\n" <<
				"These lines; " << sizeSmaller2CharLines.EndString().Chars() << "\n";
		}
		if (notUnique != 0) {
			Log::Error(notUnique) << " " << type << " are not unique.\n" <<
				"These lines; " << nonUniqueLines.EndString().Chars() << "\n";
		}
	}

	SimpleWord* IndexedList::FindWord(const char* word_chars, const uint8& str_length) {
		// Detect the index
		uint32 row = word_chars[0] - 'a';
		uint32 col;

		if (word_chars[1] == '-')
			col = 26;
		else
			col = word_chars[1] - 'a';

		uint32 id = row * WORD_COLUMN + col;

		for (uint32 i = 0; i < wordLists[id].amount; i++) { // Check all words
			if (str_length == wordLists[id].words[i].length) { // If they have same length
				if (util::IsSameArray(word_chars, wordLists[id].words[i].chars, str_length)) {
					return &wordLists[id].words[i];
				}
			}
		}
		return nullptr;
	}

	int8 IndexedList::ParseWord(const String& raw_string, String& out_string, const bool write_result) {
		// return  1  : word found
		// return -1  : The word has not found
		// return -2  : Character size exceeds WORD_CHAR_SIZE size
		// return -3  : Character size smaller than 2 characters. Empty.
		// return -4  : There is some character which is not alphabetic

		char temp[WORD_CHAR_SIZE];
		uint8 lastTempIndex = 0;

		for (uint32 i = 0; i < raw_string.Length(); i++) { // Read all chars
			char letter = util::ToLowerCase(raw_string[i]);
			if (letter != 0) {
				temp[lastTempIndex] = letter;
				lastTempIndex += 1; // Increase length by one
			}
			else if (raw_string[i] == '-') {
				temp[lastTempIndex] = raw_string[i];
				lastTempIndex += 1; // Increase length by one
			}
			else if (raw_string[i] == ' ' || raw_string[i] == '\t') {
				continue;
			}
			else if (raw_string[i] == '#' || raw_string[i] == '\0' || raw_string[i] == '\n') {
				break;
			}
			else {
				return -4; // There is some charachter which is not alphabetic
			}
		}

		if (lastTempIndex > WORD_CHAR_SIZE) { // Size control
			return -2;
		}
		else if (lastTempIndex < 2) { // Size control
			return -3;
		}


		SimpleWord* found_word = FindWord(temp, lastTempIndex);

		if (found_word != nullptr) {
			if (write_result) {
				out_string += "1. ";
				out_string.Append(found_word->chars, found_word->length);
				out_string += '\n';
				out_string.EndString();
			}
			return 1;
		}
		else
			return -1;
	}
#pragma endregion IndexedList

}