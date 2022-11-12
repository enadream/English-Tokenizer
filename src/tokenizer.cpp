#include "tokenizer.hpp"
#include "utility.hpp"

Tokenizer::Tokenizer() { // Constructor
	lastTokenIndex = -1;
	lastCharIndex = -1;
	lastSentenceIndex = -1;
}

void Tokenizer::ParseClause(const char& input_str) {

	bool newToken = true;
	bool newSentence = true;

	auto initNextToken = [&]() { // Initialize new token with lastCharIndex+1
		tokens[++lastTokenIndex].start_address = &characters[++lastCharIndex];
		tokens[lastTokenIndex].size = 1;
		tokens[lastTokenIndex].index = lastTokenIndex;
	};

	auto finishCurrentToken = [&]() { // Increase size and add terminal charachter
		tokens[lastTokenIndex].size += 1;
		characters[++lastCharIndex] = '\0';
	};

	uint8 is_ending = false;
	uint8 is_punctuation = false;

	for (int i = 0; i < 10000; i++) { // Parse sentences into tokens
		if ((&input_str)[i] == '\0') {
			break;
		}
		if ((&input_str)[i] != ' ' && (&input_str)[i] != '\n') {
			// If not ending
			if (is_ending = IsEndingPunctuation((&input_str)[i]))
				is_punctuation = false;
			else
				is_punctuation = IsPunctuation((&input_str)[i]);

			if (newToken) { // initialize new token
				if (is_ending) { // end the sentence 
					// example input "word ???" ??? has repetition
					if (newSentence) { // the first token of a sentence cannot be an ending punctuation i.e "what ???"
						if (lastTokenIndex >= 0) {
							// Add endind character to charachter, so override \0
							characters[lastCharIndex] = (&input_str)[i];

							// End opened token
							finishCurrentToken();
						}
						continue;
					}

					// example input "word ?"
					// Create new token for ending punctuation
					initNextToken();
					sentences[lastSentenceIndex].token_amount += 1;
					// Write last char to chars array
					characters[lastCharIndex] = (&input_str)[i];
					// End opened token
					finishCurrentToken();
					// Activate new sentence gate
					newSentence = true;

					continue;
				}

				// Start new token
				initNextToken();
				// Add character to token
				characters[lastCharIndex] = (&input_str)[i];

				if (is_punctuation) { // When the input is punctiation
					finishCurrentToken();
				}
				else {
					newToken = false;
				}

				if (newSentence) { // Checks if the new sentence activated
					sentences[++lastSentenceIndex].first_token = &tokens[lastTokenIndex];
					sentences[lastSentenceIndex].index = lastSentenceIndex;
					sentences[lastSentenceIndex].token_amount = 1;
					newSentence = false;
				}
				else { // increase token amount if new sentence is not activated
					sentences[lastSentenceIndex].token_amount += 1;
				}
			}
			else { // If token is continueing
				// if punctuation connect with letters i.e 5
				if (is_ending || is_punctuation) {

					// Finishing the current token
					finishCurrentToken();

					// Initializing next token
					initNextToken();
					sentences[lastSentenceIndex].token_amount += 1;

					// Add ending punctuation to token
					characters[lastCharIndex] = (&input_str)[i];

					// End new token and add terminal character
					finishCurrentToken();

					// Activate new token status
					newToken = true;

					if (is_ending) { // if ending
						newSentence = true;
					}

					// Continue loop to avoid charachter add
					continue;
				}

				tokens[lastTokenIndex].size += 1;
				characters[++lastCharIndex] = (&input_str)[i];
			}
		}
		else {
			if (newToken == false) { // When the ending of a token
				// increase the currect token size with 1 
				tokens[lastTokenIndex].size += 1;
				// add ending character to clauses to indicate token
				characters[++lastCharIndex] = '\0';
				newToken = true;
			}
		}
	}

	if (newToken == false) { // Finish the tokenizing
		// increase the currect token size with 1 
		tokens[lastTokenIndex].size += 1;
		// add ending character to clauses to indicate token
		characters[++lastCharIndex] = '\0';
		newToken = true;
	}
}

void Tokenizer::GetAllSentences(char& out_string) const {
	int lastStrIndex = -1;
	int lastTokenIndex = -1;

	auto writeIntToString = [&](const uint32& number) {
		uint32 newNumber;
		char reversed_arr[10];
		newNumber = number;
		uint8 out_size = 0;

		// Reduce and add to char array
		for (uint8 i = 0; i < 10; i++) {
			if (newNumber > 9) {
				out_size += 1;
				reversed_arr[i] = digitToChar(newNumber % 10);
				newNumber /= 10;
			}
			else {
				out_size += 1;
				reversed_arr[i] = digitToChar(newNumber);
				break;
			}
		}
		// swap chars 
		for (int i = 0; i < out_size; i++)
		{
			(&out_string)[out_size + lastStrIndex - i] = reversed_arr[i];
		}

		lastStrIndex += out_size;
	};

	for (uint32 i = 0; i < lastSentenceIndex + 1; i++) { // Copy each sentence
		(&out_string)[++lastStrIndex] = '[';
		writeIntToString(sentences[i].index);
		(&out_string)[++lastStrIndex] = ']';
		(&out_string)[++lastStrIndex] = ':';

		for (int j = 0; j < sentences[i].token_amount; j++) // Copy each token
		{
			(&out_string)[++lastStrIndex] = '[';

			lastTokenIndex += 1;
			for (int k = 0; k < tokens[lastTokenIndex].size - 1; k++) // Copy each char
			{
				(&out_string)[++lastStrIndex] = *(tokens[lastTokenIndex].start_address + k);
			}

			(&out_string)[++lastStrIndex] = ']';
			(&out_string)[++lastStrIndex] = ' ';
		}

		(&out_string)[++lastStrIndex] = '\n';
	}

	(&out_string)[++lastStrIndex] = '\0';

}
