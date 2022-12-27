#ifndef WORDS_HPP
#define WORDS_HPP

#include <vector>
#include "simple_types.hpp"

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

	Determiner,
	Quantifier,
	Punctuation,
	IndefiniteArticle,
};

struct TypeAndSuffixes
{
	WordType type;
	std::vector<uint8> suffixes;
};

struct WordToken {
	String* data; // address of token
	std::vector<TypeAndSuffixes> types; // parsed types
};

#endif // !WORDS_HPP
