#include "noun.hpp"
#include "misc/util.hpp"
#include "log.hpp"

#define MIN_NOUN_SIZE 32  // 32 default
#define NOUN_ROW 26
#define NOUN_COL 27
#define NOUN_SIZE_INC_COEF 4 // 4 default

namespace noun {
	NounHandler::NounHandler() {
		nounLists = new NounList[NOUN_ROW * NOUN_COL];
		irrNounList.nouns = nullptr;
		irrNounList.amount = 0;
		irrNounList.capacity = 0;

		for (uint32 row = 0; row < NOUN_ROW; row++) {
			for (int32 col = 0; col < NOUN_COL; col++) {
				nounLists[row * NOUN_COL + col].nouns = nullptr;
				nounLists[row * NOUN_COL + col].amount = 0;
				nounLists[row * NOUN_COL + col].capacity = 0;
				nounLists[row * NOUN_COL + col].indicator[0] = 'a' + row;

				if (col < 26)
					nounLists[row * NOUN_COL + col].indicator[1] = 'a' + col;
				else // i.e x-ray
					nounLists[row * NOUN_COL + col].indicator[1] = '-';


			}
		}
	}

	NounHandler::~NounHandler() {
		// Delete each noun heap
		for (int row = 0; row < NOUN_ROW; row++) {
			for (int col = 0; col < NOUN_COL; col++) {
				if (nounLists[row * NOUN_COL + col].nouns != nullptr)
					delete[] nounLists[row * NOUN_COL + col].nouns;
			}
		}
		// Delete container
		delete[] nounLists;
		// Delete irregular noun collections
		if (irrNounList.nouns != nullptr)
			delete[] irrNounList.nouns;
	}

	void NounHandler::Free() {
		for (int row = 0; row < NOUN_ROW; row++) {
			for (int col = 0; col < NOUN_COL; col++) {
				if (nounLists[row * NOUN_COL + col].nouns != nullptr) {
					delete[] nounLists[row * NOUN_COL + col].nouns;
					nounLists[row * NOUN_COL + col].capacity = 0;
					nounLists[row * NOUN_COL + col].amount = 0;
					nounLists[row * NOUN_COL + col].nouns = nullptr;
				}
			}
		}

		// Delete Irregular records
		if (irrNounList.nouns != nullptr) {
			delete[] irrNounList.nouns;
			irrNounList.nouns = nullptr;
			irrNounList.amount = 0;
			irrNounList.capacity = 0;
		}
	}

#pragma region Private Functions
	void NounHandler::CheckException_S(Noun& noun) const {
		// None, EndsWith_Cy, EndsWith_ss, EndsWith_zz, EndsWith_ch, EndsWith_sh, EndsWith_s, EndsWith_z, EndsWith_x, EndsWith_o

		// Check if second to last character is consonant and last char is y
		if (!util::IsVowel(noun.chars[noun.length - 2]) && noun.chars[noun.length - 1] == 'y') {
			noun.s = Suffix_0y_ies; // EndsWith_Cy
			return;
		}
		else if (noun.chars[noun.length - 1] == 's') { // When last character is s
			if (noun.chars[noun.length - 2] == 's') // when verb ends with ss
				noun.s = Suffix_es; // EndsWith_ss
			else
				noun.s = Suffix_ses; // EndsWith_s
			return;
		}
		else if (noun.chars[noun.length - 1] == 'z') { // When last character is z
			if (noun.chars[noun.length - 2] == 'z') // when verb ends with zz
				noun.s = Suffix_es; // EndsWith_zz
			else
				noun.s = Suffix_zes; // EndsWith_z
			return;
		}
		else if (noun.chars[noun.length - 1] == 'h') { // When last character is h
			if (noun.chars[noun.length - 2] == 's') {// when verb ends with sh
				noun.s = Suffix_es; // EndsWith_sh
				return;
			}
			else if (noun.chars[noun.length - 2] == 'c') { // when verb ends with ch
				noun.s = Suffix_es; // EndsWith_ch
				return;
			}
		}
		else if (noun.chars[noun.length - 1] == 'x') { // When last character x
			noun.s = Suffix_es; // EndsWith_x
			return;
		}
		else if (noun.chars[noun.length - 1] == 'o') { // When last character o
			noun.s = Suffix_es; // EndsWith_o
			return;
		}

		noun.s = None;
	}

	uint8 NounHandler::S_Parser(const char* noun_chars, const uint8& length, std::vector<Noun*>& out_nouns) const {
		// None, EndsWith_Cy, EndsWith_ss, EndsWith_zz, EndsWith_ch, EndsWith_sh, EndsWith_s, EndsWith_z, EndsWith_x, EndsWith_o
		// -s		-(-y)ies	-es			   -es			-es			-es			 -ses		 -zes	,	 -es    ,	-es

		uint8 foundNouns = 0;
		if (length > 2) { // the input text has to be at least 3 chars
			if (noun_chars[length - 1] == 's') { // input str ends with -s
				foundNouns += FindWithException(noun_chars, length - 1, None, out_nouns);
				if (foundNouns > 0) // Return if found
					return foundNouns;

				if (noun_chars[length - 2] == 'e') { // input str ends with -es
					{
						foundNouns += FindWithException(noun_chars, length - 2, Suffix_es, out_nouns);
						if (foundNouns > 0) // Return if found
							return foundNouns;
					}
					if (noun_chars[length - 3] == 'i') { // input str ends with -ies
						char tempNoun[NOUN_CHAR_SIZE]; // Creating a temporary space for noun
						util::MemCpy(tempNoun, noun_chars, length - 3); // copying charachters excluding last 3
						tempNoun[length - 3] = 'y'; // adding e to the last char of the noun 

						// exceptions.s = Suffix_0y_ies; // setting the suffix type
						foundNouns += FindWithException(tempNoun, length - 2, Suffix_0y_ies, out_nouns);
						if (foundNouns > 0) // Return if found
							return foundNouns;
					}
					if (length > 4) {
						if (noun_chars[length - 3] == 's' && noun_chars[length - 4] == 's') { // input str ends with -sses
							foundNouns += FindWithException(noun_chars, length - 3, Suffix_ses, out_nouns);
							if (foundNouns > 0) // Return if found
								return foundNouns;
						}
						else if (noun_chars[length - 3] == 'z' && noun_chars[length - 4] == 'z') { // input str ends with -zzes
							foundNouns += FindWithException(noun_chars, length - 3, Suffix_zes, out_nouns);
							if (foundNouns > 0) // Return if found
								return foundNouns;
						}
					}
				}
			}
		}

		return foundNouns;
	}

	uint8 NounHandler::Pos_Parser(const char* noun_chars, const uint8& length, std::vector<Noun*>& out_nouns) const {
		// Return 0: no result
		// Return 1: is or possessive
		// Return 2: plural + possessive

		uint8 foundNouns = 0;

		if (length > 3) {
			if (noun_chars[length - 2] == '\'' && noun_chars[length - 1] == 's') { // ends with 's, is or possesive
				foundNouns = FindNoun(noun_chars, length - 2, &out_nouns); // Find noun
				if (foundNouns > 0)
					return 1;
			}
			else if (noun_chars[length - 1] == '\'') { // plural + possesive 
				foundNouns = S_Parser(noun_chars, length - 1, out_nouns); // Plural possesesive
				if (foundNouns > 0)
					return 2;
			}
		}

		return foundNouns;
	}

	void NounHandler::CreateIrrNoun(Noun& noun) {
		if (&noun == nullptr) {
			// Do nothing
		}
		else if (noun.s == IrrSingular) { // Create new collection
			if (irrNounList.capacity > irrNounList.amount) { // If there is enough space for new collection
				// Do nothing
			}
			else { // IF space is not enough
				if (irrNounList.capacity > 0) { // If current capacity bigger than 0
					// Update new capacity
					irrNounList.capacity *= NOUN_SIZE_INC_COEF;
					// Hold old pointer
					IrregularNoun* p_old_data = irrNounList.nouns;
					// Create new heap space
					irrNounList.nouns = new IrregularNoun[irrNounList.capacity];
					// Copy Old data to new one
					util::MemCpy(irrNounList.nouns, p_old_data, irrNounList.amount);
					// Free old space
					delete[] p_old_data;
				}
				else { // init a new collection
					irrNounList.nouns = new IrregularNoun[MIN_NOUN_SIZE];
					irrNounList.capacity = MIN_NOUN_SIZE;
				}
			}

			// Assign verb address to base form and make plural form null
			irrNounList.nouns[irrNounList.amount].singularForm = &noun;
			irrNounList.nouns[irrNounList.amount].pluralForm = nullptr;
			// Increase irregular noun collection amount by 1
			irrNounList.amount += 1;
		}
		else if (noun.s == IrrPlural) { // use the last collection plural form
			irrNounList.nouns[irrNounList.amount - 1].pluralForm = &noun;
		}
	}
	int8 NounHandler::UpdateIrrNounAdress(Noun& new_address, const Noun& old_adress) {
		for (uint16 i = 0; i < irrNounList.amount; i++) {
			if (new_address.s == IrrSingular) {
				if (irrNounList.nouns[i].singularForm == &old_adress) {
					irrNounList.nouns[i].singularForm = &new_address;
					return 1;
				}
			}
			else if (new_address.s == IrrPlural) {
				if (irrNounList.nouns[i].pluralForm == &old_adress) {
					irrNounList.nouns[i].pluralForm = &new_address;
					return 1;
				}
			}
		}

		return -1; // Verb couln't found in irregular verbs
	}
#pragma endregion

	int8 NounHandler::AddNoun(const char* word_chars, const uint32& length) {
		// return 2   : Command line
		// return 1  :  The word added successfully
		// return -1  : There is some charachter which is not alphabetic
		// return -2  : Character size exceeds WORD_CHAR_SIZE size
		// return -3  : Character size smaller than 2 chars
		// return -4  : The verb is already exist

		char temp[NOUN_CHAR_SIZE];
		uint8 lastTempIndex = 0;
		bool commanLine = false;
		bool commaAppeared = false;

		for (uint32 i = 0; i < length; i++) { // Read all chars
			char letter = util::ToLowerCase(word_chars[i]);
			if (letter != 0) {
				if (lastTempIndex < NOUN_CHAR_SIZE) // Length control before adding the temp array
					temp[lastTempIndex++] = letter;
				else
					return -2;
			}
			else if (word_chars[i] == ' ' || word_chars[i] == '\t') {
				continue;
			}
			else if (word_chars[i] == '-' && lastTempIndex != 0) {
				if (lastTempIndex < NOUN_CHAR_SIZE) // Length control before adding the temp array
					temp[lastTempIndex++] = word_chars[i];
				else
					return -2;
			}
			else if (word_chars[i] == '#') { // Command line
				commanLine = true;
				break;
			}
			else if (word_chars[i] == ',') { // Irregular Noun
				commaAppeared = true;

				if (lastTempIndex > NOUN_CHAR_SIZE) { // Size control
					return -2;
				}
				else if (lastTempIndex == 0) { // empty line
					return 2;
				}
				else if (lastTempIndex < 2) { // Size control
					return -3;
				}
				if (FindNoun(temp, lastTempIndex) != 0) { // Existence control
					return -4;
				}

				// Create new noun and irregular noun
				CreateIrrNoun(CreateNewNoun(temp, lastTempIndex, IrrSingular));
				// Free temp array
				lastTempIndex = 0;
			}
			else if (word_chars[i] == '\0' || word_chars[i] == '\n') {
				break;
			}
			else {
				return -1; // There is some charachter which is not alphabetic
			}
		}

		if (lastTempIndex > NOUN_CHAR_SIZE) { // Size control
			return -2;
		}
		else if (lastTempIndex == 0) { // empty line
			return 2;
		}
		else if (lastTempIndex < 2) { // Size control
			return -3;
		}

		if (commaAppeared) {
			// Create new noun and irregular noun
			CreateIrrNoun(CreateNewNoun(temp, lastTempIndex, IrrPlural));
		}
		else {
			if (FindNoun(temp, lastTempIndex) != 0) { // Existence control
				return -4;
			}
			// Create new noun
			CreateNewNoun(temp, lastTempIndex, Undefined);
		}

		return 1;
	}

	Noun& NounHandler::CreateNewNoun(const char* noun_chars, const uint32& str_length, const Exception& exception_p) {
		int row = noun_chars[0] - 'a'; // 97 means 'a' in ASCII code
		int col;


		if (noun_chars[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = noun_chars[1] - 'a'; // 97 means 'a' in ASCII code

		int id = row * NOUN_COL + col;

		// Check index list size
		if (nounLists[id].amount + 1 > nounLists[id].capacity) {

			if (nounLists[id].capacity > 0) { // If current capacity bigger than 0
				// Update new capacity
				nounLists[id].capacity = nounLists[id].capacity * NOUN_SIZE_INC_COEF;
				// Hold old pointer
				Noun* p_old_data = nounLists[id].nouns;
				// Create new verbs array
				nounLists[id].nouns = new Noun[nounLists[id].capacity];
				// Copy Old data to new array
				for (uint32 item = 0; item < nounLists[id].amount; item++) {
					// Copy data
					nounLists[id].nouns[item] = p_old_data[item];
					// If the verb is irregular update the adress
					if (nounLists[id].nouns[item].s == IrrSingular ||
						nounLists[id].nouns[item].s == IrrPlural) {
						UpdateIrrNounAdress(nounLists[id].nouns[item], p_old_data[item]);
					}

				}
				// Free old space
				delete[] p_old_data;
			}
			else { // Initialize new heap array
				// Set capacity
				if (col == 26) { // index for '-' i.e "x-"
					nounLists[id].capacity = MIN_NOUN_SIZE / 2;
				}
				else if (col == 27) { // index for ' ' i.e "a" one char noun
					nounLists[id].capacity = 1;
				}
				else {
					nounLists[id].capacity = MIN_NOUN_SIZE;
				}

				// Create a space
				nounLists[id].nouns = new Noun[nounLists[id].capacity];
			}
		}

		// Copy chars to buffer verb and increase the size of verb
		util::MemCpy(nounLists[id].nouns[nounLists[id].amount].chars, noun_chars, str_length);
		nounLists[id].nouns[nounLists[id].amount].length = str_length;

		// Check Exception -S 
		if (exception_p == IrrSingular) {
			// Change regularity of the noun
			nounLists[id].nouns[nounLists[id].amount].s = IrrSingular;
		}
		else if (exception_p == IrrPlural) {
			// Change regularity of the noun
			nounLists[id].nouns[nounLists[id].amount].s = IrrPlural;
		}
		else { // When the noun is regular
			// Check S exception
			if (exception_p == Undefined) {
				CheckException_S(nounLists[id].nouns[nounLists[id].amount]);
			}
			else {
				nounLists[id].nouns[nounLists[id].amount].s = exception_p;
			}

		}

		// Increase last empty index by one
		nounLists[id].amount += 1;

		// Return the newly added noun's adress
		return nounLists[id].nouns[nounLists[id].amount - 1];
	}

	void NounHandler::MultipleAdder(const char* file, const uint64& line_length) {
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

			switch (AddNoun(&file[i], lineLength)) {
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
			Log::Info(successfull) << " nouns added successfully.\n";
		}
		if (nonAlphabetic != 0) {
			Log::Error(nonAlphabetic) << " nouns contain non-alphabetic characters.\n" <<
				"These lines; " << nonAlphabeticLines.EndString().Chars() << "\n";
		}
		if (charExceedsSize != 0) {
			Log::Error(charExceedsSize) << " nouns exceed " << NOUN_CHAR_SIZE << " characters.\n" <<
				"These lines; " << exceedsSizeLines.EndString().Chars() << "\n";
		}
		if (charSizeSmaller1 != 0) {
			Log::Error(charSizeSmaller1) << " nouns are smaller than 2 characters.\n" <<
				"These lines; " << sizeSmaller1CharLines.EndString().Chars() << "\n";
		}
		if (notUnique != 0) {
			Log::Error(notUnique) << " nouns are not unique.\n" <<
				"These lines; " << nonUniqueLines.EndString().Chars() << "\n";
		}
	}

	uint8 NounHandler::FindNoun(const char* word_chars, const uint8& length, std::vector<Noun*>* out_nouns) const {
		int row = word_chars[0] - 'a'; // 97 means 'a' in ASCII code
		int col;

		int8 foundAmount = 0;

		if (word_chars[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = word_chars[1] - 'a'; // 97 means 'a' in ASCII code


		int32 id = row * NOUN_COL + col;

		for (int32 i = 0; i < nounLists[id].amount; i++) { // Get all nouns from buffer
			if (nounLists[id].nouns[i].length == length) {
				if (util::IsSameArray(nounLists[id].nouns[i].chars, word_chars, length)) {
					if (out_nouns == nullptr) {
						return 1;
					}
					else {
						out_nouns->push_back(&nounLists[id].nouns[i]);
						foundAmount += 1;
					}
				}
			}
		}

		return foundAmount;
	}

	uint8 NounHandler::FindWithException(const char* noun_chars, const int& length, Exception ex_type, std::vector<Noun*>& out_nouns) const {
		int row = noun_chars[0] - 'a'; // 97 means 'a' in ASCII code
		int col;

		if (noun_chars[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = noun_chars[1] - 'a'; // 97 means 'a' in ASCII code

		int32 id = row * NOUN_COL + col;
		uint8 foundAmount = 0;

		for (int i = 0; i < nounLists[id].amount; i++) { // Get all verbs from buffer
			//If they have same lenght
			if (nounLists[id].nouns[i].length == length) {
				// If they have same type
				if (ex_type == nounLists[id].nouns[i].s) {
					// If they have same content
					if (util::IsSameArray(nounLists[id].nouns[i].chars, noun_chars, length)) {
						out_nouns.push_back(&nounLists[id].nouns[i]);
						foundAmount++;
					}
				}
			}
		}

		return foundAmount;
	}

	int8 NounHandler::ParseNoun(const String& raw_string, TypeAndSuffixes& word, String& out_string, const bool write_result) const {
		// return 2   : Empty line
		// return -1  : No verb found.
		// return +1  : The verb Successfully found.
		// return -2  : There is some characters which is not alphabetic
		// return -3  : First character cannot be hyphen
		// return -4  : Character size less than 2 characters
		// return -5  : Character size exceeds VERB_CHAR_SIZE size

		char noun_chars[150];
		uint8 length = 0;
		std::vector<Noun*> found_nouns;

		auto writeNoun = [&](Noun& noun, uint8 index) {
			util::IntToStr(out_string, index + 1);
			out_string += ". ";
			out_string.Append(noun.chars, noun.length);

			out_string += "\t[Sfx_S]: ";
			ExceptionToStr(noun.s, out_string);

			out_string += '\n';
			out_string.EndString();
		};

		// Make all chars lowercase and get rid of spaces
		for (int i = 0; i < raw_string.Length(); i++) {
			char currentCh = util::ToLowerCase(raw_string[i]);

			if (currentCh != 0) // If the char is alphabetic
				noun_chars[length++] = currentCh;
			else if (raw_string[i] == ' ' || raw_string[i] == '\t')
				continue;
			else if (raw_string[i] == '-' || raw_string[i] == '\'') { // If the charachter is hyphen or single quote 
				if (i == 0)
					return -3; // First character cannot be hyphen
				else
					noun_chars[length++] = raw_string[i]; // Add hy
			}
			else
				return -2; // There is some character which is not alphabetic
		}

		// Size control
		if (length < 2)
			return -4; // Character size smaller than 2 characters
		else if (length > NOUN_CHAR_SIZE + 3)
			return -5; // Character size exceeds NOUN_CHAR_SIZE size

		// Search if the noun in native form
		uint8 foundAmount = FindNoun(noun_chars, length, &found_nouns);
		// Search if the noun has -s
		uint8 foundAmountSParser = S_Parser(noun_chars, length, found_nouns);
		// Search if ends with ' or 's
		uint8 foundQuotationMark = Pos_Parser(noun_chars, length, found_nouns);

		// Save information to word
		if (foundAmount > 0) {
			for (uint8 i = 0; i < foundAmount; i++) {
				if (found_nouns[i]->s == IrrPlural)
					word.suffixes.push_back(S_Parsed);
				else
					word.suffixes.push_back(BaseForm);
			}
		}
		if (foundAmountSParser > 0) {
			for (uint8 i = 0; i < foundAmountSParser; i++) {
				word.suffixes.push_back(S_Parsed);
			}
		}
		else if (foundQuotationMark > 0) {
			switch (foundQuotationMark)
			{
			case 1:
				word.suffixes.push_back(CliticS);
				break;
			case 2:
				word.suffixes.push_back(PluralPoss);
				break;
			default:
				break;
			}
		}

		if (write_result) {
			if (foundAmount != 0) {
				// Turning noun data into str
				for (uint8 i = 0; i < foundAmount; i++)
					writeNoun(*found_nouns[i], i);
			}
			// Write s parse result
			if (foundAmountSParser != 0) {
				for (uint8 i = 0; i < foundAmountSParser; i++) {
					out_string += "\x1b[95m[Inflectional Noun (-s)]: \x1b[0m";
					uint16 id = foundAmount + i;
					switch (found_nouns[id]->s)
					{
					case None:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += " + s\n";
						break;
					case Suffix_es:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += " + es\n";
						break;
					case Suffix_ses:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += " + ses\n";
						break;
					case Suffix_zes:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += " + zes\n";
						break;
					case Suffix_0y_ies:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += "(-y) + ies\n";
						break;
					default:
						Log::Error("S parser returns a value that doesn't exist in switch");
						return -1;
					}
					writeNoun(*found_nouns[id], id);
				}
			}
			// Write clitic s
			else if (foundQuotationMark > 0) {
				uint16 id = foundAmount; // Accessing the last element in found nouns

				if (foundQuotationMark == 1) {
					out_string += "\x1b[95m[Inflectional Noun ('s)]: \x1b[0m\n";
					writeNoun(*found_nouns[id], id);
				}
				else {
					out_string += "\x1b[95m[Inflectional Plural Noun Possessive (s')]: \x1b[0m";
					switch (found_nouns[id]->s)
					{
					case None:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += " + s'\n";
						break;
					case Suffix_es:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += " + es'\n";
						break;
					case Suffix_ses:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += " + ses'\n";
						break;
					case Suffix_zes:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += " + zes'\n";
						break;
					case Suffix_0y_ies:
						out_string.Append(found_nouns[id]->chars, found_nouns[id]->length);
						out_string += "(-y) + ies'\n";
						break;
					default:
						Log::Error("S parser returns a value that doesn't exist in switch");
						return -1;
					}
					writeNoun(*found_nouns[id], id);
				}
			}
		}

		if (foundAmount + foundAmountSParser + foundQuotationMark > 0) {
			word.type = WordType::Noun;
			return 1;
		}
		else
			return -1;
	}

	void NounHandler::ExceptionToStr(const Exception ex_type, String& out_string) const {
		switch (ex_type)
		{
		case None:
			out_string += "-s";
			return;
		case Suffix_es:
			out_string += "-es";
			return;
		case Suffix_zes:
			out_string += "-zes";
			return;
		case Suffix_ses:
			out_string += "-ses";
			return;
		case Suffix_0y_ies:
			out_string += "(-y) + ies";
			return;
		case IrrSingular:
			out_string += "Irr Singular";
			return;
		case IrrPlural:
			out_string += "Irr Plural";
			return;
		default:
			out_string += "Incompatible!";
			return;
		}
		return;
	}
}