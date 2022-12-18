#ifndef MAIN_HANDLER_HPP
#define MAIN_HANDLER_HPP

#include <vector>

// User defined libs
#include "verb.hpp"
#include "noun.hpp"
#include "tokenizer.hpp"
#include "simple_types.hpp"
#include "tokenizer.hpp"
#include "misc/string.hpp"

namespace handle {
	enum WordType : uint8 {
		Undefined,
		Noun,
		Verb,
		Pronoun,
		Adverb,
		Adjective,
		Preposition,
		Conjunction,
		Interjection,
	};

	struct Word {
		String* data; // address of data
		std::vector<WordType> type; // parsed types
	};

	class MainHandler {
	private: // variables
		noun::NounHandler noun;
		basic::BasicType pronoun;
		basic::BasicType adv;
		basic::BasicType adj;
		basic::BasicType prepos;
		basic::BasicType conj;
		basic::BasicType interj;

	public: // variables
		tkn::Tokenizer tokenize;
		verb::VerbHandler verb;
		std::vector<Word> words;

	private: // functions
		void ParseNoun(const char* raw_word, const uint8 length, String* out_string, int32* result);
		void ParseVerb(const String* raw_word, String* out_string, int32* result);
		void ParsePronoun(const char* raw_word, const uint8 length, String* out_string, int32* result);
		void ParseAdv(const char* raw_word, const uint8 length, String* out_string, int32* result);
		void ParseAdj(const char* raw_word, const uint8 length, String* out_string, int32* result);
		void ParsePrepos(const char* raw_word, const uint8 length, String* out_string, int32* result);
		void ParseConj(const char* raw_word, const uint8 length, String* out_string, int32* result);
		void ParseInterj(const char* raw_word, const uint8 length, String* out_string, int32* result);

	public: // functions
		MainHandler() = default;
		void Read(const WordType type, const char* data, const uint64& length);
		void Delete(const WordType type);
		void Parse(const String& str, const int32 word_id = -1);
		void Parse(const WordType type, const String& str);
		void ParseSentence(const String& str);
		void PrintResult(const WordType type, const int32& result, const String& str);
		void WordsToStr(String& out_str);
	};
}


#endif // !MAIN_HANDLER_HPP
