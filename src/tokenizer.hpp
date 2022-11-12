#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include "datatypes.hpp"


#define TOKEN_CHAR_SIZE 3000    // 3000 byte
#define TOKEN_SIZE 700         // 11 200 byte
#define SENTENCE_AMOUNT 250    // 4000 byte

typedef struct Token      // 16 byte
{
	char* start_address;  // 8 byte
	uint32 index;         // 4 byte
	uint16 size;          // 2 byte	
}Token;


typedef struct Sentence   // 16 byte
{
	Token* first_token;   // 8 byte
	uint32 index;         // 4 byte
	uint16 token_amount;  // 2 byte	
}Sentence;

class Tokenizer {
private:
	Token tokens[TOKEN_SIZE];				// 11200 byte
	Sentence sentences[SENTENCE_AMOUNT];	// 4000 byte
	char characters[TOKEN_CHAR_SIZE];		// 3000 byte
	int32 lastTokenIndex;                   // 4 byte
	int32 lastSentenceIndex;				// 4 byte
	int32 lastCharIndex;                    // 4 byte
public:
	Tokenizer();
	void ParseClause(const char& sentence);
	void GetAllSentences(char& out_string) const;

};



#endif // !TOKENIZER_H
