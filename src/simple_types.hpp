#ifndef SIMPLE_TYPES_HPP
#define SIMPLE_TYPES_HPP

#include "misc/data_types.hpp"
#include "misc/string.hpp"

#define WORD_CHAR_SIZE 23



namespace basic {

	struct Word // 24 byte
	{
		char chars[WORD_CHAR_SIZE];
		uint8 length = 0;
	};

	class BasicType {
	private:
		Word* words = nullptr;
		uint32 amount = 0;
		uint32 capacity = 0;
	public:
		BasicType();
		~BasicType();

		void IncreaseSpace();
		void FreeAll();
		int8 AddWord(const char* word_chars, const uint32& length);
		void MultipleAdder(const char* file, const uint64& line_length, const char* type);
		int32 FindWord(const char* word_chars, const uint8& length);
		int32 ParseWord(const char* raw_word, const uint8& length, String& out_string);
	};
}
#endif // !CLOSED_CLASSES_HPP
