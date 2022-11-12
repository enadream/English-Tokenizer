/* #include <iostream>
#include <vector>
#include <chrono>

using namespace std::chrono;
using namespace std;

#define aa 50000000

int main()
{
	vector<int> v(aa, 42);
	vector<int> v1(aa, 42);
	vector<int> v2(aa, 42);

	auto start = high_resolution_clock::now();
	//  Old-fashioned loop.
	for (size_t i = 0; i < v.size(); i++)
	{
		v[i] *= v[i];
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	cout << "Old-fashioned loop: " << duration.count() << " milliseconds.\n";

	start = high_resolution_clock::now();
	//  Range-based loop.
	for (int& val : v1)
	{
		val *= val;
	}
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);

	cout << "Range-based loop: " << duration.count() << " milliseconds.\n";

	start = high_resolution_clock::now();
	for (auto val = v2.begin(); val != v2.end(); ++val) {

		*val *= *val;
	}
	stop = high_resolution_clock::now();

	duration = duration_cast<milliseconds>(stop - start);
	cout << "3rd loop: " << duration.count() << " milliseconds.\n";

	{
		int* arr = new int[aa];
		for (int index = 0; index < aa; index++) {
			arr[index] = 42;
		}

		auto start = high_resolution_clock::now();
		for (int index = 0; index < aa; index++) {

			arr[index] *= arr[index];
		}
		auto stop = high_resolution_clock::now();

		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "ARRAY loop: " << duration.count() << " milliseconds.\n";
		delete arr;
	}

	system("PAUSE");
}

/*
void speedTest() {
	char arr_1[1000];
	char arr_2[1000];
	char arr_3[1000];

	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
	std::chrono::nanoseconds duration;

	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++) {
		arr_1[i] = i % 100;
	}
	endTime = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	std::cout << "Define time: " << duration.count() << " nanoseconds.\n";



	// memcpy
	startTime = std::chrono::high_resolution_clock::now();
	memcpy(arr_2, arr_1, sizeof(char) * 1000);
	endTime = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
	std::cout << "Memcpy time: " << duration.count() << " nanoseconds.\n";


	// my loop
	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++)
	{
		arr_3[i] = arr_2[i];
	}
	endTime = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
	std::cout << "My loop time: " << duration.count() << " nanoseconds.\n";

	std::cout << "value time: " << (int)arr_1[555] << " nanoseconds.\n";
	std::cout << "value time: " << (int)arr_2[555] << " nanoseconds.\n";
}*/

