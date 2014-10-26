#include "PagodaTester.h"
#include "PerformanceCounter.h"


//Where input and output of known pagodas will be stored
const std::string PagodaTester::pagodaInput = "knownPagodas.txt";
const std::string PagodaTester::pagodaOutput = "knownPagodasFound.txt";

//Number of threads and board type (Eng/eur)
PagodaTester::PagodaTester(int t, bool eng)
{
	p = Pagoda(eng);
	boardEng = eng;

	int hardwareThreads = (signed int) std::thread::hardware_concurrency();

	if (t <= 1) threads = 1;
	else if (hardwareThreads < 1) threads = 1;
	else if (t > hardwareThreads) threads = hardwareThreads;
	else threads = t;

	consumerQuit = false;
	producerQuit = false;

	loadPagodas = false;
	savePagodas = false;
	accumPagodas = false;
	generateNewPags = false;

	testsCreated = 0;
	testsCompleted = 0;
	insolvableProved = 0;
	verifiedPagodas = 0;

}


PagodaTester::~PagodaTester(void)
{

}

//Necessary due to the use of std::mutex
PagodaTester& PagodaTester::operator=(const PagodaTester& rhs){

	this->p = rhs.p;
	this->boardEng = rhs.boardEng;

	this->threads = rhs.threads;

	this->consumerQuit = rhs.consumerQuit;
	this->producerQuit = rhs.producerQuit;

	this->loadPagodas = rhs.loadPagodas;
	this->savePagodas = rhs.savePagodas;
	this->accumPagodas = rhs.accumPagodas;
	this->generateNewPags = rhs.generateNewPags;

	testsCreated = rhs.testsCreated;
	testsCompleted = rhs.testsCompleted;
	insolvableProved = rhs.insolvableProved;
	verifiedPagodas = rhs.verifiedPagodas;

	return *this;
}

//This method performs a a given number of tests and outputs its results to outputfile (if supplied)
//sequentially. Results are timed and output to console upon completion
void PagodaTester::sequentialTest(int numberOfTests, const std::string& outputFile){

	resetTester();

	bool writeToFile = outputFile != "";

	//If a file name was supplied test whether it actually exists
	if (writeToFile){
		checkOFileExists(outputFile);
	}

	StartCounter();

	//Start producing and doing tests
	for (int i=0; i<numberOfTests; ++i){
		Pagoda::BoardPair bp = Pagoda::BoardPair(p.createRandomBoardPair(rand()));

		if (p.generatePagoda(bp.pagoda, bp.endState, accumPagodas)){
			verifiedPagodas++;
			bp.hasPagoda = true;
		}

		if (p.proveInsolvWithSaved(bp)){
			insolvableProved++;
			if (writeToFile) results.push_back(bp);
		}

		testsCreated++;
		testsCompleted++;

		//Update the console every so many tests
		if (testsCompleted % 10000 == 0) std::cout << "Tests completed: " << testsCompleted << std::endl;
	}

	//Output the results to console
	std::cout << std::endl << "Time taken (ms): " << GetCounter() << std::endl;
	std::cout << *this << std::endl << std::endl;


	//If we are writing to file do so
	if (writeToFile){
		if (!writeResultsToFile(outputFile)) std::cout << "Could not write results to file. " << std::endl;
	}

	//Save our pagodas if requested
	if (savePagodas){
		if (!p.savePagodaFunctions(pagodaOutput)) std::cout << "Failed to save known pagodas to file" << std::endl;
	}

};

//This function takes an input file and an optional output file, and counts the number of
//invalid pagodas and pagodas that do not prove insolvability. As requested in spec (2nd part, Q5)
void PagodaTester::verifyFile(const std::string& inputFilename, const std::string& outputFilename){

	resetTester();

	std::cout << "Loading in from file: '" << inputFilename << "'. " << std::endl;

	//Attempts to load from file
	p.loadFromFile(inputFilename, tests);

	testsCreated = tests.size();

	bool writeToFile = outputFilename != "";

	//If you want to write to file it checks that it is there before commencing tests
	if (writeToFile){
		checkOFileExists(outputFilename);
	}

	std::cout << "Beginning " << testsCreated << " tests. " << std::endl << std::endl;

	StartCounter();

	//Test all loaded in board combinations
	while (!tests.empty()){

		if (tests.back().hasPagoda){


			if (!p.verifyPagoda(tests.back().pagoda)){
				results.push_back(tests.back());
				verifiedPagodas++;
			}

			if (!p.proveInsolvable(tests.back())){
				insolvableProved++;
			}
		}

		testsCompleted++;
		tests.pop_back();
		if (testsCompleted % 10000 == 0) std::cout << "Tests completed: " << testsCompleted << std::endl;

	}

	//Output results to console
	std::cout << std::endl << "Time taken (ms): " << GetCounter() << std::endl;
	std::cout << "Number of tests loaded in: " << testsCreated << std::endl;
	std::cout << "Number of tests completed: " << testsCompleted << std::endl;
	std::cout << "Pagodas unable to verify: " << verifiedPagodas << std::endl;
	std::cout << "Pagodas unable to prove insolvability: " << insolvableProved << std::endl << std::endl;

	if (writeToFile){
		std::cout << "Writing failures to file: " << outputFilename << std::endl;
		for (unsigned int i=0; i<results.size(); ++i){
			if (!p.printToFile(outputFilename, results[i], i)) throw std::invalid_argument("Output file could not be found or created after tests!");
		}
	}
}

//This method reads in files from a supplied string and outputs results to an optional output
//file if supplied. Tests are performed sequentially and results are output to console
//upon completion
void PagodaTester::sequentialTest(const std::string& inputFilename, const std::string& outputFilename){

	resetTester();

	//Attempts to load from file
	std::cout << "Loading in tests. " << std::endl;
	p.loadFromFile(inputFilename, tests);
	testsCreated = tests.size();
	std::cout << "Loaded in " << testsCreated << " tests. " <<  std::endl;

	bool writeToFile = outputFilename != "";

	//If you want to write to file it checks that it is there before commencing tests
	if (writeToFile){
		checkOFileExists(outputFilename);
	}

	//Go through all of the tests and see which are solvable
	while (!tests.empty()){

		//If it wasn't input with a pagoda, try generate one for it
		if (!tests.back().hasPagoda){
			tests.back().hasPagoda = p.generatePagoda(tests.back().pagoda, tests.back().endState, savePagodas);
			tests.back().hasPagoda = true;
		}

		//Count the number of verified pagodas
		if (p.verifyPagoda(tests.back().pagoda, savePagodas)){
			verifiedPagodas++;
		}

		//Try and prove the board insolvable
		if (p.proveInsolvWithSaved(tests.back())){
			insolvableProved++;
			if (writeToFile) results.push_back(tests.back());
		}


		testsCompleted++;
		tests.pop_back();
		if (testsCompleted % 10000 == 0) std::cout << "Tests completed: " << testsCompleted << std::endl;

	}

	//Write to console the results of testing
	std::cout << std::endl << *this << std::endl << std::endl;

	if (writeToFile){
		if (!writeResultsToFile(outputFilename)) std::cout << "Could not write results to file. " << std::endl;
	}

	if (savePagodas){
		if (!p.savePagodaFunctions(pagodaOutput)) std::cout << "Failed to save known pagodas to file. " << std::endl;
	}

};

//TYPE 1 threading test, takes a number of tests to do, a batch size and optional output file.
//1/4 of threads produce and the rest consume, using shared buffer tests.
void PagodaTester::Threadedtest(int numberOfTests, int batchSize, const std::string& outputFile){


	resetTester();

	//If only one thread then perform sequential test!
	if (threads == 1){ sequentialTest(numberOfTests, outputFile);
	} else {
		bool writeToFile = outputFile != "";

		//If you want to write to file it checks that it is there before commencing tests
		if (writeToFile){
			checkOFileExists(outputFile);
		}

		//Allocate as much space as necessary
		if (numberOfTests > maxSize) tests.reserve(maxSize);
		else tests.reserve(numberOfTests);

		//If not threaded perform the sequential test!


		StartCounter();

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

		//Check to see if consumers have finished yet
		while (!consumerQuit){
			if (testsCreated <= testsCompleted){
				consumerQuit = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
			}
		}

		//Join with all threads created
		for (unsigned int i=0; i<threadArray.size(); ++i){
			if (threadArray[i].joinable()) threadArray[i].join();
		}

		std::cout << std::endl << "Time taken (ms): " << GetCounter() << std::endl;
		std::cout << *this << std::endl;

		if (writeToFile){
			if (!writeResultsToFile(outputFile)) std::cout << "Could not write results to file." << std::endl;
		}

		if (savePagodas){
			std::cout << "Saving pagodas to file. " << std::endl;
			if (!p.savePagodaFunctions(pagodaOutput)) std::cout << "Failed to save known pagodas to file. " << std::endl;
		}

	}
}

//TYPE 1 threading. Takes a file as input for tests and performs them with a given batch size.
//Prints to optional output file if requested.
void PagodaTester::Threadedtest(const std::string& inputFilename, int batchSize, const std::string& outputFilename){

	resetTester();

	std::cout << "Loading tests from file '" << inputFilename << "'." << std::endl;
	p.loadFromFile(inputFilename, tests);

	bool writeToFile = outputFilename != "";

	testsCreated = tests.size();
	std::cout << "Loaded in " << testsCreated << " tests. " <<  std::endl;

	//If you want to write to file it checks that it is there before commencing tests
	if (writeToFile){
		checkOFileExists(outputFilename);
	}

	StartCounter();

	//Create all of the threads and set them consuming all loaded in tests
	for (int i=0; i<threads; ++i){
		threadArray.push_back(std::thread(&PagodaTester::consumer, this, batchSize, writeToFile));
	}

	//While the consumers are still going
	while (!consumerQuit){
		if (testsCreated <= testsCompleted) consumerQuit = true;
		else std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	//Join with all threads created
	for (unsigned int i=0; i<threadArray.size(); ++i){
		if (threadArray[i].joinable()) threadArray[i].join();
	}

	std::cout << std::endl << "Time taken (ms): " << GetCounter() << std::endl;
	std::cout << *this << std::endl << std::endl;

	if (writeToFile){
		if (!writeResultsToFile(outputFilename)) std::cout << "Could not write results to file. " << std::endl;
	}

	if (savePagodas){
		if (!p.savePagodaFunctions(pagodaOutput)) std::cout << "Failed to save known pagodas to file. " << std::endl;
	}
}


//TYPE 2 threading. Given a number of tests and batch size creates threads who attempt to acquire
//jobs until told to stop. Then puts jobs into queue until number of tests required has been done.
//Sleeps until job queue is empty
void PagodaTester::ThreadedtestType2(int numberOfTests, int batchSize, const std::string& outputFile){

	//If only 1 thread then perform sequential method!
	if (threads == 1) sequentialTest(numberOfTests, outputFile);
	else {

		resetTester();

		this->testsCreated = numberOfTests;
		this->testsCompleted = numberOfTests;

		bool recordResults = outputFile != "";

		//Check if the file is actually valid
		if (recordResults){
			checkOFileExists(outputFile);
		}

		std::cout << "Starting tests. " << std::endl;
		StartCounter();

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
		jobs.push( [this, numberOfTests, recordResults] { this->produceAndConsume(numberOfTests, recordResults);});
		jobMutex.unlock();

		std::cout << "Pushed all jobs onto job list" << std::endl;

		//While jobs remain sleep the main thread
		while (!jobs.empty()){
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		//Stop threads when finished.
		consumerQuit = true;

		for (unsigned int i=0; i<threadArray.size(); ++i)
			if (threadArray[i].joinable()) threadArray[i].join();

		while (!threadArray.empty()){
			threadArray.pop_back();
		}

		std::cout << std::endl << "Time taken (ms): " << GetCounter() << std::endl;
		std::cout << "Threads destroyed. " << std::endl;
		std::cout << std::endl << *this << std::endl << std::endl;

		//Print the results to file
		if (recordResults){
			if (!writeResultsToFile(outputFile)) std::cout << "Could not write results to file. " << std::endl;
		}

		if (savePagodas){
			if (!p.savePagodaFunctions(pagodaOutput)) std::cout << "Failed to save known pagodas to file" << std::endl;
		}
	}

}



//Consumer method. Attempts to acquire tests to do, and consumes them once it has them
void PagodaTester::consumer(int batchSize, bool recordResults){

	//Storage of local jobs to do
	std::vector<Pagoda::BoardPair> internalTests = std::vector<Pagoda::BoardPair>();

	while (!consumerQuit){

		//Try to acquire jobs
		if (testsMutex.try_lock()){

			//Collect all jobs possible until you reach your batch size
			//or there are no jobs left...
			while (!tests.empty() && internalTests.size()< (unsigned int) batchSize){
				internalTests.push_back(tests.back());
				tests.pop_back();
			}
			testsMutex.unlock();
		}

		int internalTestsComplete = 0;

		//While you have jobs to do, do them
		while (!internalTests.empty()){

			//If it hasnt got a pagoda generate one
			if (generateNewPags)
				if (!internalTests.back().hasPagoda)
					internalTests.back().hasPagoda = p.generatePagoda(internalTests.back().pagoda, internalTests.back().endState, accumPagodas);

			//Try and use the generated pagoda
			if (internalTests.back().hasPagoda){
				if (p.verifyPagoda(internalTests.back().pagoda, accumPagodas)){

					//Let the program know you have found a valid pagoda
					verifiedMut.lock();
					verifiedPagodas++;
					verifiedMut.unlock();
				}
			}

			if (p.proveInsolvWithSaved(internalTests.back())){

				//Let the program know you have found a problem that can be proved insolvable
				insolvableMut.lock();
				insolvableProved++;
				if (recordResults) results.push_back(internalTests.back());
				insolvableMut.unlock();

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


//Creates randomised tests (based on specification) locally and places them on the 
//shared tests vector once completed a batch
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
			while (internalTests.size() < (unsigned int) thisBatch){
				internalTests.push_back(p.createRandomBoardPair(rand()));
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

//Creates a supplied number of tests locally and attempts to solve it. Records results
//if requested.
void PagodaTester::produceAndConsume(int tests, bool recordResults){

	Pagoda::BoardPair bp(33);

	//Do the allocated number of tests
	while (tests > 0){

		//Create a random board pair
		bp = p.createRandomBoardPair(rand());	

		//Attempt pagoda generation if requested
		if (generateNewPags)
			if (p.generatePagoda(bp.pagoda, bp.endState, accumPagodas)){

				verifiedMut.lock();
				verifiedPagodas++;
				verifiedMut.unlock();

				bp.hasPagoda = true;
			};

			if (p.proveInsolvWithSaved(bp)){

				insolvableMut.lock();
				insolvableProved++;
				if (recordResults) results.push_back(bp);
				insolvableMut.unlock();

			};

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
	os << "Insolvable found: " << p.insolvableProved << std::endl;
	os << "Unique pagodas stored: " << p.p.getNoUniquePag();
	return os;
}

void PagodaTester::resetTester(){
	p = Pagoda(boardEng);

	if (loadPagodas){
		if (!p.loadPagodaFunctions(pagodaInput)) std::cout << "Pagoda function file '" << pagodaInput << "' not found. " << std::endl;
	}

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

void PagodaTester::checkOFileExists(const std::string& filename){
	std::ofstream file = std::ofstream();
	file.open(filename);
	if (!file.is_open()) throw std::invalid_argument("Could not create output file: '" + filename + "'.");

	file.close();
}

bool PagodaTester::writeResultsToFile(const std::string& filename){
	std::cout << "Writing to file: " << filename << std::endl;
	for (unsigned int i=0; i<results.size(); ++i)
		if (!p.printToFile(filename, results[i], ((bool) i)))
			return false;

	return true;
}