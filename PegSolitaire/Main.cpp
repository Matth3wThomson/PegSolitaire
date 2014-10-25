#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Solitaire.h"
#include "Pagoda.h"
#include <iostream>
#include "PagodaTester.h"

//TODO: Move performance counter into tester!
//TODO: Evaluate performance of type 3

int main(){

	// Enable memory leak detection
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	{

		std::cout << "Program start. " << std::endl;

		/*std::vector<Pagoda::BoardPair> boards;
		Pagoda p(true);
		p.load_from_file("testDoubles.txt", boards);*/


		Matrix<int> mat(3, 3);
		for (int i=0; i<mat.get_x_dim(); ++i)
			for (int j=0; j<mat.get_y_dim(); ++j)
				mat[i][j] = rand() % 3;

		YaleMatrix<int> a(mat);


		Vector<int> b(3);

		b[0] = 1;
		b[1] = 1;
		b[2] = 1;
		/*b[3] = 1;
		b[4] = 1;*/

		Vector<double> d(3);

		d[0] = 0.4;
		d[1] = 3.8;
		d[2] = 6.7;
		/*d[3] = 8.8;
		d[4] = 0.3;*/

		/*std::cout << b * d << std::endl;*/
		/*std::cout << d * b << std::endl;*/

		std::cout << a * d << std::endl;

		//PagodaTester pt(8);
		//pt.setLoad(true);
		//pt.setSave(true);
		//pt.setAccum(true);
		//pt.setGenerateNewPags(true);

		///*std::cout << "Threaded test. " << std::endl;*/
		///*pt.sequentialTest(100000);*/
		///*pt.sequentialTest("pagodaResults.txt", "pagodaResults.txt");*/
		///*pt.Threadedtest(1000000, 100, "pagodaResults.txt");*/
		///*pt.Threadedtest("pagodaResults.txt", 100);*/
		///*pt.ThreadedtestType3(1000000, "verifyResults.txt");*/
		//pt.ThreadedtestType2(10000000, 100);
		/*pt.verifyFile("pagodaResults.txt", "Crazy tests results.txt");*/
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