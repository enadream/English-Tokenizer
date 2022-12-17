#include "string.hpp"
#include "util.hpp"

#define DEBUG_STRING 0

#if DEBUG_STRING
#include <stdio.h>
#endif // DEBUG


#pragma region Constructors
String::String() {
#if DEBUG_STRING
	printf("[Constructor]: String()\n");
#endif // DEBUG
}

String::~String() {
	delete[] p_data;
#if DEBUG_STRING
	printf("[DESTRUCTOR CALLED!]\n");
#endif // DEBUG
}

String::String(const char* str) {
	// Copy char array into buffer
	CopyChars(str);
#if DEBUG_STRING
	printf("[Constructor]: String(const char* str)\n");
#endif // DEBUG
}

String::String(const char* str, uint32 length_) { // Reading string until a spesific length
	// Copy chars with some length
	CopyChars(str, length_);
#if DEBUG_STRING
	printf("[Constructor]: String(const char* str, uint32 length_)\n");
#endif // DEBUG
}

String::String(const String& string) { // copy constructor
	// Copy string class
	Copy(string);
#if DEBUG_STRING
	printf("[Constructor]: String(const String& string_)\n");
#endif // DEBUG
}

String::String(String&& old_string) noexcept {
	p_data = nullptr;
	MoveData(old_string);
}

String::String(const uint32& capacity_) {
	NewMemory(capacity_);
}

String::String(const uint32& capacity_, bool IsExactSize) {
	NewMemory(capacity_);
}
#pragma endregion

#pragma region Class Functions
void String::IncreaseMemory(uint32 add_capacity) {
	// copy old pointer
	char* old_string = p_data;
	// update capacity
	if (capacity == 0)
		capacity = MIN_INCREASE_SIZE;
	else if (add_capacity > capacity * (STRING_SIZE_INC_COEF - 1))
		capacity += add_capacity + 1; // Always add one more to considering \0 symbol
	else
		capacity *= STRING_SIZE_INC_COEF;
	// create new space
	p_data = new char[capacity];
	// copy old data to new space
	util::MemCpy(p_data, old_string, length);
	// delete old space
	delete[] old_string;
}

void String::NewMemory(uint32 new_capacity) {
	if (capacity < new_capacity) { // If space is not enough
		// Delete old space
		delete[] p_data;
		// Update capacity
		if (new_capacity > MIN_INCREASE_SIZE)
			capacity = new_capacity + 1; // Always add one more to considering \0 symbol
		else
			capacity = MIN_INCREASE_SIZE;
		// Create new space
		p_data = new char[capacity];
	}
}

void String::NewMemory(uint32 new_capacity, bool IsExactSize) {
	if (capacity < new_capacity) { // If space is not enough
		// Delete old space
		delete[] p_data;
		// Update capacity
		capacity = new_capacity;
		// Create new space
		p_data = new char[capacity];
	}
}


void String::MoveData(String& dead_string) {
	// Delete old data if exist
	delete[] p_data;
	// copy values
	length = dead_string.length;
	capacity = dead_string.capacity;
	p_data = dead_string.p_data;
	// clearing other str
	dead_string.length = 0;
	dead_string.capacity = 0;
	dead_string.p_data = nullptr;
}

String& String::Copy(const String& string) {
	// Copy length
	length = string.length;
	// Request new memory
	NewMemory(string.capacity);
	// Copy buffers
	util::MemCpy(p_data, string.p_data, length);
	return *this;
}

String& String::CopyChars(const char* chrs) {
	// Calculate the size of str
	length = util::SizeUntilValue(chrs, '\0'); // Exclude terminating character
	// Create new capacity if it's not enough for this length
	NewMemory(length);
	// Copy char array to pointer
	util::MemCpy(p_data, chrs, length);
	return *this;
}

String& String::CopyChars(const char* chrs, uint32 length_) {
	// Update length
	length = length_;
	// Create new memory
	NewMemory(length_);
	// Copy char array to pointer
	util::MemCpy(p_data, chrs, length_);
	return *this;
}

String& String::EndString() {
	// if the space is not enough
	if (length + 1 > capacity) {
		IncreaseMemory(length + 1);
	}
	// Append terminal char and don't update the length
	p_data[length] = '\0';
	return *this;
}

String& String::Append(const char* str, const uint32 length_) {
	// if the space is not enough
	if (length_ + length > capacity) {
		IncreaseMemory(length_);
	}
	// copy new str to string
	util::MemCpy(&p_data[length], str, length_);
	// Increase the length
	length += length_;
	return *this;
}

String& String::Replace(const char* str, const uint32 length_) {
	// if the space is not enough
	if (length_ > capacity) {
		NewMemory(length_);
	}
	// copy new str to string
	util::MemCpy(p_data, str, length_);
	// Increase the length
	length = length_;
	return *this;
}

String& String::RemoveLast(uint32 value) {
	if (value < length)
		length -= value;
	else
		length = 0;
	return *this;
}

void String::Free() {
	delete[] p_data;
	p_data = nullptr;
	length = 0;
	capacity = 0;
}

const char* String::Chars() const {
	return p_data;
}

char* String::GetPointer() {
	return p_data;
}

uint32 String::Capacity() const {
	return capacity;
}
uint32 String::Length() const {
	return length;
}

String& String::SetLength(uint32 length_) {
	if (length_ > capacity) {// If the capacity is not enough
		length = capacity; // set length as the capacity
#if DEBUG_STRING
		printf("[DEBUG]:The length exceeds capacity!");
#endif
	}
	else
		length = length_; // set length
	return *this;
}

#pragma endregion

#pragma region Operators
String& String::operator=(const String& string) {
	return Copy(string);
}

String& String::operator=(const char* str) {
	return CopyChars(str);
}

String& String::operator+=(const char* str) {
	return Append(str, util::SizeUntilValue(str, '\0'));
}

String& String::operator+=(const char& character) {
	return Append(&character, 1);
}

String& String::operator+=(const String& string) {
	return Append(string.p_data, string.length);
}

String& String::operator--(int) {
	length--;
	return *this;
}

String& String::operator<<(String& old_str) {
	MoveData(old_str);
	return *this;
}

const char& String::operator[](uint32 index) const {
#if DEBUG_STRING
	if (index + 1 > capacity) { // Error index out of array
		printf("[ERROR]: Index out of array!\n");
		__debugbreak();
	}
#endif
	return p_data[index];
}

char& String::operator[](uint32 index) {
#if DEBUG_STRING
	if (index + 1 > capacity) { // Error index out of array
		printf("[ERROR]: Index out of array!\n");
		__debugbreak();
	}
#endif
	return p_data[index];
}


bool String::operator==(const char* str) const {
	uint32 str_size = util::SizeUntilValue(str, '\0');
	if (str_size != length)
		return false;

	return util::IsSameArray(p_data, str, str_size);
}

bool String::operator==(String& string) const {
	if (string.Length() != length)
		return false;

	return util::IsSameArray(p_data, string.Chars(), length);
}

#pragma endregion