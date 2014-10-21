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
		StartCounter();

		Pagoda p(true);

		Vector<int> pagoda = Vector<int>(33);
		
			/*

		pagoda[0] = -1;
		pagoda[2] = -1;
		pagoda[3] = 1;
		pagoda[4] = 1;
		pagoda[5] = 1;
		pagoda[6] = -1;
		pagoda[7] = 1;
		pagoda[9] = 1;
		pagoda[11] = 1;
		pagoda[12] = -1;
		pagoda[14] = 1;
		pagoda[15] = 1;
		pagoda[16] = 2;
		pagoda[17] = 1;
		pagoda[18] = 1;
		pagoda[20] = -1;
		pagoda[21] = 1;
		pagoda[23] = 1;
		pagoda[25] = 1;
		pagoda[26] = -1;
		pagoda[27] = 1;
		pagoda[28] = 1;
		pagoda[29] = 1;
		pagoda[30] = -1;
		pagoda[32] = -1;*/


		//Example insolvable problem
		//Vector<int> start = Vector<int>(33);
		//Vector<int> end = Vector<int>(33);
		//for (int i=0; i<33; ++i){
		//	start[i] = 1;
		//	end[i] = 0;
		//}

		//start[16] = 0;
		//end[16] = 1;

		///*for (int i=0; i<33; ++i)
		//if (i == 9 || i == 15 || i == 16 || i == 17 || i == 23)
		//start[i] = 0;
		//else start[i] = 1;*/

		///*

		//for (int i=0; i<33; ++i)
		//if (i == 9 || i == 15 || i == 16 || i == 17 || i == 23)
		//end[i] = 1;
		//else end[i] = 0;*/

		//p.set_start_state(start);
		//p.set_end_state(end);

		////p.generate_pagoda(pagoda);


		///*std::cout << p.verify_pagoda(pagoda) << std::endl;
		//std::cout << p.prove_insolvable(pagoda) << std::endl;*/

		//Vector<int> pagoda = Vector<int>();
		//if (p.generate_pagoda(pagoda)){ 
		//	std::cout << "pagoda generated" << std::endl;
		//	std::cout << pagoda << std::endl;
		//	if (p.prove_insolvable(pagoda)){
		//		std::cout << "English game insolvable" << std::endl;
		//		}		
		//}

		std::vector<Pagoda::PagodaCombination> PagodaCombs;

		/*
		for (int i=0; i<100; ++i){
			Pagoda::PagodaCombination pc = Pagoda::PagodaCombination();
			pc.startState = p.create_random_state_vector(p.get_board());
			pc.endState = p.create_random_state_vector(p.get_board());
			PagodaCombs.push_back(pc);
		}

		for (int i=0; i<100; ++i){
			p.print_to_file("results.txt", PagodaCombs[i], i);
		}*/
			

		std::cout << p.load_from_file("results.txt", PagodaCombs) <<std::endl;

		for (int i=0; i<PagodaCombs.size(); ++i)
			p.printPagCom(std::cout, PagodaCombs[i], i);


		int noGenerated = 0;
		int provedInsolvable = 0;

		for (int i=0; i<PagodaCombs.size(); ++i){
			if (PagodaCombs[i].pagodaFunction == NULL); PagodaCombs[i].pagodaFunction = new Vector<int>(p.get_no_peg_holes());
			if (p.generate_pagoda(*PagodaCombs[i].pagodaFunction, PagodaCombs[i].endState)){
				noGenerated++;
				if (p.prove_insolvable(PagodaCombs[i])) provedInsolvable++;
			}
		}

		std::cout << "Number of tests: " << PagodaCombs.size() << std::endl;
		std::cout << "Pagodas Generated: " << noGenerated << std::endl;
		std::cout << "Proved insolvable: " << provedInsolvable << std::endl;

		std::cout << "Time Taken: " << GetCounter() << std::endl;

		/*int noGenerated = 0;
		int provedInsolvable = 0;
		int tests = 10000;
		for (int i=0; i<tests; ++i){
			if (p.generate_pagoda(pagoda,  pc.endState)){ 
				noGenerated++;
				if (p.prove_insolvable(pc)){

					std::cout << pagoda << std::endl;
					provedInsolvable++;
				}
			}
			
		}

		std::cout << "Number of tests: " << tests << std::endl;
		std::cout << "Pagodas Generated: " << noGenerated << std::endl;
		std::cout << "Proved insolvable: " << provedInsolvable << std::endl;

		std::cout << "Time Taken: " << GetCounter() << std::endl;*/
		system("pause");

	}

	_CrtDumpMemoryLeaks();
}