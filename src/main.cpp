/*
*	Eren ALPAR enadream.com
*   APP VERSION 0.3
*	C++ VERSION 17
*/

#include <iostream>
#include <thread>
#include <windows.h>

// User defined headers
#include "iooperation.hpp"
#include "verb.hpp"


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

	IOOperation program;
	program.StartCLI();

	//std::cout << "Size: " << sizeof(verb::Verb);

	//Tokenizer machine;
	//// Input char area
	//char inputText[2500];
	//// Output char area
	//char outputText[3000];
	////inputText[10] = '\0';

	//char console_input[150];
	//int16 value;
	//Clock timer;

	//Log::Warning("Hello this is an warning\n") << "I am new line\n";

	/*verb::VerbHandler Verbs;
	char outputText[3000];

	Verbs.AddNewVerb(*"");
	Verbs.AddNewVerb(*"undergo /S0");
	Verbs.AddNewVerb(*"undsend");
	Verbs.AddNewVerb(*"undsend");
	Verbs.AddNewVerb(*"aAsdae");
	Verbs.AddNewVerb(*"X-RAY");
	Verbs.AddNewVerb(*"X-QUERY");
	Verbs.AddNewVerb(*"x-die");
	Verbs.AddNewVerb(*"aa");
	Verbs.AddNewVerb(*"aaasd");

	Verbs.AddNewVerb(*"be, 	(was, were), been");
	Verbs.AddNewVerb(*"burn /S0,	 (burned,	burnt), (burned, burnt) ");
	Verbs.AddNewVerb(*"draw ,	drew,	drawn");
	Verbs.AddNewVerb(*"draw");
	Verbs.AddNewVerb(*"blow,	blew,	blown");
	Verbs.AddNewVerb(*"blow,	blew,	blown");
	Verbs.AddNewVerb(*"blow,	blew,	blown");

	Verbs.GetAllIrregularVerbs(*outputText);*/
	//Verbs.GetVerbsWithIndex(*"x-", *outputText);
	/*std::cout << outputText;*/
}


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


/*
*
*
* 	//Log::Warning(*"The symbol in text file couldn't been readed.");
	//Log::Error(*"The program has halt!");
	//Log::Info(*"You have successly loaded the data.");
*
* class Test
{
public:
	Test(int a) : m_a(a) {
		std::cout << "constructor called !\n";
	}
	int operator+(int value) {
		std::cout << "Sum operator called !\n";
		return m_a + value;
	}
private:
	int m_a;

};

bool control = true;


void thread_1() {
	std::cout << "Loading: [";

	using namespace std::literals::chrono_literals;

	for (int i = 0; i < 10; i++) {
		std::cout << "#";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	std::cout << "]";
}

void thread_(int num) {
	while (control) {
		std::cout << "I am thread " << num << ". " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

constexpr float exp(float x, int n)
{
	return n == 0 ? 1 :
		n % 2 == 0 ? exp(x * x, n / 2) :
		exp(x * x, (n - 1) / 2) * x;
}
*
	int a;

	a = 4;

	std::thread myWorker_1(thread_, 1);
	std::thread myWorker_2(thread_, 2);
	std::thread myWorker_3(thread_, 3);

	std::cin.get();
	control = false;


	myWorker_1.detach();
	myWorker_2.detach();
	myWorker_3.detach();


	std::cout << "I am thread 0. " << std::this_thread::get_id() << std::endl;
	std::cin.get();

*/