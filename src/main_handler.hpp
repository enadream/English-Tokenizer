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
		basic::UnindexedList pronoun;
		basic::IndexedList adv;
		basic::IndexedList adj;
		basic::UnindexedList prepos;
		basic::UnindexedList conj;
		basic::UnindexedList interj;

	public: // variables
		tkn::Tokenizer tokenize;
		verb::VerbHandler verb;

	private: // functions
		void ParseNoun(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseVerb(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParsePronoun(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseAdv(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseAdj(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParsePrepos(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseConj(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseInterj(const String* raw_word, int32* result, String* out_string, const bool write_result);

	public: // functions
		MainHandler() = default;
		void Read(const WordType type, const char* data, const uint64& length, bool print_suc);
		void Delete(const WordType type);
		void ParseMultithread(const String& raw_word, const bool print_result, std::vector<Word>* words);
		void ParseWithType(const WordType type, const String& str);
		void ParseSentence(const String& str);
		void PrintResult(const WordType type, const int32& result, const String& str);
		void WordsToStr(std::vector<Word>& words, String& out_str);
	};
}


#endif // !MAIN_HANDLER_HPP
