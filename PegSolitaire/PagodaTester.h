#pragma once


#include "Pagoda.h"
#include <thread>
#include <queue>
#include <mutex>



/*
	NOTES:
		-This class has two forms of concurrency within it, hence why there
		 seems to be a lot of mess...
			->Threaded test (type 1) uses a typical producer/consumer model,
			  and can be used to both generate and do tests, and also to load in
			  and consume tests. However, the boards must be loaded in beforehand for
			  this method.
			  This method was used to better reflect a real life scenario
			->Type 2 uses a queue of parameterless lamdas as a job queue.
			  This method was used to be a more authentic "job" model, successfully
			  generalised to any method that doesnt require parameters or parameters
			  are known at job insertion time. This method is more efficient, although
			  cannot be used for board pairs that have been loaded in from file.
		-Due to the limitations of C++11, performance of threading takes a large hit when 
		 both generating and looking up past valid pagodas as dual read and write access
		 cannot be governed effectively without the use of a sharable lock (C++14). If
		 only generating OR only looking up past valid pagodas efficiency is upwards of 90%,
		 with type 2 locking at 100%.
		-In hindsight it would have been good to have multithreaded file input, but time limitations
		 prevented this feature being implemented

*/

class PagodaTester
{
public:
	
	PagodaTester(int threads = 1, bool Eng = true);
	~PagodaTester(void);

	PagodaTester& operator=(const PagodaTester& rhs);

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

	friend std::ostream& operator<<(std::ostream& os, const PagodaTester& p);

	void setLoad(bool a){ loadPagodas = a; };
	void setSave(bool a){ savePagodas = a; };
	void setAccum(bool a){ accumPagodas = a; };
	void setGenerateNewPags(bool a){ 
		generateNewPags = a;
		p.setGenerationMode(a);
	};

private:

	void resetTester();
	void checkOFileExists(const std::string& filename);
	bool writeResultsToFile(const std::string& filename);

	bool loadPagodas, savePagodas, accumPagodas, generateNewPags;

	//C++ limitation... Definition in CPP
	static const std::string pagodaInput;
	static const std::string pagodaOutput;

	int threads;
	//1 over consProdRat is the number of producers to consumers
	static const int consProdRat = 4;
	Pagoda p;
	bool boardEng;

	//THREADING VERSION ONE
	std::vector<std::thread> threadArray;

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
	int insolvableProved;

	//How to keep track of how many pagodas have been verified
	std::mutex verifiedMut;
	int verifiedPagodas;

	//THREADING VERSION TWO
	void consumer(int batchSize, bool recordResults = false);
	void producer(int batchSize, int totalTests);

	//Another version of multithreading
	void produceAndConsume(int tests, bool recordResults = false);

	//Acquire job function
	void acquire_job();

	//Job list and mutex
	std::mutex jobMutex;
	std::queue<std::function<void()>> jobs;

	//flags for consumer and producer quit
	bool consumerQuit;
	bool producerQuit;

};

