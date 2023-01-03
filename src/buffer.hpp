#ifndef BUFFER_HPP
#define BUFFER_HPP

// User defined libs
#include "misc/data_types.hpp"

#define DEFAULT_INCREASE_AMOUNT 1000 // in byte 

class Buffer {
public: // Variables
	uint8* data; // 8 byte
	uint64 capacity; // 8 byte

public: // Functions
	Buffer();
	Buffer(const Buffer&) = default;

	explicit Buffer(uint64 size_byte);
	~Buffer();

	void Allocate(uint64 size_byte);
	void Release();
	void IncreaseCapacity(uint64 size_byte = DEFAULT_INCREASE_AMOUNT);

	template <typename Type>
	Type* As() {
		return (Type*)data;
	}
};

#endif // !BUFFER_HPP
