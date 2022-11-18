#ifndef VERB_HPP
#define VERB_HPP

// User defined libs
#include "datatypes.hpp"
#define VERB_CHAR_SIZE 20

namespace verb {
	typedef enum Suffix_t : uint8 {
		Undefined,
		None,
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
		/*
		DoubleLastChar, // Double the last char, this option determineable only from dictionary /DL double last char i.e stopped
		EndsWith_e,  // Ends with e
		EndsWith_Cy, // Ends with consonant + y
		EndsWith_z,  // Ends with z
		EndsWith_s,  // Ends with s
		EndsWith_x,  // Ends with x
		EndsWith_o,  // Ends with o
		EndsWith_ss, // Ends with ss
		EndsWith_zz, // Ends with zz
		EndsWith_ch, // Ends with ch
		EndsWith_sh, // Ends with sh
		EndsWith_w_x_y, // Ends with w or x or y
		EndsWith_Ce, // Ends with consonant + e
		EndsWith_ie, // Ends with ie
		EndsWith_XVC // Contains one vowel and ends with consonant (Consonants + Vowel + Consonant) */
	}Suffix_t;

	typedef struct SuffixGroup { // 3 byte
		Suffix_t ed;
		Suffix_t s;
		Suffix_t ing;
		SuffixGroup() {}
		SuffixGroup(Suffix_t ed_, Suffix_t s_, Suffix_t ing_) : ed(ed_), s(s_), ing(ing_) {}
	}SuffixGroup;

	typedef struct Verb {    // 24 byte
		char chars[VERB_CHAR_SIZE];
		uint8 length;
		SuffixGroup suffixes;
	}Verb;

	typedef struct IrrVerb { // 40 byte
		Verb* baseForm;         // V1
		Verb* pastSimple;		// V2
		Verb* pastSimple_2;		// V2
		Verb* pastParticiple;	// V3
		Verb* pastParticiple_2;	// V3
	}IrrVerb;

	typedef struct VerbIndexList { // 16 byte 14 reserved
		Verb* verbs;
		uint16 verbCapacity;
		uint16 verbAmount;
		char indicator[2];
	}VerbIndexList;

	typedef struct IrrVerbsList { // 16 byte 12 reserved
		IrrVerb* verbs;
		uint16 capacity;
		uint16 amount;
	}IrrVerbsList;

	class VerbHandler { // 32 byte size
	private: // Variables
		VerbIndexList* buffer;
		IrrVerbsList irrVerbCollection;

	private: // Functions
		int16 ED_Parser(const char& verb, const uint8& lenght, Verb** out_verbs) const;
		int16 S_Parser(const char& verb, const uint8& lenght, Verb** out_verbs) const;
		int16 ING_Parser(const char& verb, const uint8& lenght, Verb** out_verbs) const;
		void CheckException_S(Verb& verb) const;
		void CheckException_ING(Verb& verb) const;
		void CheckException_ED(Verb& verb) const;
		int8 SearchVerb(const char& verb_chars, const int& length, Verb**& out_verbs, SuffixGroup& exception_p) const;
		Verb& CreateNewVerb(const char& verb_chars, const int& str_lenght, const SuffixGroup& exception_p);
		void CreateIrregularVerb(Verb& verb);
		int8 UpdateIrrVerbAdress(Verb& new_address, const Verb& old_adress);
		int8 AddNewVerb(const char& line);
		uint32 ExceptionToStr(const int8 type, const Suffix_t& ex_type, char& out_str) const;
		uint32 VerbToStr(const Verb& verb, char& out_string) const;
		uint32 VerbsToStr(Verb* const* verbs, uint16 verb_size, char& out_string) const;
	public: // Functions
		VerbHandler();
		~VerbHandler();
		void GetVerbsWithIndex(const char& index_couple, char& out_string);
		uint16 GetAllIrregularVerbs(char& out_string) const;
		int8 ParseVerb(const char& raw_chars, char& out_result, bool parse_flag = false) const;
		int8 FindVerb(const char& verb_chars, const int& length, bool hold_verb = false, Verb** out_verbs = nullptr) const;
		void MultipleVerbAdder(const char& file);
		int8 DeleteAll();
	};
}

#endif // !VERBS_H
