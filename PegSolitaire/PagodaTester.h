#pragma once

#include "Pagoda.h"
#include <thread>
#include <mutex>

class PagodaTester
{
public:
	PagodaTester(int threads = 1);
	~PagodaTester(void);

	//Randomly generates boards and tests them
	void test(int numberOfTests, const std::string& outputFile);

	//Takes board configurations from a file and outputs and discrepancies
	void test(const std::string& inputFilename, const std::string& outputFilename = "");


private:

	

	int threads;
	Pagoda p;

	//List of boards to be tested.
	//Shared mutex is a C++14 thing! :/
	std::mutex jobMutex;
	std::vector<Pagoda::BoardPair> tests;

	//How to keep track of how many tests need to be created
	std::mutex tCreateMut;
	int testsCreated;

	//How to keep track of how many tests have been completed
	std::mutex tCompMut;
	int testsCompleted;

	//List of boards to be output to results file
	std::mutex insolvableMut;
	std::vector<Pagoda::BoardPair> results;
	std::string outputFile;
	int insolvableProved;

	std::mutex verifiedMut;
	int verifiedPagodas;


	std::vector<std::thread> threadArray;
	
	

	//The function that threads should execute to "do a test"
	void consumer(int batchSize, bool recordResults = false);
	void producer(int batchSize, int totalTests);

	bool consumerQuit;
	bool producerQuit;

};

