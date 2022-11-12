// User defined libs
#include "verb.hpp"
#include "utility.hpp"
#include "log.hpp"
#include "textdata.hpp"

#define VERB_SIZE 32  // 32 default
#define VERB_SIZE_HYPHEN 4
#define VERB_ROW 26
#define VERB_COL 27
#define SIZE_INCREASE_COEFFICIENT 4 // 4 default
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

	int8 VerbHandler::DeleteAll() {
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

		// Delete Irregular records
		for (int i = 0; i < irrVerbCollection.amount; i++) {
			delete[] irrVerbCollection.verbs;
			irrVerbCollection.verbs = nullptr;
			irrVerbCollection.amount = 0;
			irrVerbCollection.capacity = 0;
		}
		return 0;
	}


	void VerbHandler::MultipleVerbAdder(const char& file) { // This function add multiple verbs to ram
		uint16 _successfull = 0;
		uint16 _nonAlphabetic = 0;
		uint16 _charExceedsSize = 0;
		uint16 _charSizeSmaller2 = 0;
		uint16 _notUnique = 0;

		uint16 _emptyLine = 0;



#define CAPACITY_OF_LINES 300
		MyString nonAlphabeticLines(new char[CAPACITY_OF_LINES], CAPACITY_OF_LINES);
		MyString exceedsSizeLines(new char[CAPACITY_OF_LINES], CAPACITY_OF_LINES);
		MyString sizeSmaller2CharLines(new char[CAPACITY_OF_LINES], CAPACITY_OF_LINES);
		MyString nonUniqueLines(new char[CAPACITY_OF_LINES], CAPACITY_OF_LINES);
		MyString emptyLines(new char[CAPACITY_OF_LINES], CAPACITY_OF_LINES);
#undef CAPACITY_OF_LINES

		uint16 lineLenght = 0;

		auto findNextLine = [&](const char& line) {
			uint16 i = 0;
			while ((&line)[i] != '\n' && (&line)[i] != '\0') {
				i++;
			}
			lineLenght = i;
		};

		uint32 i = -1;
		uint32 lineNumber = 1;
		char temp[15];
		do { // Read line by line
			i++; // Increment
			switch (AddNewVerb((&file)[i])) {
			case 1: // The verb Successfully added
				_successfull++;
				break;
			case -1: // There is some charachter which is not alphabetic
				_nonAlphabetic++;
				nonAlphabeticLines.AddToEnd(*temp, IntToStr(*temp, lineNumber));
				nonAlphabeticLines.AddToEnd(*",", 1);
				break;
			case -2: // Character size exceeds VERB_CHAR_SIZE size
				_charExceedsSize++;
				exceedsSizeLines.AddToEnd(*temp, IntToStr(*temp, lineNumber));
				exceedsSizeLines.AddToEnd(*",", 1);
				break;
			case -3: // Character size smaller than 2 characters
				_charSizeSmaller2++;
				sizeSmaller2CharLines.AddToEnd(*temp, IntToStr(*temp, lineNumber));
				sizeSmaller2CharLines.AddToEnd(*",", 1);
				break;
			case -4: // The verb is already exist
				_notUnique++;
				nonUniqueLines.AddToEnd(*temp, IntToStr(*temp, lineNumber));
				nonUniqueLines.AddToEnd(*",", 1);
				break;
			case 2: // Empty Lines
				_emptyLine++;
				emptyLines.AddToEnd(*temp, IntToStr(*temp, lineNumber));
				emptyLines.AddToEnd(*",", 1);
				break;
			};

			findNextLine((&file)[i]); // find the next line from file

			i += lineLenght;
			lineNumber++;
		} while ((&file)[i] != '\0');

		if (_successfull != 0)
			Log::Info(_successfull) << " verbs added successfully.\n";
		if (_nonAlphabetic != 0) {
			Log::Error(_nonAlphabetic) << " verbs contains non-alphabetic characters.\n" <<
				"These lines; " << nonAlphabeticLines.content << "\n";
		}
		if (_charExceedsSize != 0) {
			Log::Error(_charExceedsSize) << " verbs exceed " << VERB_CHAR_SIZE << " characters.\n" <<
				"These lines; " << exceedsSizeLines.content << "\n";
		}
		if (_charSizeSmaller2 != 0) {
			Log::Error(_charSizeSmaller2) << " verbs are smaller than 2 characters.\n" <<
				"These lines; " << sizeSmaller2CharLines.content << "\n";
		}
		if (_notUnique != 0) {
			Log::Error(_notUnique) << " verbs are not unique.\n" <<
				"These lines; " << nonUniqueLines.content << "\n";
		}
		if (_emptyLine != 0) {
			Log::Warning(_emptyLine) << " empty lines.\n" <<
				"These lines; " << emptyLines.content << "\n";
		}
	}

	int8 VerbHandler::AddNewVerb(const char& line) {
		// return 2   : Empty line
		// return +1  : The verb Successfully added
		// return -1  : There is some charachter which is not alphabetic
		// return -2  : Character size exceeds VERB_CHAR_SIZE size
		// return -3  : Character size smaller than 2 characters
		// return -4  : The verb is already exist

		char temp[150];
		int length = 0;
		uint8 regularity_id = 1;
		bool parenthesOpened = false;
		bool checkUniqueness = true;
		Exception ed_exception = Undefined;
		Exception s_exception = Undefined;
		Exception ing_exception = Undefined;

		for (int i = 0; i < sizeof(temp); i++) {
			char currentCh = ToLowerCase((&line)[i]);

			if (currentCh != 0) { // When char is in a-Z
				temp[length++] = currentCh;
			}
			else if ((&line)[i] == ' ' || (&line)[i] == '\t') // If char is blank space or new tab
				continue;
			else if ((&line)[i] == '\n' || (&line)[i] == '\0' || (&line)[i] == '#') // when new line or terminal char 
			{
				if (length > 1) { // If the temp array is not empty
					if (length > VERB_CHAR_SIZE) {
						return -2;
					}
					if (regularity_id == 1) // the verb is regular
					{
						if (checkUniqueness) { // Default value is true
							if (FindVerb(*temp, length) == 0) // Check Uniqueness
								CreateNewVerb(*temp, length, Undefined, s_exception, ing_exception);
							else
								return -4; // The base verb is already exist
						}
						else { // If user disabled this feature
							CreateNewVerb(*temp, length, Undefined, s_exception, ing_exception);
						}
					}
					else { // Ending of the irregular verbs
						Verb* verb_adress = &CreateNewVerb(*temp, length, IrregularVerb_V3);
						CreateIrregularVerb(*verb_adress);
					}
				}
				else if (length == 1) { // Verb cannot be one char
					return -3;
				}

				break;
			}
			else if ((&line)[i] == '-')  // some verbs can contain '-'
				temp[length++] = (&line)[i];
			else if ((&line)[i] == ',') { // irregular verb input
				// Length control
				if (length > VERB_CHAR_SIZE) { // charachter size exceeds VERB_CHAR_SIZE size
					return -2;
				}
				else if (length == 0) { // When there is no word in temp
					continue;
				}
				else if (length == 1) { // there can be no verb with one char
					return -3;
				}


				// If parathes opened in the back
				if (parenthesOpened) {
					Verb* verb_adress = nullptr;


					if (regularity_id == 2) {
						// Create new verb in index buffer THEN add irregular verb to the last collection
						verb_adress = &CreateNewVerb(*temp, length, IrregularVerb_V2);
					}
					else if (regularity_id == 3) {
						// Create new verb in index buffer THEN add irregular verb to the last collection
						verb_adress = &CreateNewVerb(*temp, length, IrregularVerb_V3);
					}
					else if (regularity_id == 1) {
						char warning[20];
						CopyData(*warning, *temp, length);
						warning[length] = '\0';
						Log::Warning("Base verb '") << warning <<
							"' cannot start with parentheses! Please find and remove it!\n";
						return -1;
					}

					CreateIrregularVerb(*verb_adress);
					length = 0;
				}
				else { // Parentheses closed correctly
					Verb* verb_adress = nullptr;

					if (regularity_id == 1) { // the verb is V1
						// Create new verb in index buffer THEN create irregular verb collection
						if (FindVerb(*temp, length) == 0) // If base form is unique
							verb_adress = &CreateNewVerb(*temp, length, IrregularVerb, s_exception, ing_exception);
						else
							return -4; // The base verb is already exist

						s_exception = Undefined;
						ing_exception = Undefined;
					}
					else if (regularity_id == 2) {
						// Create new verb in index buffer THEN add irregular verb to the last collection
						verb_adress = &CreateNewVerb(*temp, length, IrregularVerb_V2);
					}
					else if (regularity_id == 3) { // When you forgot the closing the parantheses at the V3
						verb_adress = &CreateNewVerb(*temp, length, IrregularVerb_V3);
						char warning[20];
						CopyData(*warning, *temp, length);
						warning[length] = '\0';
						Log::Warning("At the end of this verb '") << warning <<
							"' ends with comma(',') please find and remove it!\n";
						break;
					}

					CreateIrregularVerb(*verb_adress);
					length = 0;
					regularity_id += 1;
				}
			}
			else if ((&line)[i] == '(') {
				parenthesOpened = true;
			}
			else if ((&line)[i] == ')') { // Add last verb 
				parenthesOpened = false;

				// Length control
				if (length > VERB_CHAR_SIZE) { // charachter size exceeds VERB_CHAR_SIZE size
					return -2;
				}
				else if (length == 0) { // When there is no word in temp
					continue;
				}
				else if (length == 1) { // there can be no verb with one char
					return -3; // One char exception
				}

				// Add verbs according to their regularity
				if (regularity_id == 2) {
					// Create new verb in index buffer THEN add irregular verb to the last collection
					Verb* verb_adress = &CreateNewVerb(*temp, length, IrregularVerb_V2);
					CreateIrregularVerb(*verb_adress);
				}
				else if (regularity_id == 3) {
					// Create new verb in index buffer THEN add irregular verb to the last collection
					Verb* verb_adress = &CreateNewVerb(*temp, length, IrregularVerb_V3);
					CreateIrregularVerb(*verb_adress);
					break;
				}

				length = 0;
				regularity_id += 1;
			}
			else if ((&line)[i] == '/') {
				if (DoesContain(*"I0", (&line)[i + 1])) { // I0: ing except none
					ing_exception = None;
					i += 2;
				}
				else if (DoesContain(*"S0", (&line)[i + 1])) { // S0 : S except none
					s_exception = None;
					i += 2;
				}
				else if (DoesContain(*"DL", (&line)[i + 1])) { // DL : Double last char
					ing_exception = DoubleLastChar;
					ed_exception = DoubleLastChar;
					i += 2;
				}
				else if (DoesContain(*"U0", (&line)[i + 1])) { // U0 : don't check uniqueness
					checkUniqueness = false;
					i += 2;
				}
				else {
					return -1; // There is some character which is not alphabetic
				}
			}
			else
				return -1; // There is some character which is not alphabetic
		}

		if (length > 0)
			return 1; // Successfly added
		else
			return 2; // Line was empty
	}


	void VerbHandler::CreateIrregularVerb(Verb& verb) {
		if (&verb == nullptr) {
			// Do nothing
		}
		else if (verb.exceptFor_ED == IrregularVerb) { // Create new collection
			if (irrVerbCollection.capacity > irrVerbCollection.amount) { // If there is enough space for new collection
				// Do nothing
			}
			else { // IF space is not enough
				if (irrVerbCollection.capacity > 0) { // If current capacity bigger than 0
					int newSize = irrVerbCollection.capacity * SIZE_INCREASE_COEFFICIENT;
					// Hold old pointer
					IrrVerb* p_old_data = irrVerbCollection.verbs;
					// Create new heap space
					irrVerbCollection.verbs = new IrrVerb[newSize];
					// Update new capacity
					irrVerbCollection.capacity = newSize;
					// Copy Old data to new one
					CopyData(*irrVerbCollection.verbs, *p_old_data, irrVerbCollection.amount);
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
		else if (verb.exceptFor_ED == IrregularVerb_V2) { // use the last collection PAST SIMPLE
			if (irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastSimple == nullptr)
				irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastSimple = &verb;
			else
				irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastSimple_2 = &verb;
		}
		else if (verb.exceptFor_ED == IrregularVerb_V3) { // use the last collection PAST PARTICIPLE
			if (irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastParticiple == nullptr)
				irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastParticiple = &verb;
			else
				irrVerbCollection.verbs[irrVerbCollection.amount - 1].pastParticiple_2 = &verb;
		}
	}

	Verb& VerbHandler::CreateNewVerb(const char& verb_chars, const int& str_lenght, Exception ed_except,
		Exception s_except, Exception ing_except) {
		int row = (&verb_chars)[0] - 97; // 97 means 'a' in ASCII code
		int col;

		if ((&verb_chars)[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = (&verb_chars)[1] - 97; // 97 means 'a' in ASCII code


		// Check buffer size
		if (buffer[row * VERB_COL + col].verbCapacity > buffer[row * VERB_COL + col].verbAmount) {
			// There is enough space for new verb
		}
		else { // There is no empyt slot to put new word
			if (buffer[row * VERB_COL + col].verbCapacity > 0) { // If current capacity bigger than 0
				// Calculating coefficient for new verb heap size
				int newSize = buffer[row * VERB_COL + col].verbCapacity * SIZE_INCREASE_COEFFICIENT;
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
					if (buffer[row * VERB_COL + col].verbs[item].exceptFor_ED == IrregularVerb ||
						buffer[row * VERB_COL + col].verbs[item].exceptFor_ED == IrregularVerb_V2 ||
						buffer[row * VERB_COL + col].verbs[item].exceptFor_ED == IrregularVerb_V3) {
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
		CopyData(*buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].chars, verb_chars, str_lenght);
		buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].length = str_lenght;

		// Check Exceptions... -S -ING -ED
		if (ed_except == IrregularVerb)
		{
			// Change regularity of the verb
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_ED = IrregularVerb;

			// Check S exception
			if (s_except == Undefined) {
				CheckException_S(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_S = s_except;
			}
			// Check ING exception
			if (ing_except == Undefined) {
				CheckException_ING(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_ING = ing_except;
			}
		}
		else if (ed_except == IrregularVerb_V2) {
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_ED = IrregularVerb_V2;
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_ING = IrregularVerb_V2;
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_S = IrregularVerb_V2;
		}
		else if (ed_except == IrregularVerb_V3) {
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_ED = IrregularVerb_V3;
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_ING = IrregularVerb_V3;
			buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_S = IrregularVerb_V3;
		}
		else { // When the verb is regular
			// Check ED exception
			if (ed_except == Undefined) {
				CheckException_ED(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_ED = ed_except;
			}
			// Check S exception
			if (s_except == Undefined) {
				CheckException_S(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_S = s_except;
			}
			// Check ING exception
			if (ing_except == Undefined) {
				CheckException_ING(buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount]);
			}
			else {
				buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount].exceptFor_ING = ing_except;
			}

		}

		// Increase last empty index by one
		buffer[row * VERB_COL + col].verbAmount += 1;

		// Return the newly added verb's adress
		return buffer[row * VERB_COL + col].verbs[buffer[row * VERB_COL + col].verbAmount - 1];
	}


	int8 VerbHandler::ParseVerb(const char& verb_chars, char& out_result, bool parse_flag) const {
		// return 2   : Empty line
		// return +1  : The verb Successfully found.
		// return -1  : No verb found.
		// return -2  : There is some characters which is not alphabetic
		// return -3  : First character cannot be hyphen
		// return -4  : Character size less than 2 characters
		// return -5  : Character size exceeds VERB_CHAR_SIZE size

		char temp[150];
		uint8 lenght = 0;

		// Make all chars lowercase and get rid of spaces
		for (int i = 0; (&verb_chars)[i] != '\0'; i++) {
			char currentCh = ToLowerCase((&verb_chars)[i]);

			if (currentCh != 0) // If the char is alphabetic
				temp[lenght++] = currentCh;
			else if ((&verb_chars)[i] == ' ' || (&verb_chars)[i] == '\t')
				continue;
			else if ((&verb_chars)[i] == '-') { // If the charachter is hyphen
				if (i == 0)
					return -3; // First character cannot be hyphen
				else
					temp[lenght++] = (&verb_chars)[i]; // Add hy
			}
			else
				return -2; // There is some character which is not alphabetic
		}

		// Size control
		if (lenght < 2)
			return -4; // Character size smaller than 2 characters
		else if (lenght > VERB_CHAR_SIZE)
			return -5; // Character size exceeds VERB_CHAR_SIZE size


#define VERB_RESULT_ARRAY_SIZE 3
		Verb* foundVerbs[VERB_RESULT_ARRAY_SIZE];
		for (uint8 i = 0; i < VERB_RESULT_ARRAY_SIZE; i++) {
			foundVerbs[i] = nullptr;
		}


		// Search if the verb in native form
		int8 resultVerbsAmount = FindVerb(*temp, lenght, true, foundVerbs);
		if (resultVerbsAmount > 0) {
			uint16 size_verb_data = 0;

			for (int8 i = 0; i < resultVerbsAmount; i++) {
				(&out_result)[size_verb_data++] = digitToChar(i + 1);
				size_verb_data += CopyStr((&out_result)[size_verb_data], *". ");
				size_verb_data += GetVerbData(*foundVerbs[i], (&out_result)[size_verb_data]);
			}

			(&out_result)[size_verb_data - 1] = '\0';

			if (!parse_flag)
				return 1;
		}

		if (parse_flag) {
			// Search if the verb has -ed 
			// Search if the verb has -s
			// Search if the verb has -ing
		}

		return -1;
	}

#pragma region Suffix Parser
	int8 VerbHandler::ED_Parser(const char& verb, const uint8& lenght) {
		// return -1: verb couldn't find
		// return  1: verb found

		// None, EndsWith_w_x_y, EndsWith_Cy, EndsWith_e, EndsWith_XVC, DoubleLastChar
		// -ed,		-ed,		-ied		, -d		, -X + ED		,-X + ED 	  

		Verb* foundVerbs[VERB_RESULT_ARRAY_SIZE];
		for (uint8 i = 0; i < VERB_RESULT_ARRAY_SIZE; i++) {
			foundVerbs[i] = nullptr;
		}

		if ((&verb)[lenght - 2] == 'e' && (&verb)[lenght - 1] == 'd') { // when the last 2 chars is -ed

		}
		else {
			return -1;
		}
	}
	int8 VerbHandler::S_Parser(const char& verb, const uint8& lenght) {

	}
	int8 VerbHandler::ING_Parser(const char& verb, const uint8& lenght) {

	}
#pragma endregion


	int8 VerbHandler::FindVerb(const char& verb_chars, const int& length, bool hold_verb, Verb** out_verbs) const {
		// return -1 : verb is smaller than 2 chars
		// return 0  : no verb found
		// return 1  : 1 verb found
		// return 2  : 2 verb found
		// return 3  : 3 verb found
		int8 result = 0;

		if (length < 2) // Verb cannot be one character
			return -1;

		int row = (&verb_chars)[0] - 97; // 97 means 'a' in ASCII code

		int col;

		if ((&verb_chars)[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = (&verb_chars)[1] - 97; // 97 means 'a' in ASCII code

		for (int i = 0; i < buffer[row * VERB_COL + col].verbAmount; i++) { // Get all verbs from buffer
			if (buffer[row * VERB_COL + col].verbs[i].length == length) {
				if (IsSameStr(*buffer[row * VERB_COL + col].verbs[i].chars, verb_chars, length)) {
					if (hold_verb) {
						if (result < VERB_RESULT_ARRAY_SIZE)
						{
							(out_verbs)[result] = &buffer[row * VERB_COL + col].verbs[i];
							result++;
						}
					}
					else {
						result++;
						return result;
					}
				}
			}
		}

		return result;
	}

	int8 VerbHandler::SearchVerb(const char& verb_chars, const int& length,
		const Exception ed_, const Exception s_, const Exception ing_, Verb*& out_verbs) {
		// return -2 : no identifer
		// return -1  : results more than VERB_RESULT_ARRAY_SIZE verbs
		// return 0  : no verb found
		// return 1  : 1 verb found
		// return 2  : 2 verb found
		// return 3  : 3 verb found...

		int8 result = 0;


		int row = (&verb_chars)[0] - 97; // 97 means 'a' in ASCII code
		int col;
		if ((&verb_chars)[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = (&verb_chars)[1] - 97; // 97 means 'a' in ASCII code

		auto typeIdentifier = [&](int i) {
			if (ed_ != Undefined) {
				return ed_ == buffer[row * VERB_COL + col].verbs[i].exceptFor_ED;
			}
			else if (s_ != Undefined) {
				return s_ == buffer[row * VERB_COL + col].verbs[i].exceptFor_S;
			}
			else if (s_ != Undefined) {
				return ing_ == buffer[row * VERB_COL + col].verbs[i].exceptFor_ING;
			}
			return false;
		};


		for (int i = 0; i < buffer[row * VERB_COL + col].verbAmount; i++) { // Get all verbs from buffer
			if (buffer[row * VERB_COL + col].verbs[i].length == length) {
				if (typeIdentifier(i)) { // When the type same with parameter
					if (IsSameStr(*buffer[row * VERB_COL + col].verbs[i].chars, verb_chars, length)) {
						if (result < VERB_RESULT_ARRAY_SIZE)
						{
							(&out_verbs)[result] = &buffer[row * VERB_COL + col].verbs[i];
							result++;
						}
						else {
							return -1;
						}
					}
				}
			}
		}

		return result;
	}

#undef VERB_RESULT_ARRAY_SIZE

	uint16 VerbHandler::GetVerbData(const Verb& verb, char& out_string) const {
		if (&verb == nullptr) // If the object is empty
			return 0;

		auto exceptionToStr = [](const Exception& ex_type, char& out_str) {
			uint32 length = 0;
			switch (ex_type)
			{
			case verb::None:
				length = CopyStr(out_str, *VERB_EX_NONE);
				break;
			case verb::IrregularVerb:
				length = CopyStr(out_str, *VERB_EX_IRREGULAR_V1);
				break;
			case verb::IrregularVerb_V2:
				length = CopyStr(out_str, *VERB_EX_IRREGULAR_V2);
				break;
			case verb::IrregularVerb_V3:
				length = CopyStr(out_str, *VERB_EX_IRREGULAR_V3);
				break;
			case verb::DoubleLastChar:
				length = CopyStr(out_str, *VERB_EX_DOUBLE_LAST);
				break;
			case verb::EndsWith_e:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_E);
				break;
			case verb::EndsWith_Cy:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_CY);
				break;
			case verb::EndsWith_z:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_Z);
				break;
			case verb::EndsWith_s:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_S);
				break;
			case verb::EndsWith_x:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_X);
				break;
			case verb::EndsWith_o:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_O);
				break;
			case verb::EndsWith_ss:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_SS);
				break;
			case verb::EndsWith_zz:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_ZZ);
				break;
			case verb::EndsWith_ch:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_CH);
				break;
			case verb::EndsWith_sh:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_SH);
				break;
			case verb::EndsWith_w_x_y:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_WXY);
				break;
			case verb::EndsWith_Ce:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_CE);
				break;
			case verb::EndsWith_ie:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_IE);
				break;
			case verb::EndsWith_XVC:
				length = CopyStr(out_str, *VERB_EX_ENDSWITH_XVC);
				break;
			default:
				length = CopyStr(out_str, *"Undefined");
				break;
			}
			return length;
		};

		uint16 lastIndex = 0;
		CopyData(out_string, *verb.chars, verb.length);
		lastIndex += verb.length;

		lastIndex += CopyStr((&out_string)[lastIndex], *"\t[ED_E]: ");
		lastIndex += exceptionToStr(verb.exceptFor_ED, (&out_string)[lastIndex]);

		lastIndex += CopyStr((&out_string)[lastIndex], *", [S_E]: ");
		lastIndex += exceptionToStr(verb.exceptFor_S, (&out_string)[lastIndex]);

		lastIndex += CopyStr((&out_string)[lastIndex], *", [ING_E]: ");
		lastIndex += exceptionToStr(verb.exceptFor_ING, (&out_string)[lastIndex]);

		(&out_string)[lastIndex++] = '\n';
		return lastIndex;
	}

	void VerbHandler::GetVerbsWithIndex(const char& index_couple, char& out_string) {
		int row = (&index_couple)[0] - 97; // 97 means 'a' in ASCII code
		int col;

		if ((&index_couple)[1] == '-')
			col = 26; // 26 reserved for -
		else
			col = (&index_couple)[1] - 97; // 97 means 'a' in ASCII code


		uint32 lastIndex = 0;
		for (int i = 0; i < buffer[row * VERB_COL + col].verbAmount; i++) {
			lastIndex += GetVerbData(buffer[row * VERB_COL + col].verbs[i], (&out_string)[lastIndex]);
		}

		(&out_string)[lastIndex] = '\0';
	}

	int8 VerbHandler::UpdateIrrVerbAdress(Verb& new_address, const Verb& old_adress) {

		for (uint16 i = 0; i < irrVerbCollection.amount; i++) {
			if (new_address.exceptFor_ED == IrregularVerb) {
				if (irrVerbCollection.verbs[i].baseForm == &old_adress) {
					irrVerbCollection.verbs[i].baseForm = &new_address;
					return 1;
				}
			}
			else if (new_address.exceptFor_ED == IrregularVerb_V2) {
				if (irrVerbCollection.verbs[i].pastSimple == &old_adress) {
					irrVerbCollection.verbs[i].pastSimple = &new_address;
					return 1;
				}
				else if (irrVerbCollection.verbs[i].pastSimple_2 == &old_adress) {
					irrVerbCollection.verbs[i].pastSimple_2 = &new_address;
					return 1;
				}
			}
			else if (new_address.exceptFor_ED == IrregularVerb_V3) {
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

	uint16 VerbHandler::GetAllIrregularVerbs(char& out_string) const {
		int lastCharIndex = 0;

		for (int i = 0; i < irrVerbCollection.amount; i++) {
			// Pase form copy
			lastCharIndex += CopyStr((&out_string)[lastCharIndex], *"[Base]:");
			CopyData((&out_string)[lastCharIndex], *irrVerbCollection.verbs[i].baseForm->chars,
				irrVerbCollection.verbs[i].baseForm->length);
			lastCharIndex += irrVerbCollection.verbs[i].baseForm->length;

			// Past simple forms copy
			if (irrVerbCollection.verbs[i].pastSimple != nullptr) {
				lastCharIndex += CopyStr((&out_string)[lastCharIndex], *"\t[PastSimple]:");

				CopyData((&out_string)[lastCharIndex], *irrVerbCollection.verbs[i].pastSimple->chars,
					irrVerbCollection.verbs[i].pastSimple->length);
				lastCharIndex += irrVerbCollection.verbs[i].pastSimple->length;

				if (irrVerbCollection.verbs[i].pastSimple_2 != nullptr) {
					lastCharIndex += CopyStr((&out_string)[lastCharIndex], *", ");
					CopyData((&out_string)[lastCharIndex], *irrVerbCollection.verbs[i].pastSimple_2->chars,
						irrVerbCollection.verbs[i].pastSimple_2->length);
					lastCharIndex += irrVerbCollection.verbs[i].pastSimple_2->length;
				}
			}

			// Past Participle forms copy
			if (irrVerbCollection.verbs[i].pastParticiple != nullptr) {
				lastCharIndex += CopyStr((&out_string)[lastCharIndex], *"\t[PastParticiple]:");

				CopyData((&out_string)[lastCharIndex], *irrVerbCollection.verbs[i].pastParticiple->chars,
					irrVerbCollection.verbs[i].pastParticiple->length);
				lastCharIndex += irrVerbCollection.verbs[i].pastParticiple->length;

				if (irrVerbCollection.verbs[i].pastParticiple_2 != nullptr) {
					lastCharIndex += CopyStr((&out_string)[lastCharIndex], *", ");
					CopyData((&out_string)[lastCharIndex], *irrVerbCollection.verbs[i].pastParticiple_2->chars,
						irrVerbCollection.verbs[i].pastParticiple_2->length);
					lastCharIndex += irrVerbCollection.verbs[i].pastParticiple_2->length;
				}
			}

			(&out_string)[lastCharIndex++] = '\n';
		}

		(&out_string)[--lastCharIndex] = '\0';
		return irrVerbCollection.amount;
	}




#pragma region Exception Controllers

	void VerbHandler::CheckException_S(Verb& verb) const {
		// None, EndsWith_Cy, EndsWith_ss, EndsWith_zz, EndsWith_ch, EndsWith_sh, EndsWith_s, EndsWith_z, EndsWith_x, EndsWith_o

		// Check if second to last character is consonant and last char is y
		if (!IsVowel(verb.chars[verb.length - 2]) && verb.chars[verb.length - 1] == 'y') {
			verb.exceptFor_S = EndsWith_Cy;
			return;
		}
		else if (verb.chars[verb.length - 1] == 's') { // When last character is s
			if (verb.chars[verb.length - 2] == 's') // when verb ends with ss
				verb.exceptFor_S = EndsWith_ss;
			else
				verb.exceptFor_S = EndsWith_s;
			return;
		}
		else if (verb.chars[verb.length - 1] == 'z') { // When last character is z
			if (verb.chars[verb.length - 2] == 'z') // when verb ends with zz
				verb.exceptFor_S = EndsWith_zz;
			else
				verb.exceptFor_S = EndsWith_z;
			return;
		}
		else if (verb.chars[verb.length - 1] == 'h') { // When last character is h
			if (verb.chars[verb.length - 2] == 's') {// when verb ends with sh
				verb.exceptFor_S = EndsWith_sh;
				return;
			}
			else if (verb.chars[verb.length - 2] == 'c') { // when verb ends with ch
				verb.exceptFor_S = EndsWith_ch;
				return;
			}
		}
		else if (verb.chars[verb.length - 1] == 'x') { // When last character x
			verb.exceptFor_S = EndsWith_x;
			return;
		}
		else if (verb.chars[verb.length - 1] == 'o') { // When last character o
			verb.exceptFor_S = EndsWith_o;
			return;
		}

		verb.exceptFor_S = None;

	}

	void VerbHandler::CheckException_ING(Verb& verb) const {
		// None, EndsWith_w_x_y, EndsWith_Ce, EndsWith_ie, EndsWith_XVC, DoubleLastChar 


		if (verb.chars[verb.length - 1] == 'w' || verb.chars[verb.length - 1] == 'x' || verb.chars[verb.length - 1] == 'y') {
			verb.exceptFor_ING = EndsWith_w_x_y;
			return;
		}
		else if (verb.chars[verb.length - 1] == 'e') {
			if (!IsVowel(verb.chars[verb.length - 2])) { // Verb ends with consonant + vowel
				verb.exceptFor_ING = EndsWith_Ce;
				return;
			}
			else if (verb.chars[verb.length - 2] == 'i') { // Verb ends with ie
				verb.exceptFor_ING = EndsWith_ie;
				return;
			}
		}
		else if (!IsVowel(verb.chars[verb.length - 1])) {
			if (IsVowel(verb.chars[verb.length - 2])) {// Ends with Vowel + Constant
				if (verb.length < 3) // Length control
					return;

				bool isAllConsonant = true;
				for (int i = 0; i < verb.length - 2; i++) {
					if (IsVowel(verb.chars[verb.length - 3 - i])) {
						isAllConsonant = false;
						break;
					}
				}
				if (isAllConsonant && verb.length > 2) {
					verb.exceptFor_ING = EndsWith_XVC;
					return;
				}
			}
		}

		verb.exceptFor_ING = None;
	}

	void VerbHandler::CheckException_ED(Verb& verb) const {
		// None, EndsWith_w_x_y, EndsWith_Cy, EndsWith_e, EndsWith_XVC, DoubleLastChar

		if (verb.chars[verb.length - 1] == 'w' || verb.chars[verb.length - 1] == 'x') {
			verb.exceptFor_ED = EndsWith_w_x_y;
			return;
		}
		else if (verb.chars[verb.length - 1] == 'y') {
			if (!IsVowel(verb.chars[verb.length - 2]))
				verb.exceptFor_ED = EndsWith_Cy; // Consonant + y => -ied
			else
				verb.exceptFor_ED = EndsWith_w_x_y;
			return;
		}
		else if (verb.chars[verb.length - 1] == 'e') { // Verb ends with e
			verb.exceptFor_ED = EndsWith_e;
			return;
		}
		else if (!IsVowel(verb.chars[verb.length - 1])) {
			if (IsVowel(verb.chars[verb.length - 2])) {// Ends with Vowel + Constant
				if (verb.length < 3) // Length control
					return;

				bool isAllConsonant = true;
				for (int i = 0; i < verb.length - 2; i++) { // check to first if contains other vowel char 
					if (IsVowel(verb.chars[verb.length - 3 - i])) {
						isAllConsonant = false;
						break;
					}
				}
				if (isAllConsonant) {
					verb.exceptFor_ED = EndsWith_XVC;
					return;
				}
			}
		}

		verb.exceptFor_ED = None;
	}

#pragma endregion


}