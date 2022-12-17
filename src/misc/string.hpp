#ifndef STRING_HPP
#define STRING_HPP

// User defined libs
#include "data_types.hpp"

#define MIN_INCREASE_SIZE 64 // Add 16 char each time
#define STRING_SIZE_INC_COEF 4

class String { // 16 byte
private: // Privates variables
	char* p_data = nullptr; // 8 byte
	uint32 length = 0; // stores length of the array
	uint32 capacity = 0; // stores capacity of the array

private: // Functions
	// Copy a string into this object and delete old buffer
	String& Copy(const String& string);
	// Copy char array into this object and delete old buffer
	String& CopyChars(const char* chrs);
	String& CopyChars(const char* chrs, uint32 length_);

	void MoveData(String& dead_string);
public:
	explicit String(); // Empty constructor
	~String(); // Destructor

	String(const char* str); // Start with char array
	explicit String(const char* str, uint32 length_); // Start with char array but limited length
	explicit String(const String& string); // Copy constructor
	explicit String(String&& old_str) noexcept; // Move semantics
	explicit String(const uint32& capacity_); // Start with capacity
	explicit String(const uint32& capacity_, bool IsExactSize);

	// Get members
	const char* Chars() const;
	char* GetPointer();
	uint32 Capacity() const;
	uint32 Length() const;
	// Set members
	String& SetLength(uint32 length_);

	// Create a new memory while deleting old one
	void NewMemory(uint32 new_capacity = MIN_INCREASE_SIZE);
	void NewMemory(uint32 new_capacity, bool IsExactSize);
	// Increase memory while preserving old one
	void IncreaseMemory(uint32 add_capacity = MIN_INCREASE_SIZE);
	// Append terminal character to end
	String& EndString();
	// Append a char array into this object, with length limitation
	String& Append(const char* str, const uint32 length_);
	// Repace a char array into this object, with length limitation
	String& Replace(const char* str, const uint32 length_);
	// Decrease length by value
	String& RemoveLast(uint32 value);
	// Free memory
	void Free();

	// Operators
	String& operator=(const String& string_);
	String& operator=(const char* str);  // Str always need's to have \0 char at the end!
	String& operator+=(const char* str); // Str always need's to have \0 char at the end!
	String& operator+=(const char& character);
	String& operator+=(const String& string);
	String& operator--(int); // Decrease length by one
	String& operator<<(String& old_str); // Move semantics
	bool operator==(const char* str) const;
	bool operator==(String& string) const;
	const char& operator[](uint32 index) const;
	char& operator[](uint32 index);
};

namespace util {
	inline void StrTransfer(String* dest, String* source, const uint32& length) {
		for (uint32 i = 0; i < length; i++) {
			dest[i] << source[i];
		}
	}
}

#endif