#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

// User defined libs
#include "misc/data_types.hpp"
#include "misc/string.hpp"
#include "misc/array.hpp"

#define SENTENCE_AMOUNT 100    // 1600 byte
#define MIN_TOKEN_AMOUNT 16

namespace tokenizer {
	class Sentence // 16 byte
	{
	private:
		String* tokens = nullptr; // 8 byte
		uint32 amount = 0; // 4 byte
		uint32 capacity = 0; // 4 byte
	public:
		Sentence() = default;
		explicit Sentence(uint32 capacity_);
		~Sentence();

		void FreeSentence();
		void IncreaseSize(uint32 add_capacity = MIN_TOKEN_AMOUNT);
		void NewMemory(uint32 new_capacity);
		String& CreateToken();
		String& CreateToken(uint32 exact_size);
		String& CreateToken(const char* word, const uint32& token_length);
		String& CreateToken(String& old_str);
		String& LastToken();

		uint32 Amount()const;
		const String* Tokens() const;

		String& operator[](uint32 index);
		const String& operator[](uint32 index) const;

	};

	class Tokenizer {
	private:
		util::Array<Sentence, SENTENCE_AMOUNT> sentences; // 16 * SENTENCE_AMOUNT byte

	public:
		Tokenizer() = default;
		int8 ParseClause(const String& sentence);
		int8 GetAllSentences(String& out_string) const;
		void FreeAll();

	};
}

namespace util {
	inline uint8 IsEndingPunctuation(const char& one_char) {
		switch (one_char)
		{
		case  '.': // Full stop
			return 1;
		case  '?': // Question mark
			return 2;
		case  '!': // Exclamation point
			return 3;
		default:
			return 0;
		}
	}

	inline uint8 IsPunctuation(const char& one_char) {

		switch (one_char)
		{
		case  ',':	    // Comma
			return 1;
		case  '\"':		// Quotation Mark
			return 2;
		case  ':':		// Colon Mark
			return 3;
		case  ';':		// Semicolon Mark
			return 4;
		case  '(':		// Open Parantheses
			return 5;
		case  ')':		// Close Parantheses
			return 6;
		case  '[':		// Open Brackets
			return 7;
		case  ']':		// Close Brackets
			return 8;
		case  '/':		// Slash
			return 9;
		default:
			return 0;
		}
	}
}

#endif // !TOKENIZER_H
