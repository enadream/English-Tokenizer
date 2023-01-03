#ifndef MAIN_HANDLER_HPP
#define MAIN_HANDLER_HPP



// User defined libs
#include "verb.hpp"
#include "noun.hpp"
#include "tokenizer.hpp"
#include "simple_types.hpp"
#include "tokenizer.hpp"
#include "misc/string.hpp"
#include "words.hpp"
#include "file_system.hpp"

namespace handle {


	class MainHandler {
	private: // variables
		noun::NounHandler noun;
		basic::UnindexedList pronoun;
		basic::IndexedList adv;
		basic::IndexedList adj;
		basic::UnindexedList prepos;
		basic::UnindexedList conj;
		basic::UnindexedList interj;
		basic::AuxiliaryVerb aux_verb;
		basic::UnindexedList det;

	public: // variables
		tkn::Tokenizer tokenize;
		verb::VerbHandler verb;

	private: // functions
		void ParseNoun(const String* raw_word, int32* result, TypeAndSuffixes* suffix, String* out_string, const bool write_result);
		void ParseVerb(const String* raw_word, int32* result, TypeAndSuffixes* suffix, String* out_string, const bool write_result);
		void ParseAuxVerb(const String* raw_word, TypeAndSuffixes* word, int32* result, String* out_string, const bool write_result);
		void ParsePronoun(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseAdv(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseAdj(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParsePrepos(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseConj(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseInterj(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParseDet(const String* raw_word, int32* result, String* out_string, const bool write_result);
		void ParsePunc(const String* raw_word, int32* result);

	public: // functions
		MainHandler() = default;
		void Read(const WordType type, String& dir);
		void Delete(const WordType type);
		void ParseMultithread(const String& raw_word, const bool print_result, WordToken& word);
		void ParseWithType(const WordType type, const String& str);
		void ParseSentence(const String& str);
		void PrintResult(const WordType type, const int32& result, const String& str);
		void WordsToStr(std::vector<WordToken>& words, String& out_str);
		void WordToStr(WordToken& word, String& out_str);

		void Test(String& dir);
	};
}


#endif // !MAIN_HANDLER_HPP
