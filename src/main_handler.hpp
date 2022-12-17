#ifndef MAIN_HANDLER_HPP
#define MAIN_HANDLER_HPP



// User defined libs
#include "verb.hpp"
#include "noun.hpp"
#include "tokenizer.hpp"
#include "simple_types.hpp"

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
		const char* data; // address of data
		WordType type;
	};

	class MainHandler {
	private:
		noun::NounHandler noun;
		verb::VerbHandler verb;
		basic::BasicType pronoun;
		basic::BasicType adj;
		basic::BasicType adv;
		basic::BasicType prepos;
		basic::BasicType conj;
		basic::BasicType interj;
	public:
		MainHandler() = default;

		void Parse();
		void Delete();
		void Read();
	};
}


#endif // !MAIN_HANDLER_HPP
