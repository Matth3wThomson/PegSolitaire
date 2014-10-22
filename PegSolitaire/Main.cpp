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
		
		std::vector<Pagoda::BoardPair> PagodaCombs;

		/*
		for (int i=0; i<100; ++i){
			Pagoda::BoardPair bp = Pagoda::BoardPair();
			bp.startState = p.create_random_state_vector(p.get_board());
			bp.endState = p.create_random_state_vector(p.get_board());
			PagodaCombs.push_back(bp);
		}

		for (int i=0; i<100; ++i){
			p.print_to_file("results.txt", PagodaCombs[i], i);
		}*/
			

		std::cout << p.load_from_file("results.txt", PagodaCombs) <<std::endl;

		/*for (int i=0; i<PagodaCombs.size(); ++i)
			p.printPagCom(std::cout, PagodaCombs[i], i);*/


		int noGenerated = 0;
		int provedInsolvable = 0;
		int verifiedPagodas = 0;

		/*for (int i=0; i<PagodaCombs.size(); ++i){
			if (PagodaCombs[i].pagodaFunction == NULL); PagodaCombs[i].pagodaFunction = new Vector<int>(p.get_no_peg_holes());
			if (p.generate_pagoda(*PagodaCombs[i].pagodaFunction, PagodaCombs[i].endState)){
				noGenerated++;
				if (p.prove_insolvable(PagodaCombs[i])) provedInsolvable++;
			}
		}*/

		for (unsigned int i=0; i<PagodaCombs.size(); ++i){
			if (PagodaCombs[i].pagodaFunction != NULL)
				if (p.verify_pagoda(*PagodaCombs[i].pagodaFunction)) verifiedPagodas++;
			if (p.prove_insolvable(PagodaCombs[i])) provedInsolvable++;
		}

		std::cout << "Number of tests: " << PagodaCombs.size() << std::endl;
		std::cout << "Pagodas Generated: " << noGenerated << std::endl;
		std::cout << "Verfied Pagodas: " << verifiedPagodas << std::endl;
		std::cout << "Proved insolvable: " << provedInsolvable << std::endl;

		std::cout << "Time Taken: " << GetCounter() << std::endl;

		system("pause");

	}

	_CrtDumpMemoryLeaks();
}