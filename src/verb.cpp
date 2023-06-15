// User defined libs
#include "verb.hpp"
#include "log.hpp"
#include "misc/util.hpp"
#include "text_data.hpp"

#define VERB_SIZE 32  // 32 default
#define VERB_SIZE_HYPHEN 4
#define VERB_ROW 26
#define VERB_COL 27
#define VERB_SIZE_INCREASE_COEFFICIENT 4 // 4 default
#define VERB_LINE_LENGHT 100

namespace verb {
	VerbHandler::VerbHandler() {
		buffer = new VerbIndexList[VERB_ROW * VERB_COL]; // buffer[row * 27 + column]
		irrVerbCollection.amount = 0;
		irrVerbCollection.capacity = 0;
		irrVerbCollection.verbs = nullptr;

		// Initialize Indexer
		for (int row = 0; row < VERB_ROW; row++) {
			for (int col = 0; col < VERB_COL; col++) {
				buffer[row * VERB_COL + col].verbs = nullptr;
				buffer[row * VERB_COL + col].verbAmount = 0;
				buffer[row * VERB_COL + col].verbCapacity = 0;
				buffer[row * VERB_COL + col].indicator[0] = 'a' + row;
				if (col == 26)
					buffer[row * VERB_COL + col].indicator[1] = '-';
				else
					buffer[row * VERB_COL + col].indicator[1] = 'a' + col;
			}
		}
	}

	VerbHandler::~VerbHandler() {
		// Delete each verb heap
		for (int row = 0; row < VERB_ROW; row++) {
			for (int col = 0; col < VERB_COL; col++) {
				if (buffer[row * VERB_COL + col].verbs != nullptr)
					delete[] buffer[row * VERB_COL + col].verbs;
			}
		}
		// Delete container
		delete[] buffer;
		// Delete irregular verb collections
		if (irrVerbCollection.verbs != nullptr)
			delete[] irrVerbCollection.verbs;
	}

	int8 VerbHandler::Free() {
		// 0 : deleted successfully
		// Delete each verb heap
		for (int row = 0; row < VERB_ROW; row++) {
			for (int col = 0; col < VERB_COL; col++) {
				if (buffer[row * VERB_COL + col].verbs != nullptr) {
					delete[] buffer[row * VERB_COL + col].verbs;

					buffer[row * VERB_COL + col].verbAmount = 0;
					buffer[row * VERB_COL + col].verbCapacity = 0;
					buffer[row * VERB_COL + col].verbs = nullptr;
				}
			}
		}

		// Delete irregular verb collections
		if (irrVerbCollection.verbs != nullptr) {
			delete[] irrVerbCollection.verbs;
			irrVerbCollection.verbs = nullptr;
			irrVerbCollection.amount = 0;
			irrVerbCollection.capacity = 0;
		}

		return 0;
	}



	void VerbHandler::MultipleVerbAdder(const char* file, const uint64& size) { // This function add multiple verbs to ram
		uint32 _successfull = 0;
		uint32 _nonAlphabetic = 0;
		uint32 _charExceedsSize = 0;
		uint32 _charSizeSmaller2 = 0;
		uint32 _notUnique = 0;
		uint32 _emptyLine = 0;

		String nonAlphabeticLines;
		String exceedsSizeLines;
		String sizeSmaller2CharLines;
		String nonUniqueLines;
		String emptyLines;

		auto calculateLineLength = [](const char& line) {
			uint32 i = 0;
			while ((&line)[i] != '\r' && (&line)[i] != '\n' && (&line)[i] != '\0') {
				i++;
			}
			return i;
		};

		uint32 lineNumber = 1;
		uint32 lineLength = 0;

		for (uint64 i = 0; i < size; i++) {

			lineLength = calculateLineLength(file[i]);
			if (lineLength == 0)
				continue;

			switch (AddNewVerb(&file[i], lineLength)) {
			case 1: // The verb Successfully added
				_successfull++;
				break;
			case -1: // There is some charachter which is not alphabetic
				_nonAlphabetic++;
				util::IntToStr(nonAlphabeticLines, lineNumber);
				nonAlphabeticLines += ',';
				break;
			case -2: // Character size exceeds VERB_CHAR_SIZE size
				_charExceedsSize++;
				util::IntToStr(exceedsSizeLines, lineNumber);
				exceedsSizeLines += ',';
				break;
			case -3: // Character size smaller than 2 characters
				_charSizeSmaller2++;
				util::IntToStr(sizeSmaller2CharLines, lineNumber);
				sizeSmaller2CharLines += ',';
				break;
			case -4: // The verb is already exist
				_notUnique++;
				util::IntToStr(nonUniqueLines, lineNumber);
				nonUniqueLines += ',';
				break;
			case -5:
				break;
			case -6:
				break;
			case 2: // Empty Lines
				_emptyLine++;
				util::IntToStr(emptyLines, lineNumber);
				emptyLines += ',';
				break;
			};

			i += lineLength;
			lineNumber++;
		}

		if (_successfull != 0) {
			Log::Info(_successfull) << " verbs added successfully.\n";
		}
		if (_nonAlphabetic != 0) {
			Log::Error(_nonAlphabetic) << " verbs contain non-alphabetic characters.\n" <<
				"These lines; " << nonAlphabeticLines.EndString().Chars() << "\n";
		}
		if (_charExceedsSize != 0) {
			Log::Error(_charExceedsSize) << " verbs exceed " << VERB_CHAR_SIZE << " characters.\n" <<
				"These lines; " << exceedsSizeLines.EndString().Chars() << "\n";
		}
		if (_charSizeSmaller2 != 0) {
			Log::Error(_charSizeSmaller2) << " verbs are smaller than 2 characters.\n" <<
				"These lines; " << sizeSmaller2CharLines.EndString().Chars() << "\n";
		}
		if (_notUnique != 0) {
			Log::Error(_notUnique) << " verbs are not unique.\n" <<
				"These lines; " << nonUniqueLines.EndString().Chars() << "\n";
		}
		//if (_emptyLine != 0) {
		//	Log::Warning(_emptyLine) << " empty lines.\n" <<
		//		"These lines; " << emptyLines.EndString().Chars() << "\n";
		//}
	}

	int8 VerbHandler::AddNewVerb(const char* line, const uint16& size) {
		// return 2   : Empty line
		// return 1  : The verb added successfully
		// return -1  : There is some charachter which is not alphabetic
		// return -2  : Character size exceeds VERB_CHAR_SIZE size
		// return -3  : Character size smaller than 2 characters
		// return -4  : The verb is already exist
		// return -5  : The verb doesn't contain V2 or V3 form
		// return -6  : Wrong syntax

		char temp[VERB_CHAR_SIZE];
		uint8 verbLength = 0;
		uint8 regularity_id = 1;
		bool parenthesOpened = false;
		bool checkUniqueness = true;
		SuffixGroup exception_data(Undefined, Undefined, Undefined);
		Verb* lastAddedVerb = nullptr;
		char incorrect_verb[VERB_CHAR_SIZE + 1];


		auto dispatchVerb = [&]() { // Returns either -4 because of uniquness or 1 if succeed
			if (verbLength == 0) { // If the temp array contains the verb at least 3 chars
				return 2;  // Character size smaller than 2 characters
			}
			else if (verbLength < 2) {
				return -3;
			}

			if (checkUniqueness) { // Default value is true
				if (FindVerb(temp, verbLength) == 0) // Checking Uniqueness
					lastAddedVerb = &CreateVerb(temp, verbLength, exception_data);
				else
					return -4; // The base verb is already exist
			}
			else { // If user disabled this feature
				lastAddedVerb = &CreateVerb(temp, verbLength, exception_data);
				checkUniqueness = true; // activating Uniqueness true for the next word if exist
			}

			// Resetting the verb length
			verbLength = 0;
			// Resetting the suffix informations
			exception_data.ing = Undefined;
			exception_data.s = Undefined;
			return 1; // The verb added successfully 
		};

		for (int i = 0; i < size; i++) {
			char currentCh = util::ToLowerCase(line[i]);

			if (currentCh != 0) { // When char is in a-Z
				if (verbLength < VERB_CHAR_SIZE) // Length control before adding the temp array
					temp[verbLength++] = currentCh;
				else
					return -2; //Character size exceeds VERB_CHAR_SIZE size
			}
			else if (line[i] == ' ' || line[i] == '\t') { // If char is blank space or new tab
				continue;
			}
			else if (line[i] == '#') { // If the comment line
				break;
			}
			else if (line[i] == '-') {  // some verbs can contain '-'
				if (verbLength == 0) { // the first character of verb cannot be hyphen
					return -1; // There is some charachter which is not alphabetic
				}
				else if (verbLength < VERB_CHAR_SIZE) { // Length control before adding the temp array
					temp[verbLength++] = line[i]; // Second char can be hyphen
				}
				else {
					return -2; // Character size exceeds VERB_CHAR_SIZE size
				}
			}
			else if (line[i] == ',') { // irregular verb input
				// Length control
				if (verbLength == 0) { // When there is no word in temp
					util::MemCpy(incorrect_verb, temp, verbLength);
					incorrect_verb[verbLength] = '\0';
					Log::Warning("[WRONG SYNTAX]: The verb contains wrong comma(,) '") << incorrect_verb <<
						"' please find and remove it!\n";
					return -6; // Wrong syntax
				}

				// If parentheses opened in the back
				if (parenthesOpened) {
					int8 result;
					switch (regularity_id) {
					case 2:
						checkUniqueness = false;
						exception_data.ed = IrregularVerb_V2;
						result = dispatchVerb();
						if (result != 1)  // If the verb has not been added
							return result; // The verb is already exist
						break;
					case 3:
						checkUniqueness = false;
						exception_data.ed = IrregularVerb_V3;
						result = dispatchVerb();
						if (result != 1)  // If the verb has not been added
							return result; // The verb is already exist
						break;
					case 1:
						util::MemCpy(incorrect_verb, temp, verbLength);
						incorrect_verb[verbLength] = '\0';
						Log::Error("[WRONG SYNTAX]: The verb cannot start with parentheses: '")
							<< incorrect_verb << "' please find and remove it!\n";
						return -6; // Wrong syntax
					default:
						util::MemCpy(incorrect_verb, temp, verbLength);
						incorrect_verb[verbLength] = '\0';
						Log::Error("[WRONG SYNTAX]: The irregular verb contains more than 3 time tenses: '")
							<< incorrect_verb << "' please find and remove it!\n";
						return -6; // Wrong syntax
					}

					// Adding last verb to irregular verbs 
					CreateIrregularVerb(*lastAddedVerb);
				}
				else { // Parentheses closed
					int8 result;

					switch (regularity_id)
					{
					case 1: // the verb is V1
						exception_data.ed = IrregularVerb;
						result = dispatchVerb();
						if (result != 1)  // If the verb has not been added
							return result; // The verb is already exist
						break;
					case 2: // the verb is V2
						checkUniqueness = false;
						exception_data.ed = IrregularVerb_V2;
						result = dispatchVerb();
						if (result != 1)  // If the verb has not been added
							return result; // The verb is already exist
						break;
					case 3: // the verb is V3
						util::MemCpy(incorrect_verb, temp, verbLength);
						incorrect_verb[verbLength] = '\0';
						Log::Warning("[WRONG SYNTAX]: At the end of this verb '") << incorrect_verb <<
							"' ends with comma(',') please find and remove it!\n";
						goto end_of_the_loop; // Break the loop
					default:
						util::MemCpy(incorrect_verb, temp, verbLength);
						incorrect_verb[verbLength] = '\0';
						Log::Error("[WRONG SYNTAX]: The irregular verb contains more than 3 time tenses: '")
							<< incorrect_verb << "' please find and remove it!\n";
						return -6; // Wrong syntax
					}

					// Increasing regularity id so that next verb's type can be changed
					regularity_id += 1;
					// Adding last verb to irregular verbs 
					CreateIrregularVerb(*lastAddedVerb);
				}
			}
			else if (line[i] == '(') {
				parenthesOpened = true;
			}
			else if (line[i] == ')') { // Add last verb 
				parenthesOpened = false;
			}
			else if (line[i] == '/') { // User defined commands in dictionary
				if (util::DoesContain("I0", &line[i + 1])) { // I0: ing except none
					exception_data.ing = None;
					i += 2;
				}
				else if (util::DoesContain("S0", &line[i + 1])) { // S0 : S except none
					exception_data.s = None;
					i += 2;
				}
				else if (util::DoesContain("S1", &line[i + 1])) { // U0 : don't check uniqueness
					exception_data.s = Suffix_es;
					i += 2;
				}
				else if (util::DoesContain("DL", &line[i + 1])) { // DL : Double last char
					exception_data.ing = Suffix_X_ing;
					exception_data.ed = Suffix_X_ed;
					i += 2;
				}
				else if (util::DoesContain("U0", &line[i + 1])) { // U0 : don't check uniqueness
					checkUniqueness = false;
					i += 2;
				}
				else {
					return -1; // There is some character which is not alphabetic
				}
			}
			else {
				return -1; // There is some character which is not alphabetic
			}

		}
	end_of_the_loop:

		if (regularity_id == 1) { // The verb was regular
			// returns -4; The base verb is already exist
			// returns 1; The verb added successfully 
			return dispatchVerb();
		}
		else if (regularity_id == 3) { // V3 form
			checkUniqueness = false; // Don't check uniqueness for V3
			exception_data.ed = IrregularVerb_V3;
			// returns -4; The base verb is already exist
			// returns 1; The verb added successfully 
			int8 result = dispatchVerb();
			if (result == 1) { // If the verb adds successfully
				CreateIrregularVerb(*lastAddedVerb); // Add the verb into irregulars
			}
			return result;
		}
		else {
			return -5; // The verb doesn't contain V2 or V3 form
		}
	}


	void VerbHandler::CreateIrregularVerb(Verb& verb) {
		if (&verb == nullptr) {
			// Do nothing
		}
		else if (verb.suffixes.ed == IrregularVerb) { // Create new collection
			if (irrVerbCollection.capacity > irrVerbCollection.amount) { // If there is enough space for new collection
				// Do nothing
			}
			else { // IF space is not enough
				if (irrVerbCollection.capacity > 0) { // If current capacity bigger than 0
					int newSize = irrVerbCollection.capacity * VERB_SIZE_INCREASE_COEFFICIENT;
					// Hold old pointer
					IrrVerb* p_old_data = irrVerbCollection.verbs;
					// Create new heap space
					irrVerbCollection.verbs = new IrrVerb[newSize];
					// Update new capacity
					irrVerbCollection.capacity = newSize;
					// Copy Old data to new one
					util::MemCpy(irrVerbCollection.verbs, p_old_data, irrVerbCollection.amount);
					// Free old space
					delete[] p_old_data;
				}
				else { // init a new collection
					irrVerbCollection.verbs = new IrrVerb[VERB_SIZE];
					irrVerbCollection.capacity = VERB_SIZE;
				}
			}

			// Assign verb address to base form and make others null
			irrVerbCollection.verbs[irrVerbCollection.amount].baseForm = &verb;
			irrVerbCollection.verbs[irrVerbCollection.amount].pastSimple = nullptr;
			irrVerbCollection.verbs[irrVerbCollection.amount].pastSimple_2 = nullptr;
			irrVerbCollection.verbs[irrVerbCollection.amount].pastParticiple = nullptr;
			irrVerbCollection.verbs[irrVerbCollection.amount].pastParticiple_2 = nullptr;
			// Increase irregular verb collection amount by 1
			irrVerbCollection.amount += 1;
		}
		else if (verb.suffixes.ed == IrregularVerb_V2) { // use the last collection PAST SIMPLE
			if (irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastSimple == nullptr)
				irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastSimple = &verb;
			else
				irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastSimple_2 = &verb;
		}
		else if (verb.suffixes.ed == IrregularVerb_V3) { // use the last collection PAST PARTICIPLE
			if (irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastParticiple == nullptr)
				irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastParticiple = &verb;
			else
				irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastParticiple_2 = &verb;
		}
	}

	Verb& VerbHandler::CreateVerb(const char* verb_chars, const int& str_lenght, const SuffixGroup& exception_p) {
		int row = verb_chars[0] - 97; // 97 means 'a' in ASCII code
		int col;

		if (verb_chars[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = verb_chars[1] - 97; // 97 means 'a' in ASCII code


		// Check buffer size
		if (buffer[row * VERB_COL + col].verbCapacity > buffer[row * VERB_COL + col].verbAmount) {
			// There is enough space for new verb
		}
		else { // There is no empyt slot to put new word
			if (buffer[row * VERB_COL + col].verbCapacity > 0) { // If current capacity bigger than 0
				// Calculating coefficient for new verb heap size
				int newSize = buffer[row * VERB_COL + col].verbCapacity * VERB_SIZE_INCREASE_COEFFICIENT;
				// Hold old pointer
				Verb* p_old_data = buffer[row * VERB_COL + col].verbs;
				// Create new verbs array
				buffer[row * VERB_COL + col].verbs = new Verb[newSize];
				// Update new capacity
				buffer[row * VERB_COL + col].verbCapacity = newSize;
				// Copy Old data to new array
				for (uint32 item = 0; item < buffer[row * VERB_COL + col].verbAmount; item++) {
					// Copy data
					buffer[row * VERB_COL + col].verbs[item] = p_old_data[item];
					// If the verb is irregular update the adress
					if (buffer[row * VERB_COL + col].verbs[item].suffixes.ed == IrregularVerb ||
						buffer[row * VERB_COL + col].verbs[item].suffixes.ed == IrregularVerb_V2 ||
						buffer[row * VERB_COL + col].verbs[item].suffixes.ed == IrregularVerb_V3) {
						UpdateIrrVerbAdress(buffer[row * VERB_COL + col].verbs[item], p_old_data[item]);
					}

				}

				//CopyData(*buffer[row * VERB_COL + col].verbs, *p_old_data, buffer[row * VERB_COL + col].verbAmount);
				// Free old space
				delete[] p_old_data;
			}
			else { // Initialize new heap array
				if (col == 26) { // index for '-' i.e "x-"
					buffer[row * VERB_COL + col].verbs = new Verb[VERB_SIZE_HYPHEN];
					buffer[row * VERB_COL + col].verbCapacity = VERB_SIZE_HYPHEN;
				}
				else {
					buffer[row * VERB_COL + col].verbs = new Verb[VERB_SIZE];
					buffer[row * VERB_COL + col].verbCapacity = VERB_SIZE;
				}

			}
		}

		// Copy chars to buffer verb and increase the size of verb
		util::MemCpy(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].chars, verb_chars, str_lenght);
		buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].length = str_lenght;

		// Check Exceptions... -S -ING -ED
		if (exception_p.ed == IrregularVerb)
		{
			// Change regularity of the verb
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.ed = IrregularVerb;

			// Check S exception
			if (exception_p.s == Undefined) {
				CheckException_S(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.s = exception_p.s;
			}
			// Check ING exception
			if (exception_p.ing == Undefined) {
				CheckException_ING(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.ing = exception_p.ing;
			}
		}
		else if (exception_p.ed == IrregularVerb_V2) {
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.ed = IrregularVerb_V2;
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.ing = IrregularVerb_V2;
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.s = IrregularVerb_V2;
		}
		else if (exception_p.ed == IrregularVerb_V3) {
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.ed = IrregularVerb_V3;
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.ing = IrregularVerb_V3;
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.s = IrregularVerb_V3;
		}
		else { // When the verb is regular
			// Check ED exception
			if (exception_p.ed == Undefined) {
				CheckException_ED(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.ed = exception_p.ed;
			}
			// Check S exception
			if (exception_p.s == Undefined) {
				CheckException_S(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.s = exception_p.s;
			}
			// Check ING exception
			if (exception_p.ing == Undefined) {
				CheckException_ING(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].suffixes.ing = exception_p.ing;
			}

		}

		// Increase last empty index by one
		buffer[row * VERB_COL + col].verbAmount += 1;

		// Return the newly added verb's adress
		return buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount - 1];
	}

	int8 VerbHandler::ParseVerb(const String& raw_string, TypeAndSuffixes& word,
		String& out_string, const bool write_result) const {
		// return +1  : The verb Successfully found.
		// return -1  : No verb found.
		// return -2  : There is some characters which is not alphabetic
		// return -3  : First character cannot be hyphen
		// return -4  : Character size less than 2 characters
		// return -5  : Character size exceeds VERB_CHAR_SIZE size

		char verb_chars[150];
		uint8 lenght = 0;
		std::vector<Verb*> found_verbs;

		auto writeVerb = [&](Verb& verb, uint16 i) {
			util::IntToStr(out_string, i + 1);
			out_string += ". ";
			VerbToStr(verb, out_string);
			out_string += '\n';
			out_string.EndString();
		};

		// Make all chars lowercase and get rid of spaces
		for (int i = 0; i < raw_string.Length(); i++) {
			char currentCh = util::ToLowerCase(raw_string[i]);

			if (currentCh != 0) // If the char is alphabetic
				verb_chars[lenght++] = currentCh;
			else if (raw_string[i] == ' ' || raw_string[i] == '\t')
				continue;
			else if (raw_string[i] == '-') { // If the charachter is hyphen
				if (i == 0)
					return -3; // First character cannot be hyphen
				else
					verb_chars[lenght++] = raw_string[i]; // Add hy
			}
			else
				return -2; // There is some character which is not alphabetic
		}

		// Size control
		if (lenght < 2)
			return -4; // Character size smaller than 2 characters
		else if (lenght > VERB_CHAR_SIZE + 3)
			return -5; // Character size exceeds VERB_CHAR_SIZE size

		// Parse if the verb in native form
		uint8 foundAmount = FindVerb(verb_chars, lenght, &found_verbs);
		uint8 foundAmountEd = 0;
		uint8 foundAmountIng = 0;
		uint8 foundAmountS = 0;

		if (foundAmount > 0) {
			for (uint8 i = 0; i < foundAmount; i++) {
				if (found_verbs[i]->suffixes.ed == IrregularVerb_V2)
					word.suffixes.push_back(IrregularVerb_V2);
				else if (found_verbs[i]->suffixes.ed == IrregularVerb_V3)
					word.suffixes.push_back(IrregularVerb_V3);
				else
					word.suffixes.push_back(BaseForm);
			}
		}

		// Parse if the verb has -ed
		if (foundAmountEd = ED_Parser(verb_chars, lenght, found_verbs) > 0) {
			for (uint8 i = 0; i < foundAmountEd; i++) { // save information to word
				uint8 id = foundAmount + i;
				word.suffixes.push_back(ED_Parsed);
			}
		}
		// Parse if the verb has -ing
		else if (foundAmountIng = ING_Parser(verb_chars, lenght, found_verbs) > 0) {
			for (uint8 i = 0; i < foundAmountIng; i++) { // save information to word
				uint8 id = foundAmount + i;
				word.suffixes.push_back(ING_Parsed);
			}
		}
		// Parse if the verb has -s
		else if (foundAmountS = S_Parser(verb_chars, lenght, found_verbs) > 0) {
			for (uint8 i = 0; i < foundAmountS; i++) { // save information to word
				uint8 id = foundAmount + i;
				word.suffixes.push_back(S_Parsed);
			}
		}


		if (write_result) { // Turn verbs into str
			// Base form results
			for (uint16 i = 0; i < foundAmount; i++) {
				writeVerb(*found_verbs[i], i);
			}
			// Inflectual verb ed results
			for (uint16 i = 0; i < foundAmountEd; i++) {
				out_string += "\x1b[95m[Infectional Verb (-ed)]: \x1b[0m";
				uint8 id = foundAmount + i;
				switch (found_verbs[id]->suffixes.ed)
				{
				case None:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += " + ed\n";
					break;
				case Suffix_0y_ied:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += "(-y) + ied\n";
					break;
				case Suffix_d:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += " + d\n";
					break;
				case Suffix_X_ed:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += " + (";
					out_string += verb_chars[lenght - 3];
					out_string += ")ed\n";
					break;
				default:
					Log::Error("ED parser returns a value that doesn't exist in switch.");
					return -1;
				}
				writeVerb(*found_verbs[id], id);
			}
			// Inflectual verb ing results
			for (uint16 i = 0; i < foundAmountIng; i++) {
				out_string += "\x1b[95m[Infectional Verb (-ing)]: \x1b[0m";
				uint8 id = foundAmount + i;

				switch (found_verbs[id]->suffixes.ing)
				{
				case None:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += " + ing\n";
					break;
				case Suffix_0e_ing:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += "(-e) + ing\n";
					break;
				case Suffix_0ie_ying:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += "(-ie) + ying\n";
					break;
				case Suffix_X_ing:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += " + (";
					out_string += verb_chars[lenght - 4];
					out_string += ")ing\n";
					break;
				default:
					Log::Error("ING parser returns a value that doesn't exist in switch.");
					return -1;
				}
				// Turning verb data into str
				writeVerb(*found_verbs[id], id);
			}
			// Inflectual verb s results
			for (uint16 i = 0; i < foundAmountS; i++) {
				out_string += "\x1b[95m[Infectional Verb (-s)]: \x1b[0m";
				uint8 id = foundAmount + i;

				switch (found_verbs[id]->suffixes.s)
				{
				case None:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += " + s\n";
					break;
				case Suffix_es:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += " + es\n";
					break;
				case Suffix_ses:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += " + ses\n";
					break;
				case Suffix_zes:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += " + zes\n";
					break;
				case Suffix_0y_ies:
					out_string.Append(found_verbs[id]->chars, found_verbs[id]->length);
					out_string += "(-y) + ies\n";
					break;
				default:
					Log::Error("S parser returns a value that doesn't exist in switch");
					return -1;
				}
				writeVerb(*found_verbs[id], id);
			}
		}

		if (foundAmount + foundAmountEd + foundAmountIng + foundAmountS > 0) {
			word.type = WordType::Verb;
			word.adrs = (char*)found_verbs[0];
			return 1;
		}
		else
			return -1;
	}

#pragma region Suffix Parser
	uint8 VerbHandler::ED_Parser(const char* verb_chars, const uint8& lenght, std::vector<Verb*>& out_verbs) const {
		// return  -1: verb couldn't find
		// return  X: verb found and with exception

		// None, EndsWith_w_x_y, EndsWith_Cy, EndsWith_e, EndsWith_XVC, DoubleLastChar
		// -ed,		-ed,		-ied		, -d		, -X + ED		,-X + ED 	  

		SuffixGroup exceptions(Undefined, Undefined, Undefined);

		uint8 result = 0;

		if (verb_chars[lenght - 1] == 'd') { // when the char is -d
			// If the input ends with d
			exceptions.ed = Suffix_d; // EndsWith_e
			result += FindWithException(verb_chars, lenght - 1, exceptions, out_verbs); // If verbs ends with -e
			if (result > 0) // If the verb found return
				return result;

			// If the input ends with -ed
			if (verb_chars[lenght - 2] == 'e') { // when the last 2 chars is -ed
				exceptions.ed = None;
				result += FindWithException(verb_chars, lenght - 2, exceptions, out_verbs); // If verbs ends with None
				if (result > 0) // If the verb found return
					return result;

				if (lenght > 2 && verb_chars[lenght - 3] == 'i') {
					char tempVerb[VERB_CHAR_SIZE]; // Creating a temporary space for verb 
					util::MemCpy(tempVerb, verb_chars, lenght - 3); // copying charachters excluding last 3
					tempVerb[lenght - 3] = 'y'; // adding y to the last char of the verb 

					exceptions.ed = Suffix_0y_ied; //EndsWith_Cy
					result += FindWithException(tempVerb, lenght - 2, exceptions, out_verbs); // If verbs ends with  Cy
					if (result > 0) // If the verb found return
						return result;
				}
				else if (lenght > 5 && !util::IsVowel(verb_chars[lenght - 3])) {
					// verb lenght has to be bigger than 5 the 3rd to last has to be consonant

					if (verb_chars[lenght - 3] == verb_chars[lenght - 4]) { // If the 3rd to last and 4th chars are same
						exceptions.ed = Suffix_X_ed;
						result += FindWithException(verb_chars, lenght - 3, exceptions, out_verbs); // If verbs ends with consonants + Vovel + Consonant
						if (result > 0) // If the verb found return
							return result;
					}
				}
			}
		}

		return result;
	}

	uint8 VerbHandler::ING_Parser(const char* verb_chars, const uint8& lenght, std::vector<Verb*>& out_verbs) const {
		// None, EndsWith_w_x_y, EndsWith_Ce, EndsWith_ie, EndsWith_XVC, DoubleLastChar 
		// -ing,	-ing,		 -(-e)ing,		-ying,			-xxing,		-xxing

		SuffixGroup exceptions(Undefined, Undefined, Undefined);
		int8 result = 0;

		if (lenght > 4) { // the lengh has to be min 5 chars
			// If the input str ends with -ing
			if (verb_chars[lenght - 3] == 'i' && verb_chars[lenght - 2] == 'n' && verb_chars[lenght - 1] == 'g') {
				char tempVerb[VERB_CHAR_SIZE]; // Creating a temporary space for verb 

				exceptions.ing = None;
				result += FindWithException(verb_chars, lenght - 3, exceptions, out_verbs); // If verbs ends with none
				if (result > 0) // If the verb found return
					return result;
				if (!util::IsVowel(verb_chars[lenght - 4])) { // If the 4th to last char is Consonant

					{ // If the original verb ends with -e
						util::MemCpy(tempVerb, verb_chars, lenght - 3); // copying charachters excluding last 3
						tempVerb[lenght - 3] = 'e'; // adding e to the last char of the verb 

						exceptions.ing = Suffix_0e_ing; // EndsWith_Ce
						result += FindWithException(tempVerb, lenght - 2, exceptions, out_verbs); // If verbs ends with Cy
						if (result > 0) // If the verb found return
							return result;
					}

					if (lenght > 6 && verb_chars[lenght - 4] == verb_chars[lenght - 5]) { // If the input str ends with double last char
						exceptions.ing = Suffix_X_ing;
						result += FindWithException(verb_chars, lenght - 4, exceptions, out_verbs); // If verbs ends with consonants + Vovel + Consonant
						if (result > 0) // If the verb found return
							return result;
					}
				}

				if (verb_chars[lenght - 4] == 'y') { // If the input text ends with -ying
					util::MemCpy(tempVerb, verb_chars, lenght - 4); // copying charachters excluding last 4
					util::MemCpy(&tempVerb[lenght - 4], "ie", 2);// copying ie to the end

					exceptions.ing = Suffix_0ie_ying; // EndsWith_ie
					result += FindWithException(tempVerb, lenght - 2, exceptions, out_verbs); // If verbs ends with  Cy
					if (result > 0) // If the verb found return
						return result;
				}
			}
		}

		return result;
	}

	uint8 VerbHandler::S_Parser(const char* verb_chars, const uint8& lenght, std::vector<Verb*>& out_verbs) const {
		// None, EndsWith_Cy, EndsWith_ss, EndsWith_zz, EndsWith_ch, EndsWith_sh, EndsWith_s, EndsWith_z, EndsWith_x, EndsWith_o
		// -s		-(-y)ies	-es			   -es			-es			-es			 -ses		 -zes	,	 -es    ,	-es

		SuffixGroup exceptions(Undefined, Undefined, Undefined);
		int8 result = 0;

		if (lenght > 2) { // the input text has to be at least 3 chars
			if (verb_chars[lenght - 1] == 's') { // input str ends with -s
				{
					exceptions.s = None;
					result += FindWithException(verb_chars, lenght - 1, exceptions, out_verbs);
					if (result > 0) // If the verb found return
						return result;
				}
				if (verb_chars[lenght - 2] == 'e') { // input str ends with -es
					{
						exceptions.s = Suffix_es;
						result += FindWithException(verb_chars, lenght - 2, exceptions, out_verbs);
						if (result > 0) // If the verb found return
							return result;
					}
					if (verb_chars[lenght - 3] == 'i') { // input str ends with -ies
						char tempVerb[VERB_CHAR_SIZE]; // Creating a temporary space for verb
						util::MemCpy(tempVerb, verb_chars, lenght - 3); // copying charachters excluding last 3
						tempVerb[lenght - 3] = 'y'; // adding e to the last char of the verb 
						exceptions.s = Suffix_0y_ies; // setting the suffix type

						result += FindWithException(tempVerb, lenght - 2, exceptions, out_verbs);
						if (result > 0) // If the verb found return
							return result;
					}
					if (lenght > 4) {
						if (verb_chars[lenght - 3] == 's' && verb_chars[lenght - 4] == 's') { // input str ends with -sses
							exceptions.s = Suffix_ses;
							result += FindWithException(verb_chars, lenght - 3, exceptions, out_verbs);
							if (result > 0) // If the verb found return
								return result;
						}
						else if (verb_chars[lenght - 3] == 'z' && verb_chars[lenght - 4] == 'z') { // input str ends with -zzes
							exceptions.s = Suffix_zes;
							result += FindWithException(verb_chars, lenght - 3, exceptions, out_verbs);
							if (result > 0) // If the verb found return
								return result;
						}
					}
				}
			}
		}

		return result;
	}

#pragma endregion
	uint8 VerbHandler::FindWithException(const char* verb_chars, const int& length, SuffixGroup& exception_p, std::vector<Verb*>& out_verbs) const {
		// return 0  : no verb found
		// return 1  : 1 verb found
		// return 2  : 2 verb found
		// return 3  : 3 verb found.
		// ...

		int8 lastId = 0;

		if (length < 2) {
			return 0;
		}

		int row = verb_chars[0] - 97; // 97 means 'a' in ASCII code
		int col;
		if (verb_chars[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = verb_chars[1] - 97; // 97 means 'a' in ASCII code


		bool (*ed_type)(const SuffixGroup&, Verb&) = [](const SuffixGroup& ee, Verb& verb) {
			return ee.ed == verb.suffixes.ed;
		};

		bool (*s_type)(const SuffixGroup&, Verb&) = [](const SuffixGroup& ee, Verb& verb) {
			return ee.s == verb.suffixes.s;
		};

		bool (*ing_type)(const SuffixGroup&, Verb&) = [](const SuffixGroup& ee, Verb& verb) {
			return ee.ing == verb.suffixes.ing;
		};

		bool (*IsSameType)(const SuffixGroup&, Verb&);

		if (exception_p.ed != Undefined) {
			IsSameType = ed_type;
		}
		else if (exception_p.s != Undefined) {
			IsSameType = s_type;
		}
		else if (exception_p.ing != Undefined) {
			IsSameType = ing_type;
		}
		else {
			return -1;
		}

		for (int i = 0; i < buffer[row * VERB_COL + col].verbAmount; i++) { // Get all verbs from buffer
			//If they have same lenght
			if (buffer[row * VERB_COL + col].verbs[i].length == length) {
				// If they have same type
				if (IsSameType(exception_p, buffer[row * VERB_COL + col].verbs[i])) {
					// If they have same content
					if (util::IsSameArray(buffer[row * VERB_COL + col].verbs[i].chars, verb_chars, length)) {
						out_verbs.push_back(&buffer[row * VERB_COL + col].verbs[i]);
						lastId++;
					}
				}
			}
		}

		return lastId;
	}

	uint8 VerbHandler::FindVerb(const char* verb_chars, const int& length, std::vector<Verb*>* out_verbs) const {
		// return 0  : no verb found
		// return 1  : 1 verb found
		// return 2  : 2 verb found
		// return 3  : 3 verb found
		// ...

		int8 foundAmount = 0;
		int row = verb_chars[0] - 97; // 97 means 'a' in ASCII code
		int col;
		if (verb_chars[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = verb_chars[1] - 'a'; // 97 means 'a' in ASCII code

		for (int i = 0; i < buffer[row * VERB_COL + col].verbAmount; i++) { // Get all verbs from buffer
			if (buffer[row * VERB_COL + col].verbs[i].length == length) {
				if (util::IsSameArray(buffer[row * VERB_COL + col].verbs[i].chars, verb_chars, length)) {
					if (out_verbs != nullptr) {
						out_verbs->push_back(&buffer[row * VERB_COL + col].verbs[i]);
						foundAmount++;
					}
					else {
						return 1;
					}
				}
			}
		}

		return foundAmount;
	}

#undef VERB_RESULT_ARRAY_SIZE

	void VerbHandler::VerbToStr(const Verb& verb, String& out_string) const {
		if (&verb == nullptr) // If the object is empty return 0
			return;

		out_string.Append(verb.chars, verb.length);

		out_string += "\t[Sfx_ED]: ";
		ExceptionToStr(0, verb.suffixes.ed, out_string);

		out_string += ", [Sfx_S]: ";
		ExceptionToStr(1, verb.suffixes.s, out_string);

		out_string += ", [Sfx_ING]: ";
		ExceptionToStr(2, verb.suffixes.ing, out_string);
	}

	void VerbHandler::GetVerbsWithIndex(const char* index_couple, String& out_string) {
		int row = index_couple[0] - 97; // 97 means 'a' in ASCII code
		int col;

		if (index_couple[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = index_couple[1] - 97; // 97 means 'a' in ASCII code


		for (int i = 0; i < buffer[row * VERB_COL + col].verbAmount; i++) {
			VerbToStr(buffer[row * VERB_COL + col].verbs[i], out_string);
			out_string += '\n';
		}

		out_string.EndString();
	}

	Verb* VerbHandler::FindBaseVerb(Verb* verb) {

		for (uint16 i = 0; i < irrVerbCollection.amount; i++) {
			if (irrVerbCollection.verbs[i].pastSimple == verb) {
				return irrVerbCollection.verbs[i].baseForm;
			}
			else if (irrVerbCollection.verbs[i].pastSimple_2 == verb) {
				return irrVerbCollection.verbs[i].baseForm;
			}
		}

		return nullptr;
	}

	int8 VerbHandler::UpdateIrrVerbAdress(Verb& new_address, const Verb& old_adress) {

		for (uint16 i = 0; i < irrVerbCollection.amount; i++) {
			if (new_address.suffixes.ed == IrregularVerb) {
				if (irrVerbCollection.verbs[i].baseForm == &old_adress) {
					irrVerbCollection.verbs[i].baseForm = &new_address;
					return 1;
				}
			}
			else if (new_address.suffixes.ed == IrregularVerb_V2) {
				if (irrVerbCollection.verbs[i].pastSimple == &old_adress) {
					irrVerbCollection.verbs[i].pastSimple = &new_address;
					return 1;
				}
				else if (irrVerbCollection.verbs[i].pastSimple_2 == &old_adress) {
					irrVerbCollection.verbs[i].pastSimple_2 = &new_address;
					return 1;
				}
			}
			else if (new_address.suffixes.ed == IrregularVerb_V3) {
				if (irrVerbCollection.verbs[i].pastParticiple == &old_adress) {
					irrVerbCollection.verbs[i].pastParticiple = &new_address;
					return 1;
				}
				else if (irrVerbCollection.verbs[i].pastParticiple_2 == &old_adress) {
					irrVerbCollection.verbs[i].pastParticiple_2 = &new_address;
					return 1;
				}
			}

		}

		return -1; // Verb couln't found in irregular verbs
	}

	uint16 VerbHandler::GetAllIrregularVerbs(String& out_string) const {
		for (int i = 0; i < irrVerbCollection.amount; i++) {
			// Base form copy
			out_string += "[Base]:";
			out_string.Append(irrVerbCollection.verbs[i].baseForm->chars, irrVerbCollection.verbs[i].baseForm->length);

			// Past simple forms copy
			if (irrVerbCollection.verbs[i].pastSimple != nullptr) {
				out_string += "\t[PastSimple]:";
				out_string.Append(irrVerbCollection.verbs[i].pastSimple->chars, irrVerbCollection.verbs[i].pastSimple->length);

				if (irrVerbCollection.verbs[i].pastSimple_2 != nullptr) {
					out_string += ", ";
					out_string.Append(irrVerbCollection.verbs[i].pastSimple_2->chars,
						irrVerbCollection.verbs[i].pastSimple_2->length);
				}
			}

			// Past Participle forms copy
			if (irrVerbCollection.verbs[i].pastParticiple != nullptr) {
				out_string += "\t[PastParticiple]:";
				out_string.Append(irrVerbCollection.verbs[i].pastParticiple->chars,
					irrVerbCollection.verbs[i].pastParticiple->length);

				if (irrVerbCollection.verbs[i].pastParticiple_2 != nullptr) {
					out_string += ", ";
					out_string.Append(irrVerbCollection.verbs[i].pastParticiple_2->chars,
						irrVerbCollection.verbs[i].pastParticiple_2->length);
				}
			}

			out_string += '\n';
		}
		if (irrVerbCollection.amount > 0) {
			out_string--; // Remove last char
			out_string.EndString(); // Place \0 char
		}

		return irrVerbCollection.amount;
	}


#pragma region Exception Controllers

	void VerbHandler::CheckException_S(Verb& verb) const {
		// None, EndsWith_Cy, EndsWith_ss, EndsWith_zz, EndsWith_ch, EndsWith_sh, EndsWith_s, EndsWith_z, EndsWith_x, EndsWith_o

		// Check if second to last character is consonant and last char is y
		if (!util::IsVowel(verb.chars[verb.length - 2]) && verb.chars[verb.length - 1] == 'y') {
			verb.suffixes.s = Suffix_0y_ies; // EndsWith_Cy
			return;
		}
		else if (verb.chars[verb.length - 1] == 's') { // When last character is s
			if (verb.chars[verb.length - 2] == 's') // when verb ends with ss
				verb.suffixes.s = Suffix_es; // EndsWith_ss
			else
				verb.suffixes.s = Suffix_ses; // EndsWith_s
			return;
		}
		else if (verb.chars[verb.length - 1] == 'z') { // When last character is z
			if (verb.chars[verb.length - 2] == 'z') // when verb ends with zz
				verb.suffixes.s = Suffix_es; // EndsWith_zz
			else
				verb.suffixes.s = Suffix_zes; // EndsWith_z
			return;
		}
		else if (verb.chars[verb.length - 1] == 'h') { // When last character is h
			if (verb.chars[verb.length - 2] == 's') {// when verb ends with sh
				verb.suffixes.s = Suffix_es; // EndsWith_sh
				return;
			}
			else if (verb.chars[verb.length - 2] == 'c') { // when verb ends with ch
				verb.suffixes.s = Suffix_es; // EndsWith_ch
				return;
			}
		}
		else if (verb.chars[verb.length - 1] == 'x') { // When last character x
			verb.suffixes.s = Suffix_es; // EndsWith_x
			return;
		}
		else if (verb.chars[verb.length - 1] == 'o') { // When last character o
			verb.suffixes.s = Suffix_es; // EndsWith_o
			return;
		}

		verb.suffixes.s = None;

	}

	void VerbHandler::CheckException_ING(Verb& verb) const {
		// None, EndsWith_w_x_y, EndsWith_Ce, EndsWith_ie,  EndsWith_XVC, DoubleLastChar 
		// -ing,	-ing,		 -(-e)ing,		-(-ie)ying,		-xing,		-xing

		if (verb.chars[verb.length - 1] == 'w' || verb.chars[verb.length - 1] == 'x' || verb.chars[verb.length - 1] == 'y') {
			verb.suffixes.ing = None; // EndsWith_w_x_y
			return;
		}
		else if (verb.chars[verb.length - 1] == 'e') {
			if (verb.length > 2 && !util::IsVowel(verb.chars[verb.length - 2])) { // Verb ends with consonant + vowel
				verb.suffixes.ing = Suffix_0e_ing; // EndsWith_Ce
				return;
			}
			else if (verb.chars[verb.length - 2] == 'i') { // Verb ends with ie
				verb.suffixes.ing = Suffix_0ie_ying; // EndsWith_ie
				return;
			}
		}
		else if (!util::IsVowel(verb.chars[verb.length - 1])) {
			if (util::IsVowel(verb.chars[verb.length - 2])) {// Ends with Vowel + Constant
				if (verb.length > 2) { // Length control
					bool isAllConsonant = true;
					for (int i = 0; i < verb.length - 2; i++) {
						if (util::IsVowel(verb.chars[verb.length - 3 - i])) {
							isAllConsonant = false;
							break;
						}
					}
					if (isAllConsonant && verb.length > 2) { // Verb contains one vowel and ends with (Vowel + Constant)
						verb.suffixes.ing = Suffix_X_ing; // EndsWith_XVC
						return;
					}
				}
			}
		}

		verb.suffixes.ing = None;
	}

	void VerbHandler::CheckException_ED(Verb& verb) const {
		// None, EndsWith_w_x_y, EndsWith_Cy, EndsWith_e, EndsWith_XVC, DoubleLastChar
		// -ed,		-ed,		-ied		, -d		, -X + ED		,-X + ED 	  

		if (verb.chars[verb.length - 1] == 'w' || verb.chars[verb.length - 1] == 'x') {
			verb.suffixes.ed = None; // EndsWith_w_x_y
			return;
		}
		else if (verb.chars[verb.length - 1] == 'y') {
			if (!util::IsVowel(verb.chars[verb.length - 2])) // Consonant + y
				verb.suffixes.ed = Suffix_0y_ied; //  EndsWith_Cy
			else
				verb.suffixes.ed = None; // EndsWith_w_x_y
			return;
		}
		else if (verb.chars[verb.length - 1] == 'e') { // Verb ends with e
			verb.suffixes.ed = Suffix_d; // EndsWith_e
			return;
		}
		else if (!util::IsVowel(verb.chars[verb.length - 1])) {
			if (util::IsVowel(verb.chars[verb.length - 2])) {// Ends with Vowel + Constant
				if (verb.length < 3) // Length control
					return;

				bool isAllConsonant = true;
				for (int i = 0; i < verb.length - 2; i++) { // check to first if contains other vowel char 
					if (util::IsVowel(verb.chars[verb.length - 3 - i])) {
						isAllConsonant = false;
						break;
					}
				}
				if (isAllConsonant) {
					verb.suffixes.ed = Suffix_X_ed; // EndsWith_XVC
					return;
				}
			}
		}

		verb.suffixes.ed = None;
	}

#pragma endregion

	void VerbHandler::ExceptionToStr(const int8 type, const Suffix_t& ex_type,
		String& out_string) const { // 0: ED, 1: S, 2:ING

		switch (type)
		{
		case 0: // Ed type
			switch (ex_type)
			{
			case None:
				out_string += "-ed";
				return;
			case IrregularVerb:
				out_string += "Irr verb V1";
				return;
			case IrregularVerb_V2:
				out_string += "Irr verb V2";
				return;
			case IrregularVerb_V3:
				out_string += "Irr verb V3";
				return;
			case Suffix_d:
				out_string += "-d";
				return;
			case Suffix_0y_ied:
				out_string += "(-y) + ied";
				return;
			case Suffix_X_ed:
				out_string += "(X) + ed";
				return;
			default:
				out_string += "Incompatible!";
				return;
			}
			return;

		case 1: // S type
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
			default:
				out_string += "Incompatible!";
				return;
			}
			return;

		case 2: // Ing type
			switch (ex_type)
			{
			case None:
				out_string += "-ing";
				return;
			case Suffix_0e_ing:
				out_string += "(-e) + ing";
				return;
			case Suffix_0ie_ying:
				out_string += "(-ie) + ying";
				return;
			case Suffix_X_ing:
				out_string += "(X) + ing";
				return;
			default:
				out_string += "Incompatible!";
				return;
			}
			return;

		default:
			out_string += "Incorrect Type!";
			return;
		}

		/* Old types
		switch (ex_type)
		{
		case verb::None:
			return CopyStr(out_str, *VERB_EX_NONE);
		case verb::IrregularVerb:
			return CopyStr(out_str, *VERB_EX_IRREGULAR_V1);
		case verb::IrregularVerb_V2:
			return CopyStr(out_str, *VERB_EX_IRREGULAR_V2);
		case verb::IrregularVerb_V3:
			return CopyStr(out_str, *VERB_EX_IRREGULAR_V3);
		case verb::DoubleLastChar:
			return CopyStr(out_str, *VERB_EX_DOUBLE_LAST);
		case verb::EndsWith_e:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_E);
		case verb::EndsWith_Cy:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_CY);
		case verb::EndsWith_z:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_Z);
		case verb::EndsWith_s:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_S);
		case verb::EndsWith_x:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_X);
		case verb::EndsWith_o:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_O);
		case verb::EndsWith_ss:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_SS);
		case verb::EndsWith_zz:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_ZZ);
		case verb::EndsWith_ch:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_CH);
		case verb::EndsWith_sh:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_SH);
		case verb::EndsWith_w_x_y:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_WXY);
		case verb::EndsWith_Ce:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_CE);
		case verb::EndsWith_ie:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_IE);
		case verb::EndsWith_XVC:
			return CopyStr(out_str, *VERB_EX_ENDSWITH_XVC);
		default:
			return CopyStr(out_str, *"Undefined");
		}*/
	}

}