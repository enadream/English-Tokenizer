#ifndef VERB_HPP
#define VERB_HPP

// User defined libs
#include "misc/data_types.hpp"
#include "misc/string.hpp"
#include "words.hpp"

#define VERB_CHAR_SIZE 20

namespace verb {
	typedef enum Suffix_t : uint8 {
		Undefined,
		None, // Default exception
		// -Ed Suffix
		IrregularVerb,	  // Base Form
		IrregularVerb_V2, // Past Simple
		IrregularVerb_V3, // Past Participle
		Suffix_d,
		Suffix_0y_ied,
		Suffix_X_ed,
		// -S Suffix
		Suffix_es,
		Suffix_zes,
		Suffix_ses,
		Suffix_0y_ies,
		// -Ing Suffix
		Suffix_0e_ing,
		Suffix_0ie_ying,
		Suffix_X_ing,
		// Additions
		BaseForm,
		ED_Parsed,
		S_Parsed,
		ING_Parsed,
	}Suffix_t;

	struct SuffixGroup { // 3 byte
		Suffix_t ed;
		Suffix_t s;
		Suffix_t ing;
		SuffixGroup() = default;
		SuffixGroup(Suffix_t ed_, Suffix_t s_, Suffix_t ing_) : ed(ed_), s(s_), ing(ing_) {}
	};

	struct Verb {    // 24 byte
		char chars[VERB_CHAR_SIZE];
		uint8 length;
		SuffixGroup suffixes;
	};

	struct IrrVerb { // 40 byte
		Verb* baseForm;         // V1
		Verb* pastSimple;		// V2
		Verb* pastSimple_2;		// V2
		Verb* pastParticiple;	// V3
		Verb* pastParticiple_2;	// V3
	};

	struct VerbIndexList { // 16 byte 14 reserved
		Verb* verbs;
		uint16 verbCapacity;
		uint16 verbAmount;
		char indicator[2];
	};

	struct IrrVerbsList { // 16 byte 12 reserved
		IrrVerb* verbs;
		uint16 capacity;
		uint16 amount;
	};

	class VerbHandler { // 32 byte size
	private: // Variables
		VerbIndexList* buffer;
		IrrVerbsList irrVerbCollection;

	private: // Functions
		uint8 ED_Parser(const char* verb_chars, const uint8& lenght, std::vector<Verb*>& out_verbs) const;
		uint8 ING_Parser(const char* verb_chars, const uint8& lenght, std::vector<Verb*>& out_verbs) const;
		uint8 S_Parser(const char* verb_chars, const uint8& lenght, std::vector<Verb*>& out_verbs) const;

		uint8 FindVerb(const char* verb_chars, const int& length, std::vector<Verb*>* out_verbs = nullptr) const;
		uint8 FindWithException(const char* verb_chars, const int& length, SuffixGroup& exception_p, std::vector<Verb*>& out_verbs) const;

		Verb& CreateVerb(const char* verb_chars, const int& str_lenght, const SuffixGroup& exception_p);
		void CreateIrregularVerb(Verb& verb);
		int8 UpdateIrrVerbAdress(Verb& new_address, const Verb& old_adress);
		int8 AddNewVerb(const char* line, const uint16& size);

		void CheckException_S(Verb& verb) const;
		void CheckException_ING(Verb& verb) const;
		void CheckException_ED(Verb& verb) const;

		void ExceptionToStr(const int8 type, const Suffix_t& ex_type, String& out_string) const;
		void VerbToStr(const Verb& verb, String& out_string) const;
	public: // Functions
		VerbHandler();
		~VerbHandler();
		void GetVerbsWithIndex(const char* index_couple, String& out_string);
		uint16 GetAllIrregularVerbs(String& out_string) const;
		int8 ParseVerb(const String& raw_string, TypeAndSuffixes& word, String& out_string, const bool write_result) const;
		void MultipleVerbAdder(const char* file, const uint64& size);
		Verb* FindBaseVerb(Verb* verb);
		int8 Free();
	};
}

#endif // !VERBS_H
