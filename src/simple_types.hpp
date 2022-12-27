#ifndef SIMPLE_TYPES_HPP
#define SIMPLE_TYPES_HPP

#include "misc/data_types.hpp"
#include "misc/string.hpp"

#define WORD_CHAR_SIZE 23

#define WORD_ROW 26
#define WORD_COLUMN 27

namespace basic {

	struct SimpleWord // 24 byte
	{
		char chars[WORD_CHAR_SIZE];
		uint8 length = 0;
	};

	class UnindexedList {
	private:
		SimpleWord* words = nullptr;
		uint32 amount = 0;
		uint32 capacity = 0;

	private: // Functions
		void IncreaseSpace();

	public:
		UnindexedList();
		~UnindexedList();

		void FreeAll();
		int8 AddWord(const char* word_chars, const uint32& length);
		void MultipleAdder(const char* file, const uint64& line_length, const char* type);
		int32 FindWord(const char* word_chars, const uint8& length);
		int8 ParseWord(const String& raw_string, String& out_string, const bool write_result);
	};

	struct WordList { // 16 byte
		SimpleWord* words;
		uint16 capacity;
		uint16 amount;
		char indicator[2];
	};

	class IndexedList {
	private:
		WordList* wordLists;

	private: // Functions
		SimpleWord* CreateWord(const char* word_chars, const uint32& str_length);

	public:
		IndexedList();
		~IndexedList();

		void FreeAll();
		int8 AddWord(const char* word_chars, const uint32& length);
		void MultipleAdder(const char* file, const uint64& line_length, const char* type);
		SimpleWord* FindWord(const char* word_chars, const uint8& str_length);
		int8 ParseWord(const String& raw_string, String& out_string, const bool write_result);
	};
}
#endif // !CLOSED_CLASSES_HPP
