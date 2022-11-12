#ifndef VERBS_HPP
#define VERBS_HPP

// User defined libs
#include "datatypes.hpp"
#define VERB_CHAR_SIZE 20

namespace verb {
	typedef enum Exception : uint8 {
		Undefined,
		None,
		IrregularVerb, // this option determineable only from dictionary  // You need to show connections
		IrregularVerb_V2, // this option determineable only from dictionary  /V2
		IrregularVerb_V3, // this option determineable only from dictionary  /V3
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
		EndsWith_XVC // Contains one vowel and ends with consonant (Consonants + Vowel + Consonant)
	}Exception;

	typedef struct Verb {    // 24 byte
		char chars[VERB_CHAR_SIZE];
		uint8 length;
		Exception exceptFor_ED;
		Exception exceptFor_S;
		Exception exceptFor_ING;

	}Verb;

	typedef struct IrrVerb { // 40 byte
		Verb* baseForm;         // V1
		Verb* pastSimple;		// V2
		Verb* pastSimple_2;		// V2
		Verb* pastParticiple;	// V3
		Verb* pastParticiple_2;	// V3
	}IrregularVerbCollection;

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
	private:
		VerbIndexList* buffer;
		IrrVerbsList irrVerbCollection;
		int8 ED_Parser(const char& verb, const uint8& lenght);
		int8 S_Parser(const char& verb, const uint8& lenght);
		int8 ING_Parser(const char& verb, const uint8& lenght);
		void CheckException_S(Verb& verb) const;
		void CheckException_ING(Verb& verb) const;
		void CheckException_ED(Verb& verb) const;
		int8 SearchVerb(const char& verb_chars, const int& length,
			const Exception ed_, const Exception s_, const Exception ing_, Verb*& out_verbs);
		Verb& CreateNewVerb(const char& verb_chars, const int& str_lenght, Exception ed_except,
			Exception s_except = Undefined, Exception ing_except = Undefined);
		void CreateIrregularVerb(Verb& verb);
		int8 UpdateIrrVerbAdress(Verb& new_address, const Verb& old_adress);
		int8 AddNewVerb(const char& line);
	public:
		VerbHandler();
		~VerbHandler();
		void GetVerbsWithIndex(const char& index_couple, char& out_string);
		uint16 GetAllIrregularVerbs(char& out_string) const;
		uint16 GetVerbData(const Verb& verb, char& out_string) const;
		int8 ParseVerb(const char& verb_chars, char& out_result, bool parse_flag = false) const;
		int8 FindVerb(const char& verb_chars, const int& length, bool hold_verb = false, Verb** out_verbs = nullptr) const;
		void MultipleVerbAdder(const char& file);
		int8 DeleteAll();
	};
}

#endif // !VERBS_H
