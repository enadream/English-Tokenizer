#ifndef WORDS_HPP
#define WORDS_HPP

#include <vector>

enum WordType : uint8 {
	Undefined,

	Noun,
	Verb,
	AuxiliaryVerb,
	Pronoun,
	Adjective,
	Adverb,
	Preposition,
	Conjunction,
	Interjection,
	Determiner,
	Punctuation,
};


enum SuffixType : uint8 {
	BaseForm = 8,
	Negative = 50,

	m_suffix,
	s_suffix,
	re_suffix,
	ll_suffix,
	ve_suffix,
	
	adj_er,
	adj_xer,
	adj_ier,
	adj_r,

	adj_est,
	adj_xest,
	adj_iest,
	adj_st,
};

struct TypeAndSuffixes
{
	WordType type;
	std::vector<uint8> suffixes;
	char* adrs = nullptr;
};

struct WordToken {
	String* data; // address of token
	std::vector<TypeAndSuffixes> types; // parsed types
};

#endif // !WORDS_HPP
