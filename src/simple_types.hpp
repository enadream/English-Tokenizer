#ifndef SIMPLE_TYPES_HPP
#define SIMPLE_TYPES_HPP

#include "misc/data_types.hpp"
#include "misc/string.hpp"
#include "words.hpp"

#define WORD_CHAR_SIZE 23

#define WORD_ROW 26
#define WORD_COLUMN 27

namespace basic {
	struct SimpleWord // 24 byte
	{
		char chars[WORD_CHAR_SIZE];
		uint8 length = 0;
	};

	struct WordList { // 16 byte
		SimpleWord* words;
		uint16 capacity;
		uint16 amount;
		char indicator[2];
	};

	class UnindexedList {
	protected:
		SimpleWord* words = nullptr;
		uint32 amount = 0;
		uint32 capacity = 0;

	protected: // Functions
		void IncreaseSpace();

	public:
		UnindexedList();
		~UnindexedList();

		int8 AddWord(const char* word_chars, const uint32& length);
		void MultipleAdder(const char* file, const uint64& line_length, const char* type);
		int32 FindWord(const char* word_chars, const uint8& length) const;
		int8 ParseWord(const String& raw_string, String& out_string, const bool write_result);
		void Free();
	};

	class IndexedList {
	protected:
		WordList* wordLists;

	protected: // Functions
		SimpleWord* CreateWord(const char* word_chars, const uint32& str_length);

	public:
		IndexedList();
		~IndexedList();

		int8 AddWord(const char* word_chars, const uint32& length);
		void MultipleAdder(const char* file, const uint64& line_length, const char* type);
		SimpleWord* FindWord(const char* word_chars, const uint8& str_length) const;
		int8 ParseWord(const String& raw_string, String& out_string, const bool write_result);
		void Free();
	};

	class AuxiliaryVerb : public UnindexedList {
	private:
		int32 N_Parse(const char* word_chars, const uint8& length) const;

	public:
		int8 ParseWord(const String& raw_string, TypeAndSuffixes& word, String& out_string, const bool write_result) const;
	};

	class Pronoun : public UnindexedList {
	private:// m_suffix,		s_suffix,			re_suffix,			ll_suffix,			ve_suffix
		// To parse m suffix with pronoun I
		int8 M_Parse(const char* word_chars, const uint8& length) const;
		// To parse s suffix with pronouns He's She's It's, what's
		int8 S_Parse(const char* word_chars, const uint8& length) const;
		// To parse re suffix with pronouns you're
		int8 Re_Parse(const char* word_chars, const uint8& length) const;
		// To parse ll suffix with pronouns He'll, They'll 
		int8 Ll_Parse(const char* word_chars, const uint8& length) const;
		// To parse ve suffix with pronouns you've, they've, we've
		int8 Ve_Parse(const char* word_chars, const uint8& length) const;

	public:
		int8 ParseWord(const String& raw_string, TypeAndSuffixes& word, String& out_string, const bool write_result) const;
	};


	class Adjective : public IndexedList {
	private:
		// Comparative Parse
		int8 Com_Parse(const char* word_chars, const uint8& length, SimpleWord*& out_word) const;
		// Superlative Parse
		int8 Sup_Parse(const char* word_chars, const uint8& length, SimpleWord*& out_word) const;
	public:
		int8 ParseWord(const String& raw_string, TypeAndSuffixes& word, String& out_string, const bool write_result) const;
	};
}
#endif // !CLOSED_CLASSES_HPP
