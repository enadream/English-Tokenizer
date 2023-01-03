#include <fstream>

// User defined libs
#include "file_system.hpp"
#include "log.hpp"

int8 FileSystem::ReadFromDisk(Buffer& buffer, const char* file_dir) {
	std::ifstream file(file_dir, std::ios::in | std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		Log::Error("No such file found with the name \"") << file_dir << "\"\n";
		return -1;
	}

	std::streampos end = file.tellg();
	uint64 size = end - file.seekg(0, std::ios::beg).tellg();

	if (size > buffer.capacity) { // If there is not enough space for reading
		buffer.Allocate(size + 1);
	}

	file.read(buffer.As<char>(), size); // Get data to buffer
	file.close();

	buffer.As<char>()[buffer.capacity - 1] = '\0'; // Adding ending char to the end

	Log::Info("The file has readed successfully.\n");
	return 0;
}

int8 FileSystem::WriteToDisk(const char* data, const char* file_dir, const uint64& size) {
	std::ofstream file(file_dir, std::ios::out);

	if (!file.is_open()) {
		Log::Error("An error occurred while opening the file \"") << file_dir << "\"\n";
		return -1;
	}

	file.write(data, size);
	file.close();

	Log::Info("The file has written successfully.\n");
	return 0;
}