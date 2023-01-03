#ifndef WORDS_HPP
#define WORDS_HPP

#include <vector>

enum WordType : uint8 {
	Undefined,
	Noun,
	Verb,
	AuxiliaryVerb,
	Pronoun,
	Adverb,
	Adjective,
	Preposition,
	Conjunction,
	Interjection,
	Determiner,
	Punctuation,

	Negative
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
