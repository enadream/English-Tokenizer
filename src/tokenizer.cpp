#include "tokenizer.hpp"
#include "misc/util.hpp"

#define TOKEN_INCR_COEF 2
#define SENTENCE_DEBUG 1

namespace tokenizer {

#pragma region Sentence

	Sentence::Sentence(uint32 capacity_) {
		NewMemory(capacity_);
	}

	Sentence::~Sentence() {
		delete[] tokens;
	}

	void Sentence::NewMemory(uint32 new_capacity) {
		if (capacity < new_capacity) { // If space is not enough
			// Delete old space
			delete[] tokens;
			// Update capacity
			if (new_capacity > MIN_INCREASE_SIZE)
				capacity = new_capacity + 1; // Always add one more to considering \0 symbol
			else
				capacity = MIN_INCREASE_SIZE;
			// Create new space
			tokens = new String[capacity];
		}
	}

	void Sentence::IncreaseSize(uint32 add_capacity) {
		// Copy old pointer
		String* old_p = tokens;
		// update capacity
		if (capacity == 0)
			capacity = MIN_TOKEN_AMOUNT;
		else if (add_capacity > capacity * (TOKEN_INCR_COEF - 1))
			capacity += add_capacity;
		else
			capacity *= TOKEN_INCR_COEF;
		// Create new space
		tokens = new String[capacity];
		// Transfer string data to new space
		util::StrTransfer(tokens, old_p, amount);
		// delete old data
		delete[] old_p;
	}

	void Sentence::FreeSentence() {
		// delete tokens
		delete[] tokens;
		// set size and capacity 0
		tokens = nullptr;
		amount = 0;
		capacity = 0;
	}

	String& Sentence::CreateToken() {
		if (amount + 1 > capacity) {
			IncreaseSize();
		}
		amount += 1;
		return tokens[amount - 1];
	}

	String& Sentence::CreateToken(uint32 exact_size) {
		if (amount + 1 > capacity) {
			IncreaseSize();
		}
		// Allocate memory with exact_size
		tokens[amount].NewMemory(exact_size, true);
		amount += 1;
		return tokens[amount - 1];
	}

	String& Sentence::CreateToken(String& old_str) {
		if (amount + 1 > capacity) {
			IncreaseSize();
		}

		tokens[amount] << old_str;
		amount += 1;
		return tokens[amount - 1];
	}

	String& Sentence::CreateToken(const char* word, const uint32& token_length) {
		if (amount + 1 > capacity) {
			IncreaseSize();
		}

		tokens[amount].Append(word, token_length);
		amount += 1;
		return tokens[amount - 1];
	}



	String& Sentence::LastToken() {
#if SENTENCE_DEBUG
		if (amount == 0)
			__debugbreak();
#endif
		return tokens[amount - 1];
	}


	uint32 Sentence::Amount() const {
		return amount;
	}

	const String* Sentence::Tokens() const {
		return tokens;
	}

	String& Sentence::operator[](uint32 index) {
		return tokens[index];
	}

	const String& Sentence::operator[](uint32 index) const {
		return tokens[index];
	}
#pragma endregion


#pragma region Tokenizer
	int8 Tokenizer::ParseClause(const String& string) {
#define DEFAULT_TOKEN_SIZE 16

		uint32 lastSentenceIndex = 0;

		bool newToken = false;
		bool newSentence = false;
		bool isSentenceEmpty = false; // This has to be false so that if user wrongly start sentence with ending it won't crash

		uint8 is_ending = false;
		uint8 is_punctuation = false;

		auto createEmptyToken = [&]() {
			sentences[lastSentenceIndex].CreateToken(DEFAULT_TOKEN_SIZE);
			newToken = false;
		};

		// Create first empty token
		sentences[lastSentenceIndex].CreateToken(DEFAULT_TOKEN_SIZE);

		for (int i = 0; i < string.Length(); i++) { // Parse sentences into tokens
			if (string[i] == '\0') {
				break;
			}

			if (string[i] == ' ' || string[i] == '\n' || string[i] == '\t') {
				if (newToken) { // If requested new token
					createEmptyToken();
				}
				continue;
			}
			else if (util::IsPunctuation(string[i])) {
				// If last token is empty
				if (sentences[lastSentenceIndex].LastToken().Length() == 0)
					sentences[lastSentenceIndex].LastToken() += string[i];
				else // Create new token and assign value
					sentences[lastSentenceIndex].CreateToken(DEFAULT_TOKEN_SIZE / 4) += string[i];
				// Create new space
				createEmptyToken();
				isSentenceEmpty = false;
			}
			else if (util::IsEndingPunctuation(string[i])) {
				if (isSentenceEmpty) { // If sentence is empty add ending punctuation to last sentence
					sentences[lastSentenceIndex - 1].LastToken() += string[i];
				}
				else { // If current sentence not empty
					if (sentences[lastSentenceIndex].LastToken().Length() == 0)
						sentences[lastSentenceIndex].LastToken() += string[i];
					else // Create new token and assign ending punctuation
						sentences[lastSentenceIndex].CreateToken(DEFAULT_TOKEN_SIZE / 4) += string[i];

					lastSentenceIndex += 1;
					isSentenceEmpty = true;

					// Create new space
					createEmptyToken();
				}
			}
			else {
				sentences[lastSentenceIndex].LastToken() += string[i];
				isSentenceEmpty = false;
				newToken = true;
			}
		}

		if (isSentenceEmpty) {
			sentences[lastSentenceIndex].FreeSentence();
		}

		if (string.Length() > 0)
			return 1;
		else
			return 0;
	}

	int8 Tokenizer::GetAllSentences(String& string) const {
		int lastStrIndex = -1;
		int lastTokenIndex = 0;
		bool contentFound = false;

		for (uint32 i = 0; sentences[i].Tokens() != nullptr; i++) { // Copy each sentence
			string += '[';
			util::IntToStr(string, i + 1);
			string += "]:";

			for (int j = 0; j < sentences[i].Amount(); j++) // Copy each token
			{
				string += '[';
				string.Append(sentences[i][j].Chars(), sentences[i][j].Length());
				string += "] ";
				lastTokenIndex += 1;
			}

			string += '\n';

			contentFound = true;
		}

		string.EndString();

		if (contentFound)
			return 1;
		else
			return 0;
	}

	void Tokenizer::FreeAll() {
		for (uint32 i = 0; sentences[i].Tokens() != nullptr; i++) {
			sentences[i].FreeSentence();
		}
	}

}
#pragma endregion
