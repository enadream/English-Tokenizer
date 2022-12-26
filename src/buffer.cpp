
// User defined libs
#include "buffer.hpp"
#include "misc/util.hpp"

Buffer::Buffer() {
	data = nullptr;
	capacity = 0;
}

Buffer::Buffer(uint64 size_byte) {
	Allocate(size_byte);
}

Buffer::~Buffer() {
	Release();
}

void Buffer::Allocate(uint64 size_byte) {
	Release();
	data = new uint8[size_byte];
	capacity = size_byte;
}

inline void Buffer::Release() {
	delete[] data; // Free buffer area
	data = nullptr;
	capacity = 0; // Update capacity
}

void Buffer::IncreaseCapacity(uint64 size_byte) {
	if (capacity > 0) { // If buffer already initialized and contains data
		// copy old pointer
		uint8* p_old_buffer = data;
		// create new heap area
		data = new uint8[capacity + size_byte];
		// copy values 
		util::MemCpy(data, p_old_buffer, capacity);
		// Increase new capacity
		capacity += size_byte;
		// free old heap area
		delete[] p_old_buffer;
	}
	else {
		data = new uint8[size_byte];
		capacity = size_byte;
	}
}