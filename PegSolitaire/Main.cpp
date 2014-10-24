#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Solitaire.h"
#include "Pagoda.h"
#include <iostream>
#include "PagodaTester.h"
#include "PerformanceCounter.h"

//TODO: GO THROUGH AREAS OF MY PROGRAM AND LOOK FOR PLACES IM PROUD OF
//TODO: Change matrix to be the same as new matrix
//TODO: Evaluate performance of type 3
//TODO: Get help to fix type 2

int main(){

	// Enable memory leak detection
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	{

		std::cout << "Program start. " << std::endl;
		PagodaTester pt(8);

		std::cout << "Threaded test. " << std::endl;
		StartCounter();
		/*pt.sequentialTest(1000, "pagodaResults.txt");*/
		/*pt.sequentialTest("pagodaResults.txt", "pagodaResults.txt");*/
		/*pt.Threadedtest(10000000, 100);*/
		/*pt.Threadedtest("pagodaResults.txt", 100, "verifyResults.txt");*/
		/*pt.ThreadedtestType3(1000000, "verifyResults.txt");*/
		pt.ThreadedtestType2(10000000, 100);
		/*pt.verifyFile("pagodaResults.txt");*/
		std::cout << "Time taken: " << GetCounter() << std::endl;
		//std::cout << "Sequential test. " << std::endl;

		/*pt.~PagodaTester();*/

		system("pause");

	}

	_CrtDumpMemoryLeaks();
}