#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

// User defined libs
#include "misc/data_types.hpp"
#include "buffer.hpp"

class FileSystem {
public:
	static int8 ReadFromDisk(Buffer& buffer, const char* file_dir);
	static int8 WriteToDisk(const char* data, const char* file_dir, const uint64& size);
};

#endif // !FILE_SYSTEM_HPP
