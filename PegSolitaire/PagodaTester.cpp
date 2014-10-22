#include "PagodaTester.h"


PagodaTester::PagodaTester(int t)
{
	p = Pagoda();

	int hardwareThreads = (signed int) std::thread::hardware_concurrency();

	if (t <= 1) threads = 1;
	else if (hardwareThreads < 1) threads = 1;
	else if (t > hardwareThreads) threads = hardwareThreads;
	else threads = t;


}


PagodaTester::~PagodaTester(void)
{

}

void PagodaTester::test(int numberOfTests, const std::string& outputFile){

	//main thread generating jobs and multiple thread consuming?

	//create producer threads
	//create consumer threads

	//Wait for producer threads to complete or.......

	//.......while (!producerQuit) producer();

	//while (!consumerQuit){
	//if tests Created <= tests Completed then let consumers close
	//}
	
}

//TODO: if you find a result, print to file if requested
void PagodaTester::consumer(int batchSize, bool recordResults){

	std::vector<Pagoda::BoardPair> internalTests = std::vector<Pagoda::BoardPair>();

	while (!consumerQuit){

		//Try to acquire jobs
		if (jobMutex.try_lock()){

			//Collect all jobs possible until you reach your batch size
			//or there are no jobs left...
			while (!tests.empty() && internalTests.size()<batchSize){
				internalTests.push_back(tests.back());
				tests.pop_back();
			}

			jobMutex.unlock();
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
						results.push_back(internalTests.back());
						insolvableMut.unlock();

					}
				}
			}

			//Keep track of all tests you do internally
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

			//Acquire the lock to put the tests you have created into
			//the job list
			jobMutex.lock();
			while (!internalTests.empty()){
				tests.push_back(internalTests.back());
				internalTests.pop_back();
			}
			jobMutex.unlock();
		}

	}
}

