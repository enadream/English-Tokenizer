/*
*	Eren ALPAR enadream.com
*   APP VERSION 0.3
*	C++ VERSION 17
*/

#include <iostream>
#include <thread>
#include <windows.h>

// User defined headers
#include "ecli.hpp"

int main() {
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return GetLastError();
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return GetLastError();
	}

	Ecli program;
	program.Start();
}












/*
verb::VerbHandler Verbs;
	String output;

	Verbs.AddNewVerb("undergo /S0", 11);
	Verbs.AddNewVerb("undsend", 7);
	Verbs.AddNewVerb("undsend", 7);
	Verbs.AddNewVerb("X-RAY", 5);

	Verbs.AddNewVerb("X-mom", 5);
	Verbs.AddNewVerb("X-mom /U0", 9);
	Verbs.AddNewVerb("X-mom", 5);

	Verbs.MultipleVerbAdder("# A\n\
		arise, arose, arisen\n\
		awake, (awakened, awoke), (awakened, awoken)\n\
		# B\n\
		backslide, backslid, (backslidden, backslid)\n\
		be, (was, were), been\n\
		bear, bore, (born, borne)", 179);

	Verbs.AddNewVerb("be, 	(was, were), been", 22);
	Verbs.AddNewVerb("burn /S0,	 (burned,	burnt), (burned, burnt) ", 44);
	Verbs.AddNewVerb("blow,	blew,	blown", 17);
	Verbs.AddNewVerb("blow,	blew,	blown", 17);

	Verbs.GetAllIrregularVerbs(output);
	Verbs.GetVerbsWithIndex("un", output);
	std::cout << output.EndString().Chars();

*/

/*
// Get read txt file
	IOOperation::ReadFromDisk(*inputText, *"input.txt");
	//std::cout << inputText;
	timer.StartTime();
	machine.ParseClause(*inputText);
	timer.EndTime();
	Log::Info("Tokenize time: ") << timer.GetDuration(*"ns") << " nanoseconds.\n";

	// Translate data to readable format i.e
	timer.StartTime();
	machine.GetAllSentences(*outputText);
	timer.EndTime();
	Log::Info("Human readable translation time: ") << timer.GetDuration(*"ns") << " nanoseconds.\n";

	// Write to txt file
	IOOperation::WriteToDisk(outputText);
	// Print to console
	std::cout << "\n" << outputText << "\n";

*/