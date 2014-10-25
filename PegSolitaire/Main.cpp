#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Solitaire.h"
#include "Pagoda.h"
#include <iostream>
#include "PagodaTester.h"
#include "PerformanceCounter.h"

//TODO: Move performance counter into tester!
//TODO: Evaluate performance of type 3

int main(){

	// Enable memory leak detection
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	{

		std::cout << "Program start. " << std::endl;
		PagodaTester pt(8);

		/*std::cout << "Threaded test. " << std::endl;*/
		StartCounter();
		/*pt.sequentialTest(100000, true, true, "pagodaResults.txt");*/
		/*pt.sequentialTest("pagodaResults.txt", true, true, "pagodaResults.txt");*/
		pt.Threadedtest(1000000, 100, true, true);
		/*pt.Threadedtest("pagodaResults.txt", 100, "verifyResults.txt");*/
		/*pt.ThreadedtestType3(1000000, "verifyResults.txt");*/
		/*pt.ThreadedtestType2(100000, 100, "Crazy test results.txt");*/
		/*pt.verifyFile("pagodaResults.txt", "Crazy tests results.txt");*/
		std::cout << "Time taken: " << GetCounter() << std::endl;
		//std::cout << "Sequential test. " << std::endl;

		//Matrix<int> m;
		//Matrix<int> n(3,4);
		//
		//m[0][0] = 0;
		//m[0][1] = 1;
		//m[0][2] = 2;
		//m[1][0] = 6;
		//m[1][1] = 4;
		//m[1][2] = 3;
		//m[2][0] = 6;
		//m[2][1] = 7;
		//m[2][2] = 8;

		///*for (int i=0; i<m.get_x_dim(); ++i)
		//	for (int j=0; j<m.get_y_dim(); ++j)
		//		m[i][j] = i+j;*/

		//for (int i=0; i<n.get_x_dim(); ++i)
		//	for (int j=0; j<n.get_y_dim(); ++j)
		//		n[i][j] = 7;


		//std::cout << n << std::endl;
		//std::cout << n.transpose() << std::endl;

		//
		//std::cout << m * n << std::endl;
		///*pt.~PagodaTester();*/

		system("pause");

	}

	_CrtDumpMemoryLeaks();
}