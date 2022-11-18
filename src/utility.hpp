#ifndef MISC_HPP
#define MISC_HPP
#include "datatypes.hpp"

template <typename type>
inline void CopyData(type& dest, const type& target, const uint32 size) {
	for (uint32 i = 0; i < size; i++) {
		(&dest)[i] = (&target)[i];
	}
}

inline uint32 CopyStr(char& dest, const char& target, uint32 size = 0, const char break_chr = '\0') {
	int i = 0;
	if (size > 0) // When user sent the size
		for (; i < size; i++) {
			(&dest)[i] = (&target)[i];
		}
	else // User didn't mention the size
		while ((&target)[i] != '\0') {
			(&dest)[i] = (&target)[i];
			i++;
		}

	return i;
}

inline char ToLowerCase(const char& letter) { // returns the character if it's alpha otherwise returns 0
	if (letter > 64 && letter < 91) { // Capital letter
		return letter + 32; // Convert to lowercase version 
	}
	else if (letter > 96 && letter < 123) { // Char already lowercase
		return letter;
	}
	else { // Charachter is not alphabetic
		return 0;
	}
}

inline bool IsVowel(const char& letter) {
	switch (letter)
	{
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
		return true;
	default:
		return false;
		break;
	}
}

inline char digitToChar(const uint8& digit) {
	switch (digit)
	{
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	default:
		return 'N';
	}
}

inline uint8 IsEndingPunctuation(const char& one_char) {
	switch (one_char)
	{
	case  '.': // Full stop
		return 1;
	case  '?': // Question mark
		return 2;
	case  '!': // Exclamation point
		return 3;
	default:
		return 0;
	}
}

inline uint8 IsPunctuation(const char& one_char) {

	switch (one_char)
	{
	case  ',':	    // Comma
		return 1;
	case  '\"':		// Quotation Mark
		return 2;
	case  ':':		// Colon Mark
		return 3;
	case  ';':		// Semicolon Mark
		return 4;
	case  '(':		// Open Parantheses
		return 5;
	case  ')':		// Close Parantheses
		return 6;
	case  '[':		// Open Brackets
		return 7;
	case  ']':		// Close Brackets
		return 8;
	case  '/':		// Slash
		return 9;
	default:
		return 0;
	}
}

static bool DoesContain(const char& command_str, const char& line_str) { // True if the string contains command

	for (uint16 i = 0; i < UINT16_MAX; i++)
	{
		if ((&command_str)[i] == '\0') { // The string contains command and successfully ended
			return true;
		}

		if ((&line_str)[i] == '\0') {	 // When the first string ended
			if ((&command_str)[i] == ' ')
				return true;
			else
				return false;
		}

		if ((&line_str)[i] == (&command_str)[i]) { // when the chars are same
			continue;
		}
		else {
			return false; // When the chars are different
		}
	}

	return false; // The lenght of chars exceeds UINT16 MAX value
}

static inline bool IsSameStr(const char& command_str, const char& line_str, int length) { // True if the string contains command
	for (uint16 i = 0; i < length; i++)
	{
		if ((&line_str)[i] == (&command_str)[i]) { // when the chars are same
			continue;
		}
		else {
			return false; // When the chars are different
		}
	}

	return true; // The for loop ended successfully
}

static uint8 IntToStr(char& dest, const uint64& number) {
	uint64 newNumber;
	char reversed_arr[30];
	newNumber = number;
	uint8 out_size = 0;

	// Reduce and add to char array
	for (uint8 i = 0; i < 21; i++) {
		if (newNumber > 9) {
			out_size += 1;
			reversed_arr[i] = digitToChar(newNumber % 10);
			newNumber /= 10;
		}
		else {
			out_size += 1;
			reversed_arr[i] = digitToChar(newNumber);
			break;
		}
	}
	// swap chars 
	for (int i = 0; i < out_size; i++)
	{
		(&dest)[out_size - i - 1] = reversed_arr[i];
	}

	return out_size;
}

inline uint32 SizeOfStr(const char& str, const char& end_char = '\0') {
	for (uint32 i = 0; i < UINT32_MAX; i++) {
		if ((&str)[i] == end_char)
			return ++i;
	}

	return 0;
}

struct MyString
{
#define DEFAULT_MIN_CHAR_SIZE 100
#define SIZE_INCREASE_COEF 2

	char* content; // 8byte
	uint32 last_index; // 4 byte
	uint32 capacity;  // 4 byte

	MyString(char* address = nullptr, const uint32 value_cap = 0) {
		content = address;
		last_index = 0;
		capacity = value_cap;;
	}
	~MyString() {
		delete[] content;
	}
	void operator = (char* p_value) {
		if (content != nullptr)
			delete[] content; // Free old area
		content = p_value;
	}
	char& operator[](uint32 index) {
		return content[index];
	}

	int8 AddToEnd(const char& value, const uint32 size) {
		// 1	: OKAY
		// -1	: Char array haven't initialized
		// -2   : Char size exceeds capacity
		if (content != nullptr) {
			if (size + last_index > capacity) {
				CopyData(content[capacity - 4], *"...", 4);
				return -2; // char size exceeds capacity
			}
			else {
				if (last_index > 0)
					CopyData(content[--last_index], value, size);
				else
					CopyData(content[last_index], value, size);
				last_index += size;
				content[last_index++] = '\0';
			}
		}
		else {
			return -1; // The chars haven't initialized yet
		}

		return 0;
	}

	int8 EndString() {
		if (content != nullptr) {
			if (last_index + 1 < last_index)
				content[last_index++] = '\0';
			else
				return -2; // char size exceeds capacity
		}
		else {
			return -1; // The chars haven't initialized yet
		}

		return 1;
	}
};

#endif // !MISC_H
