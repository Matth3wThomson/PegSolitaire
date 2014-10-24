#pragma once

#include "Pagoda.h"
#include <thread>
#include <queue>
#include <mutex>


//TODO: Explain why there seems to be such a mess of crap in this class...

//TODO: Fix the new cool way of doing threads. NOT FINDING SOLUTIONS FOR SOME REASON.
//Also much bugs with memory leaks etc... I must be doing something wrong with lambdas?

//TODO: Explain in comments why I havent used throw on function signatures

//TODO: Fix pagoda so that it uses saved pagodas to try to solve

class PagodaTester
{
public:
	PagodaTester(int threads = 1);
	~PagodaTester(void);

	//Performs tests sequentially on randomly generated boards
	void sequentialTest(int numberOfTests, const std::string& outputFile = "");

	//Performs tests sequentially on boards read from a file
	void sequentialTest(const std::string& inputFilename, const std::string& outputFilename = "");

	//Reads boards from a file and outputs to another file boards that are not verified pagodas
	void verifyFile(const std::string& inputFilename, const std::string& outputFilename = "");

	//Randomly generates boards and tests them on all cores
	void Threadedtest(int numberOfTests, int batchSize = 100, const std::string& outputFile = "");

	//Performs tests across all cores from boards read from file
	void Threadedtest(const std::string& inputFilename, int batchSize, const std::string& outputFilename = "");

	//Randomly generates boards and tests them on all cores using a different threading system
	void ThreadedtestType2(int numberOfTests, int batchSize = 100, const std::string& outputFile = "");

	void ThreadedtestType3(int numberOfTests, const std::string& outputFile = "");

	

	friend std::ostream& operator<<(std::ostream& os, const PagodaTester& p);
private:

	void resetTester();

	int threads;
	//1 over consProdRat is the number of producers to consumers
	static const int consProdRat = 4;
	Pagoda p;

	//List of boards to be tested.
	//Shared mutex is a C++14 thing! :/
	std::mutex testsMutex;
	std::vector<Pagoda::BoardPair> tests;
	static const unsigned int maxSize = 1000000;

	//How to keep track of how many tests need to be created
	std::mutex tCreateMut;
	int testsCreated;

	//How to keep track of how many tests have been completed
	std::mutex tCompMut;
	int testsCompleted;

	//List of boards to be output to results file
	std::mutex insolvableMut;
	std::vector<Pagoda::BoardPair> results;
	/*std::string outputFile;*/ //TODO: Work out if this is even necessary
	int insolvableProved;

	std::mutex verifiedMut;
	int verifiedPagodas;

	std::vector<std::thread> threadArray;

	//The function that threads should execute to "do a test"
	void consumer(int batchSize, bool recordResults = false);
	void producer(int batchSize, int totalTests);

	//Another version of multithreading
	void produceAndConsume(int tests, bool recordResults = false);

	//Acquire job function
	void acquire_job();

	//TODO: If time, make this into function pointers...
	// http://stackoverflow.com/questions/4880308/is-it-possible-to-save-a-function-pointer-into-an-object

	//http://stackoverflow.com/questions/22109555/c-queue-of-functions

	std::mutex jobMutex;
	std::queue<std::function<void()>> jobs;

	bool consumerQuit;
	bool producerQuit;

};

