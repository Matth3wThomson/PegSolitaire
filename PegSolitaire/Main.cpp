#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Solitaire.h"
#include <iostream>
#include "PerformanceCounter.h"



int main(){

	// Enable memory leak detection
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	{

		/*Solitaire a(true);

		a.play_game();

		system("pause");*/

	/*	Matrix<int> b(4, 4, 0);
		b[1][0] = 5;
		b[1][1] = 8;
		b[2][2] = 3;
		b[3][1] = 6;

		Matrix<int> a(4, 6, 0);
		a[0][0] = 10;
		a[0][1] = 20;
		a[1][1] = 30;
		a[1][3] = 40;
		a[2][2] = 50;
		a[2][3] = 60;
		a[2][4] = 70;
		a[3][5] = 80;

		YaleMatrix<int> ym(b);
		YaleMatrix<int> ym2(a);

		std::cout << ym << std::endl;
		std::cout << ym2 << std::endl;*/
		Matrix<int> a(10, 10, 5);
		Matrix<int> b(10, 10, 3);
		Matrix<int> c(11, 11, 7);

		Vector<int> d(5, 5);
		Vector<int> e(5, 3);
		Vector<int> f(10, 3);

		/*Solitaire a(true);*/
		StartCounter();

		for (int i=0; i<10000; ++i){
			d*e;
		}

		std::cout << "10000 working additions: " << GetCounter();

		system("pause");

	}
	
	_CrtDumpMemoryLeaks();
}