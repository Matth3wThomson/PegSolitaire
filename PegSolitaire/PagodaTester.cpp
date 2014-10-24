#include "PagodaTester.h"


//Is having all my individual tests as a global vector a good idea, and having a number
// of threads producing a consuming tests, each
//thread collecting an amount from the global variable a good idea?

PagodaTester::PagodaTester(int t)
{
	p = Pagoda();

	int hardwareThreads = (signed int) std::thread::hardware_concurrency();

	if (t <= 1) threads = 1;
	else if (hardwareThreads < 1) threads = 1;
	else if (t > hardwareThreads) threads = hardwareThreads;
	else threads = t;

	consumerQuit = false;
	producerQuit = false;
	testsCreated = 0;
	testsCompleted = 0;
	insolvableProved = 0;
	verifiedPagodas = 0;

}


PagodaTester::~PagodaTester(void)
{
	
}

void PagodaTester::sequentialTest(int numberOfTests, const std::string& outputFile){

	resetTester();

	bool writeToFile = outputFile != "";

	//If a file name was supplied test whether it actually exists
	if (writeToFile){
		std::ofstream file = std::ofstream();
		file.open(outputFile);
		if (!file.is_open()) throw std::invalid_argument("Output file could not be found or created");
		else file.close();
	}

	//Start producing and doing tests
	for (int i=0; i<numberOfTests; ++i){
		Pagoda::BoardPair bp = Pagoda::BoardPair(p.create_random_board_pair(rand()));

		if (p.generate_pagoda(bp.pagoda, bp.endState)){
			verifiedPagodas++;
			bp.hasPagoda = true;

			if (p.prove_insolvable(bp)){
				insolvableProved++;
				if (writeToFile) results.push_back(bp);
			}
		}
		testsCreated++;
		testsCompleted++;

		//Update the console every so mamy tests
		if (testsCompleted % 10000 == 0) std::cout << "Tests completed: " << testsCompleted << std::endl;
	}

	//Output the results to console
	std::cout << *this << std::endl;

	//If we are writing to file do so
	if (writeToFile){
		std::cout << "Attempting to write to file: " << outputFile << std::endl;
		for (unsigned int i=0; i<results.size(); ++i)
			if (!p.print_to_file(outputFile, results[i], ((bool) i))){
				throw std::invalid_argument("Output file could not be found or created after tests. ");
			}
	}

};

void PagodaTester::verifyFile(const std::string& inputFilename, const std::string& outputFilename){

	resetTester();

	//Attempts to load from file
	if (!p.load_from_file(inputFilename, tests)) throw std::invalid_argument("Input file could not be found!");

	testsCreated = tests.size();

	bool writeToFile = outputFilename != "";

	//If you want to write to file it checks that it is there before commencing tests
	if (writeToFile){
		std::fstream file = std::fstream();
		file.open(outputFilename);

		if (!file.is_open()) throw std::invalid_argument("Output file could not be found or created!");
		else file.close();
	}

	//Test all loaded in board combinations
	while (!tests.empty()){

		if (tests.back().hasPagoda){


			if (!p.verify_pagoda(tests.back().pagoda)){
				results.push_back(tests.back());
				verifiedPagodas++;
			}

			if (!p.prove_insolvable(tests.back())){
				insolvableProved++;
			}
		}

		testsCompleted++;
		tests.pop_back();
		if (testsCompleted % 10000 == 0) std::cout << "Tests completed: " << testsCompleted << std::endl;

	}

	//Output results to console
	std::cout << "Number of tests loaded in: " << testsCreated << std::endl;
	std::cout << "Number of tests completed: " << testsCompleted << std::endl;
	std::cout << "Pagodas unable to verify: " << verifiedPagodas << std::endl;
	std::cout << "Pagodas unable to prove insolvability: " << insolvableProved << std::endl;

	if (writeToFile){
		std::cout << "Attempting to write to file: " << outputFilename << std::endl;
		for (int i=0; i<results.size(); ++i){
			if (!p.print_to_file(outputFilename, results[i], i)) throw std::invalid_argument("Output file could not be found or created after tests!");
		}
	}
}

void PagodaTester::sequentialTest(const std::string& inputFilename, const std::string& outputFilename){

	resetTester();

	//Attempts to load from file
	if (!p.load_from_file(inputFilename, tests)) throw std::invalid_argument("Input file could not be found!");

	bool writeToFile = outputFilename != "";

	testsCreated = tests.size();

	//If you want to write to file it checks that it is there before commencing tests
	if (writeToFile){
		std::fstream file = std::fstream();
		file.open(outputFilename);

		if (!file.is_open()) throw std::invalid_argument("Output file not found!");
		else file.close();
	}

	//Go through all of the tests and see which are solvable
	while (!tests.empty()){

		if (!tests.back().hasPagoda){
			tests.back().hasPagoda = p.generate_pagoda(tests.back().pagoda, tests.back().endState);
		}

		if (tests.back().hasPagoda){

			if (p.verify_pagoda(tests.back().pagoda)){
				verifiedPagodas++;

				if (p.prove_insolvable(tests.back())){
					insolvableProved++;
					if (writeToFile) results.push_back(tests.back());
				}
			}
		}

		testsCompleted++;
		tests.pop_back();
		if (testsCompleted % 10000 == 0) std::cout << "Tests completed: " << testsCompleted << std::endl;

	}

	//Write to console the results of testing
	std::cout << *this << std::endl;

	if (writeToFile){
		std::cout << "Attempting to write to file: " << outputFilename << std::endl;
		for (int i=0; i<results.size(); ++i){
			if (!p.print_to_file(outputFilename, results[i], i)) throw std::invalid_argument("Output file could not be found or created after tests");
		}
	}


};

void PagodaTester::Threadedtest(int numberOfTests, int batchSize, const std::string& outputFile){

	resetTester();

	bool writeToFile = outputFile != "";

	//If you want to write to file it checks that it is there before commencing tests
	if (writeToFile){
		std::fstream file = std::fstream();
		file.open(outputFile);

		if (!file.is_open()) throw std::invalid_argument("Output file not found!");
		else file.close();
	}

	//Allocate as much space as necessary
	if (numberOfTests > maxSize) tests.reserve(maxSize);
	else tests.reserve(numberOfTests);

	//If threaded
	if (threads != 1){

		//Create threads based on the ratio
		for (int i=1; i<threads; ++i){
			if (i<threads/consProdRat)
				threadArray.push_back(std::thread(&PagodaTester::producer, this, batchSize, numberOfTests));
			else
				threadArray.push_back(std::thread(&PagodaTester::consumer, this, batchSize, writeToFile));
		}

		//Update console
		std::cout << "Threads started: " << threads-1 << std::endl;

		//Start producing results
		producer(100, numberOfTests);

		std::cout << "Producing finished. Created: " << testsCreated << std::endl;

		/*for (int i=1; i<threads/consProdRat; ++i){
			threadArray.push_back(std::thread(&PagodaTester::consumer, this, batchSize, writeToFile));
		}*/

		//Check to see if consumers have finished yet
		while (!consumerQuit){
			if (testsCreated <= testsCompleted){
				consumerQuit = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(500)); }

			//else{
			//	//Keep console up to date with progress
			//	std::cout << "Tests completed: " << testsCompleted << std::endl;
			//	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			//}
		}

		//Join with all threads created
		for (unsigned int i=0; i<threadArray.size(); ++i){
			if (threadArray[i].joinable()) threadArray[i].join();
		}

		std::cout << *this << std::endl;

		if (writeToFile){
			std::cout << "Attempting to write to file: " << outputFile << std::endl;
			for (int i=0; i<results.size(); ++i){
				if (!p.print_to_file(outputFile, results[i], i)) throw std::invalid_argument("Output file could not be found or created after tests");
			}
		}

	} else {
		sequentialTest(numberOfTests, outputFile);
	}

}

void PagodaTester::Threadedtest(const std::string& inputFilename, int batchSize, const std::string& outputFilename){

	resetTester();

	if (!p.load_from_file(inputFilename, tests)) throw std::invalid_argument("Input file could not be found!");

	bool writeToFile = outputFilename != "";

	testsCreated = tests.size();

	//If you want to write to file it checks that it is there before commencing tests
	if (writeToFile){
		std::fstream file = std::fstream();
		file.open(outputFilename);

		if (!file.is_open()) throw std::invalid_argument("Output file not found!");
		else file.close();
	}

	//Create all of the threads and set them consuming all loaded in tests
	for (int i=0; i<threads; ++i){
		threadArray.push_back(std::thread(&PagodaTester::consumer, this, batchSize, writeToFile));
	}

	//While the consumers are still going
	while (!consumerQuit){
		if (testsCreated <= testsCompleted) consumerQuit = true;
		else{
			//Keep console up to date with progress
			std::cout << "Tests completed: " << testsCompleted << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		}
	}

	//Join with all threads created
	for (unsigned int i=0; i<threadArray.size(); ++i){
		if (threadArray[i].joinable()) threadArray[i].join();
	}

	std::cout << *this << std::endl;

	if (writeToFile){
		std::cout << "Attempting to write to file: " << outputFilename << std::endl;
		for (int i=0; i<results.size(); ++i){
			if (!p.print_to_file(outputFilename, results[i], i)) throw std::invalid_argument("Output file could not be found or created after tests");
		}
	}
}


//THIS WORKS OMFG
void PagodaTester::ThreadedtestType2(int numberOfTests, int batchSize, const std::string& outputFile){

	if (threads == 1) sequentialTest(numberOfTests, outputFile);
	else {

		resetTester();

		//TODO: Make this try to open the file first
		bool recordResults = outputFile != "";

		//Check if the file is actually valid
		if (recordResults){
			std::ifstream file = std::ifstream();
			file.open(outputFile);
			if (!file.is_open()) throw std::invalid_argument("Output file entered was not found");
		}

		////Start all threads
		for (int i=0; i<threads; ++i){
			threadArray.push_back(std::thread(&PagodaTester::acquire_job, this));
			std::cout << "Started thread " << i << std::endl;
		}

		//Push all of the jobs onto the queue
		while (numberOfTests > batchSize){
			//Jobs is a queue of functions with no parameters
			jobMutex.lock();
			jobs.push( [this, batchSize, recordResults] { this->produceAndConsume(batchSize, recordResults); });
			jobMutex.unlock();
			numberOfTests -= batchSize;
		}

		jobMutex.lock();
		jobs.push( [this, batchSize, recordResults] { this->produceAndConsume(batchSize, recordResults);});
		jobMutex.unlock();

		std::cout << "Pushed all jobs onto job list" << std::endl;

		//Only start one thread
		/*threadArray.push_back(std::thread(&PagodaTester::acquire_job, this));*/

		//While jobs remain sleep the main thread
		while (!jobs.empty()){
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			/*acquire_job();*/
		}

		//Stop threads when finished.
		consumerQuit = true;

		for (int i=0; i<threadArray.size(); ++i)
			if (threadArray[i].joinable()) threadArray[i].join();

		while (!threadArray.empty()){
			threadArray.pop_back();
		}

		/*std::this_thread::sleep_for(std::chrono::milliseconds(1000));*/

		//Print the results to file
		for (int i=0; i<results.size(); ++i){
			p.print_to_file(outputFile, results[i], i);
		}

		std::cout << *this << std::endl;
	}

}

void PagodaTester::ThreadedtestType3(int numberOfTests, const std::string& outputFile){

	resetTester();

	int batchSize = numberOfTests / threads;

	bool writeToFile = outputFile != "";

	if (writeToFile){
		std::ofstream file = std::ofstream();
		file.open(outputFile);
		if (!file.is_open()) throw std::invalid_argument("Output file was not found. ");
	}

	for (int i=0; i<threads; ++i){
		threadArray.push_back(std::thread(&PagodaTester::produceAndConsume, this, batchSize, writeToFile));
		std::cout << "Thread " << i << " started. " << std::endl;
	}

	//Join with all threads created
	for (unsigned int i=0; i<threadArray.size(); ++i){
		if (threadArray[i].joinable()) threadArray[i].join();
	}

	std::cout << "Threads finished" << std::endl;
	std::cout << *this << std::endl;


	if (writeToFile){
		std::cout << "Attempting to write to file: " << outputFile << std::endl;

		for (int i=0; i<results.size(); ++i){
			if (!p.print_to_file(outputFile, results[i], i)) throw std::invalid_argument("Writing to file failed");
		}
	}
}



//TODO: if you find a result, print to file if requested
void PagodaTester::consumer(int batchSize, bool recordResults){

	std::vector<Pagoda::BoardPair> internalTests = std::vector<Pagoda::BoardPair>();

	while (!consumerQuit){

		//Try to acquire jobs
		if (testsMutex.try_lock()){

			//Collect all jobs possible until you reach your batch size
			//or there are no jobs left...
			while (!tests.empty() && internalTests.size()<batchSize){
				internalTests.push_back(tests.back());
				tests.pop_back();
			}

			testsMutex.unlock();
		}

		int internalTestsComplete = 0;

		//While you have jobs to do, do them
		while (!internalTests.empty()){

			//If it hasnt got a pagoda generate one
			if (!internalTests.back().hasPagoda)
				internalTests.back().hasPagoda = p.generate_pagoda(internalTests.back().pagoda, internalTests.back().endState);

			//Try and use the generated pagoda
			if (internalTests.back().hasPagoda){
				if (p.verify_pagoda(internalTests.back().pagoda)){

					//Let the program know you have found a valid pagoda
					verifiedMut.lock();
					verifiedPagodas++;
					verifiedMut.unlock();

					if (p.prove_insolvable(internalTests.back())){

						//Let the program know you have found a problem that can be proved insolvable
						insolvableMut.lock();
						insolvableProved++;
						if (recordResults) results.push_back(internalTests.back());
						insolvableMut.unlock();

					}
				}
			}

			//Keep track of all tests you do internally
			internalTests.pop_back();
			internalTestsComplete++;

		}

		//If you completed any tasks this run, let the program know
		if (internalTestsComplete != 0){
			tCompMut.lock();
			testsCompleted += internalTestsComplete;
			tCompMut.unlock();
		}
	}

}


//Creates random tests
void PagodaTester::producer(int batchSize, int totalTests){

	std::vector<Pagoda::BoardPair> internalTests = std::vector<Pagoda::BoardPair>();

	//While tests still need to be produced
	while (!producerQuit){

		//Dont create any tests this loop unless instructed
		int thisBatch = 0;

		//If you can allocate yourself a job
		if (tCreateMut.try_lock()){

			//If there are less tests left than one whole batch of yours
			if ((totalTests - testsCreated) < batchSize){

				//Adjust your batch size to the number of tests left,
				//and left other threads know that producing is done
				thisBatch = totalTests - testsCreated;
				testsCreated = totalTests;
				producerQuit = true;

			} else testsCreated += thisBatch = batchSize; //Allocate some tests to create, and let everyone know these have been created

			//Unlock it now you have allocated yourself a job
			tCreateMut.unlock();

			//While you have a tests to create in this batch, keep creating them
			while (internalTests.size() < thisBatch){
				internalTests.push_back(p.create_random_board_pair(rand()));
			}

			//Need to check if the vector isnt over max size before trying to allocate
			while (tests.size() > maxSize - internalTests.size()){
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}


			//Acquire the lock to put the tests you have created into
			//the job list
			testsMutex.lock();
			while (!internalTests.empty()){
				tests.push_back(internalTests.back());
				internalTests.pop_back();
			}
			testsMutex.unlock();
		}

	}
}

void PagodaTester::produceAndConsume(int tests, bool recordResults){

	//There is a big bug here!! 
	Pagoda::BoardPair bp(33);

	//TODO: to improve performance of type 3, store a local variable and count successful
	// pagoda verifications to prevent lock contention

	//Do the allocated number of tests
	while (tests > 0){

		//Create a random board pair
		/*Pagoda::BoardPair*/ bp = p.create_random_board_pair(rand());	

		//Attempt pagoda generation
		if (p.generate_pagoda(bp.pagoda, bp.endState)){

			verifiedMut.lock();
			verifiedPagodas++;
			verifiedMut.unlock();

			bp.hasPagoda = true;
		};

		if (bp.hasPagoda){

			//TODO: Then try to prove the board pair is insolvable
			/*if (p.prove_insolv_with_saved(bp)){*/
			if (p.prove_insolvable(bp)){

				insolvableMut.lock();
				insolvableProved++;
				if (recordResults) results.push_back(bp);
				insolvableMut.unlock();

			};
		}
		tests--;
	}


}

void PagodaTester::acquire_job(){

	while (!consumerQuit){

		std::function<void()> func = std::function<void()>();

		//Try to acquire a job from the job queue
		jobMutex.lock();
		if (!jobs.empty()){
			func = jobs.front();
			jobs.pop();
		}
		jobMutex.unlock();

		//Execute the job
		if (func)
			func();

	}

}

std::ostream& operator<<(std::ostream& os, const PagodaTester& p){
	os << "Number of tests created: " << p.testsCreated << std::endl;
	os << "Number of tests completed: " << p.testsCompleted << std::endl;
	os << "Verified Pagodas: " << p.verifiedPagodas << std::endl;
	os << "Insolvable found: " << p.insolvableProved;
	return os;
}

void PagodaTester::resetTester(){
	p = Pagoda();

	while (!tests.empty()){
		tests.pop_back();
	}

	while (!results.empty()){
		results.pop_back();
	}

	while (!jobs.empty()){
		jobs.pop();
	}

	int testsCreated = 0;
	int testsCompleted = 0;
	insolvableProved = 0;
	verifiedPagodas = 0;
	consumerQuit = false;
	producerQuit = false;
}