#ifndef IO_OPERATION_HPP
#define IO_OPERATION_HPP

#include <string>
#include <iostream>

// User defined libs
#include "datatypes.hpp"

class IOOperation {
private:
	char* io_buffer; // 8 byte
	uint32 buffer_capacity; // 4 byte

	inline void DeleteBuffer();
	void IncreaseCapacity();
	void ParseParameters(const char& line, char& out_params, const char param_char = '-') const;
	bool DoesContainParameter(const char& param, const char& parameters, char& out_param_value) const;
public:
	IOOperation();
	~IOOperation();

	int8 ReadFromDisk(const char& file_name);
	void WriteToDisk(const char& in_file) const;

	void StartCLI(); // Command Line Interface
};

#endif 
