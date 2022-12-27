#ifndef NOUN_HPP
#define NOUN_HPP


// User defined libs
#include "misc/data_types.hpp"
#include "misc/string.hpp"
#include "words.hpp"

#define NOUN_CHAR_SIZE 22

namespace noun {
	typedef enum Exception : uint8 {
		// Suffixes
		Undefined,
		None,
		IrrSingular,
		IrrPlural,
		Suffix_es,
		Suffix_zes,
		Suffix_ses,
		Suffix_0y_ies,
		// Additions
		BaseForm,
		S_Parsed,
	};

	struct Noun // 24 byte
	{
		char chars[NOUN_CHAR_SIZE];
		uint8 length;
		Exception s;
	};

	struct NounList // 16 byte
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
		NounList* nounLists;
		IrregularNounList irrNounList;

	private:
		void CheckException_S(Noun& noun) const;
		uint8 S_Parser(const char* noun_chars, const uint8& lenght, std::vector<Noun*>& out_nouns) const;
		void CreateIrrNoun(Noun& noun);
		int8 UpdateIrrNounAdress(Noun& new_address, const Noun& old_adress);

	public:
		NounHandler();
		~NounHandler();

		void DeleteAll();
		int8 AddNoun(const char* word_chars, const uint32& length);
		Noun& CreateNewNoun(const char* noun_chars, const uint32& str_length, const Exception& exception_p);
		void MultipleAdder(const char* file, const uint64& line_length);
		uint8 FindNoun(const char* word_chars, const uint8& length, std::vector<Noun*>* out_nouns = nullptr) const;
		uint8 FindWithException(const char* noun_chars, const int& length, Exception ex_type, std::vector<Noun*>& out_nouns) const;
		int8 ParseNoun(const String& raw_string, TypeAndSuffixes& word, String& out_string, const bool write_result) const;
		void ExceptionToStr(const Exception ex_type, String& out_string) const;
	};
}


#endif // !noun
