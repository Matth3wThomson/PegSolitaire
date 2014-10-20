#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Solitaire.h"
#include "Pagoda.h"
#include <iostream>
#include "PerformanceCounter.h"



int main(){

	// Enable memory leak detection
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	{

		Solitaire a(true);
		Pagoda p(true);

		//a.play_game();

		//system("pause");

		//int tval = 100;

		///*Pagoda a = Pagoda();*/
		//

		//for (int i=0; i<tval; ++i){

		//}

		
		StartCounter();

		Vector<int> pagoda = Vector<int>();

		int noGenerated = 0;
		int provedInsolvable = 0;

		for (int i=0; i<10; ++i){
			if (p.generate_pagoda(pagoda)) noGenerated++;
			if (p.prove_insolvable(pagoda)) provedInsolvable++;
			p.randomize_start_and_end();
			
		}

		std::cout << "Pagodas Generated: " << noGenerated << std::endl;
		std::cout << "Proved insolvable: " << provedInsolvable << std::endl;

		std::cout << "Time Taken: " << GetCounter() << std::endl;
		system("pause");

	}
	
	_CrtDumpMemoryLeaks();
}