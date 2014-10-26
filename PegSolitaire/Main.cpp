#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Solitaire.h"
#include "Pagoda.h"
#include <iostream>
#include "PagodaTester.h"

/*
	NOTES:
		-I have not used any throws on signatures of methods due to the following
		 url: http://stackoverflow.com/questions/1055387/throw-keyword-in-functions-signature
*/

const std::string testsOutput = "outputFile.txt";

//Set of functions set for every run of a pagoda test, irrelevant of mode
void pagodaTesterSetup(PagodaTester& pt){

	int menu;

	std::cout << "Load in old pagodas? 1 = Y" << std::endl;
	std::cin >> menu;
	if (menu == 1) pt.setLoad(true);
	else pt.setLoad(false);

	std::cout << "Save pagodas found? 1 = Y" << std::endl;
	std::cin >> menu;
	if (menu == 1) pt.setSave(true);
	else pt.setSave(false);

	std::cout << "Use previously found pagodas to solve boards? 1 = Y" << std::endl;
	std::cin >> menu;
	if (menu == 1) pt.setAccum(true);
	else pt.setAccum(false);

	std::cout << "Attempt generation of pagoda for each board pair? 1 = Y" << std::endl;
	std::cin >> menu;
	if (menu == 1) pt.setGenerateNewPags(true);
	else pt.setGenerateNewPags(false);

}

//Need to hide all the horrible ifs used in the UI!
void testing_interface(){
	int menu;

	std::cout << "1 Verify file \n2 Run tests through generation \n3 Run tests through file loading(3)? " << std::endl;
	std::cin >> menu;

	//Verify file
	if (menu == 1){

		PagodaTester pt(8);

		std::string input;
		std::string output;

		bool success = true;

		do {
			success = true;

			std::cout << "Enter input filename:\n";
			std::cin >> input;

			std::cout << "Enter output filename: (N if no file writing desired)\n";
			std::cin >> output;

			if (output == "N") output = ""; 
			std::cout << std::endl;

			try {
				pt.verifyFile(input, output);
			} catch (std::invalid_argument ie){
				std::cout << "Operation failed: " << ie.what() << std::endl;
				success = false;
			}

		} while (!success);


	} else if (menu==2){
		// test generation
		PagodaTester pt;

		int numberOfTests = 0;
		int boardType;

		std::cout << "Board type:\n1 English\n2 European" << std::endl;
		std::cin >> boardType;

		if (boardType == 2) pt = PagodaTester(8, false);
		else pt = PagodaTester(8);

		pagodaTesterSetup(pt);

		std::cout << "How many tests? " << std::endl;
		std::cin >> numberOfTests;

		int output;
		std::cout << "Do you want all insolvable board pairs printed to file? (" << testsOutput << ") 1 = Y\n";
		std::cin >> output;

		std::cout << "Threaded? 1 = Y" << std::endl;
		std::cin >> menu;

		if (menu == 1){

			int batchSize = 0;
			std::cout << "What batch size? (per thread execution size)" << std::endl;
			std::cin >> batchSize;

			std::cout << "Which threaded version?\n 1 Producer/consumer sharing test buffer\n" <<
				" 2 Queue of lambda functions as a job queue. Each thread produces and consumes internally\n";
			std::cin >> menu;



			if (menu == 1)
				if (output == 1) pt.Threadedtest(numberOfTests, batchSize, testsOutput);
				else pt.Threadedtest(numberOfTests, batchSize);
			else 
				if (output == 1) pt.ThreadedtestType2(numberOfTests, batchSize, testsOutput);
				else pt.ThreadedtestType2(numberOfTests, batchSize);

		} else {
			if (output == 1) pt.sequentialTest(numberOfTests, testsOutput);
			else pt.sequentialTest(numberOfTests);
		}

	} else {
		//File loading
		PagodaTester pt(8);

		pagodaTesterSetup(pt);

		std::string input;

		int output;
		std::cout << "Do you want results printed to file? (" << testsOutput << ") 1 = Y" << std::endl;
		std::cin >> output;

		std::cout << "Threaded? 1 = Y" << std::endl;
		std::cin >> menu;

		bool success = true;

		do {

			std::cout << "Please enter filename of tests to do: " << std::endl;
			std::cin >> input;

			success = true;

			try {

				success = true;

				if (menu == 1){
					int batchSize = 100;

					std::cout << "What batch size? (per thread execution size) " << std::endl;
					std::cin >> batchSize;

					if (output == 1) pt.Threadedtest(input, batchSize, testsOutput);
					else pt.Threadedtest(input, batchSize);

				} else {
					if (output == 1) pt.sequentialTest(input, testsOutput);
					else pt.sequentialTest(input);
				}

			} catch (std::invalid_argument ia){
				std::cout << "Operation failed: " << ia.what() << std::endl;
				success = false;
			}

		} while (!success);
	}
}


int main(){

	// Enable memory leak detection
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	{

		bool quit = false;

		do {

			std::cout << "1 Solitaire Game \n2 Pagoda Testing \n";

			int menu;
			std::cin >> menu;

			if (menu == 1){
				Solitaire s;
				std::cout << "1 English \n2 European\n";
				std::cin >> menu;
				if (menu == 1)
					s = Solitaire(true);
				else s = Solitaire(false);

				s.playGame();

			} else {

				testing_interface();

			}

			std::cout << "Quit? (1/0)" << std::endl;
			std::cin >> quit;

		} while (!quit);

		system("pause");

	}

	_CrtDumpMemoryLeaks();
}