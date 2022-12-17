#ifndef NOUN_HPP
#define NOUN_HPP

// User defined libs
#include "misc/data_types.hpp"
#include "misc/string.hpp"

#define NOUN_CHAR_SIZE 22

namespace noun {
	typedef enum Exception : uint8 {
		Undefined,
		None,
		IrrSingular,
		IrrPlural,
		Suffix_es,
		Suffix_zes,
		Suffix_ses,
		Suffix_0y_ies,
	};

	struct Noun // 24 byte
	{
		char chars[NOUN_CHAR_SIZE];
		uint8 length;
		Exception s;
	};

	struct NounIndexList // 16 byte
	{
		Noun* nouns;
		uint16 capacity;
		uint16 amount;
		char indicator[2];
	};

	struct IrregularNoun { // 16 byte
		Noun* singularForm;
		Noun* pluralForm;
	};

	struct IrregularNounList { // 16 byte 12 reserved
		IrregularNoun* nouns;
		uint16 capacity;
		uint16 amount;
	};

	class NounHandler {
	private:
		NounIndexList* nounIndexes;
		IrregularNounList irrNounList;

	private:
		void CheckException_S(Noun& noun) const;
		int16 S_Parser(const char* noun_chars, const uint8& lenght, Noun*& out_noun) const;
		void CreateIrrNoun(Noun& noun);
		int8 UpdateIrrNounAdress(Noun& new_address, const Noun& old_adress);

	public:
		NounHandler();
		~NounHandler();


		void DeleteAll();
		int8 AddNoun(const char* word_chars, const uint32& length);
		Noun& CreateNewNoun(const char* noun_chars, const uint32& str_length, const Exception& exception_p);
		void MultipleAdder(const char* file, const uint64& line_length);
		Noun* FindNoun(const char* word_chars, const uint8& length) const;
		Noun* FindWithException(const char* noun_chars, const int& length, Exception ex_type) const;
		int32 ParseNoun(const char* raw_word, const uint8& length, String& out_string) const;
		void ExceptionToStr(const Exception ex_type, String& out_string) const;
	};
}


#endif // !noun
