#include <iostream>
#include <fstream>
#include <string>

typedef unsigned long long ull;

char IsPunctuation(const char& one_char);
bool IsChar(const char& letter);
#define FILE_SIZE 550000


void tokenize(const char* file_in, const ull& size, char*& file_out, ull& out_size) {
	bool line_empty = true;
	file_out = new char[size * 1.5];
	bool save = true;
	ull lastWordEndIndex = 0;

	for (ull i = 0; i < size; i++) { // Parse sentences into tokens
		if (file_in[i] != ' ' && file_in[i] != '\n' && file_in[i] != '\t' && file_in[i] != '\r' && file_in[i] != '\0') {
			if (save) {
				if (IsPunctuation(file_in[i])) {
					if (!line_empty)
						file_out[out_size++] = '\n';
					file_out[out_size++] = file_in[i];
					file_out[out_size++] = '\n';
					lastWordEndIndex = out_size;
					line_empty = true;
				}
				else if (IsChar(file_in[i])) {
					file_out[out_size++] = file_in[i];
					line_empty = false;
				}
				else {
					if (!line_empty) {
						file_out[out_size++] = '\n';
						lastWordEndIndex = out_size;
					}
					out_size = lastWordEndIndex;
					line_empty = true;
					save = false;
				}
			}
		}
		else {
			if (!save)
				save = true;
			if (!line_empty) {
				file_out[out_size++] = '\n';
				lastWordEndIndex = out_size;
			}
			line_empty = true;
		}
	}
}

int main() {
	std::string file_name;
	char* buffer;
	char* buffer_out;
	ull size;
	ull out_size = 0;

	// Read file
	std::cin >> file_name;
	std::ifstream file(file_name, std::ios::in | std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		std::cout << "No file with that name.\n";
		return -1;
	}

	// Take data into buffer
	//size = file.tellg();
	size = FILE_SIZE;
	buffer = new char[FILE_SIZE];
	file.seekg(0, std::ios::beg);
	file.read(buffer, FILE_SIZE);
	file.close();

	// Make operation
	tokenize(buffer, size, buffer_out, out_size);

	// Write file
	file_name = "test.txt";
	std::ofstream out_file(file_name, std::ios::out);
	out_file.write(buffer_out, out_size);
	out_file.close();

	delete[] buffer;
	delete[] buffer_out;

	std::cout << "Operation Done!\n";
	return 0;
}


char IsPunctuation(const char& one_char) {
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
	case  '.': // Full stop
		return 10;
	case  '?': // Question mark
		return 11;
	case  '!': // Exclamation point
		return 12;
	case  '%':		// Slash
		return 13;
	default:
		return 0;
	}
}

bool IsChar(const char& letter) { // returns the character if it's alpha otherwise returns 0
	if (letter > 64 && letter < 91) { // Capital letter
		return true;
	}
	else if (letter > 96 && letter < 123) { // Char already lowercase
		return true;
	}
	else if (letter == '\'') {
		return true;
	}
	else if (letter == '-') {
		return true;
	}
	else { // Character is not alphabetic
		return false;
	}
}