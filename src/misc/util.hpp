#ifndef UTIL_HPP
#define UTIL_HPP

#include "data_types.hpp"
#include "string.hpp"

namespace util {
	// Copy one array to another
	template <typename Type>
	inline void MemCpy(Type* dest, const Type* source, const uint32& length) {
		for (uint32 i = 0; i < length; i++) {
			dest[i] = source[i];
		}
	}

	// Returns size of an array until a value. The value is excluded.
	template <typename Type>
	inline uint32 SizeUntilValue(const Type* source, const Type value = '\0') {
		for (uint32 i = 0; i < UINT32_MAX; i++) {
			if (source[i] == value)
				return i; // The end_char excluded
		}
		return 0;
	}

	inline char DigitToChar(const int64& digit) noexcept{
		if (-1 < digit && digit < 10) {// 48 - 57 ASCII 0-9
			return digit + 48;
		}
		else {
			return 'N';
		}
	}

	template <typename Type>
	inline bool IsSameArray(const Type* first, const Type* second, const uint32& length) {
		for (uint32 i = 0; i < length; i++) {
			if (first[i] != second[i])
				return false;
		}
		return true;
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

	static uint8 IntToStr(String& dest, const uint64& number) {
		uint64 newNumber;
		char reversed_arr[30];
		char normal_arr[30];
		newNumber = number;
		uint8 out_size = 0;

		// Reduce and add to char array
		for (uint8 i = 0; i < 21; i++) {
			if (newNumber > 9) {
				out_size += 1;
				reversed_arr[i] = DigitToChar(newNumber % 10);
				newNumber /= 10;
			}
			else {
				out_size += 1;
				reversed_arr[i] = DigitToChar(newNumber);
				break;
			}
		}
		// Swap chars 
		for (int i = 0; i < out_size; i++)
		{
			normal_arr[out_size - i - 1] = reversed_arr[i];
		}

		// Copy to string
		dest.Append(normal_arr, out_size);

		return out_size;
	}

	static bool DoesContain(const char* source, const char* target) { // True if the string contains command
		for (uint16 i = 0; i < UINT16_MAX; i++)
		{
			if (source[i] == '\0') { // The string contains command and successfully ended
				return true;
			}
			if (target[i] == '\0') {	 // When the first string ended
				if (source[i] == ' ')
					return true;
				else
					return false;
			}
			if (target[i] == source[i]) { // when the chars are same
				continue;
			}
			else {
				return false; // When the chars are different
			}
		}
		return false; // The lenght of chars exceeds UINT16 MAX value
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
}

#endif // !UTIL_HPP

