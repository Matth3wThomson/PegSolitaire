#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Solitaire.h"
#include "Pagoda.h"
#include <iostream>
#include "PerformanceCounter.h"
#include "PagodaTester.h"

//TODO: GO THROUGH AREAS OF MY PROGRAM AND LOOK FOR PLACES IM PROUD OF

int main(){

	// Enable memory leak detection
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	{
		StartCounter();

		PagodaTester pt(1);

		Pagoda p(true);
		
		std::vector<Pagoda::BoardPair> BoardPairs;

		/*
		for (int i=0; i<100; ++i){
			Pagoda::BoardPair bp = Pagoda::BoardPair();
			bp.startState = p.create_random_state_vector(p.get_board());
			bp.endState = p.create_random_state_vector(p.get_board());
			BoardPairs.push_back(bp);
		}

		for (int i=0; i<100; ++i){
			p.print_to_file("results.txt", BoardPairs[i], i);
		}*/
			

		std::cout << p.load_from_file("results.txt", BoardPairs) <<std::endl;

		/*for (int i=0; i<BoardPairs.size(); ++i)
			p.printPagCom(std::cout, BoardPairs[i], i);*/


		int noGenerated = 0;
		int provedInsolvable = 0;
		int verifiedPagodas = 0;

		/*for (int i=0; i<BoardPairs.size(); ++i){
			if (BoardPairs[i].pagodaFunction == NULL); BoardPairs[i].pagodaFunction = new Vector<int>(p.get_no_peg_holes());
			if (p.generate_pagoda(*BoardPairs[i].pagodaFunction, BoardPairs[i].endState)){
				noGenerated++;
				if (p.prove_insolvable(BoardPairs[i])) provedInsolvable++;
			}
		}*/

		for (unsigned int i=0; i<BoardPairs.size(); ++i){
			if (BoardPairs[i].hasPagoda)
				if (p.verify_pagoda(BoardPairs[i].pagoda)) verifiedPagodas++;
			if (p.prove_insolvable(BoardPairs[i])) provedInsolvable++;
		}

		std::cout << "Number of tests: " << BoardPairs.size() << std::endl;
		std::cout << "Pagodas Generated: " << noGenerated << std::endl;
		std::cout << "Verfied Pagodas: " << verifiedPagodas << std::endl;
		std::cout << "Proved insolvable: " << provedInsolvable << std::endl;

		std::cout << "Time Taken: " << GetCounter() << std::endl;

		system("pause");

	}

	_CrtDumpMemoryLeaks();
}