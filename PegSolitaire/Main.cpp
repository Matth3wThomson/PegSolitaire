
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Solitaire.h"
#include <iostream>



int main(){

	// Enable memory leak detection
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	{
		/*Vector<int>(5, 5);
		Matrix<int>(10, 10, 3);

		Matrix<bool> board(7, 7, 1);

		std::cout << board;*/

		Matrix<bool> board(2, 3, true);

		Solitaire a(true);

		/*Solitaire b(board);*/

		//While it is not the end of the game
		/*while (!a.EOG()){
			std::cout << a << std::endl;
			
			int x, y;
			char dir;

			do {
				std::cout << "Please enter an X coordinate: ";
				std::cin >> x;

				std::cout << "Please enter a Y coordinate: ";
				std::cin >> y;

				std::cout << "Please enter a direction; (U)p, (D)own, (L)eft, (R)ight: ";
				std::cin >> dir;

			}while (!a.perform_move(x, y, dir));

			std::cout << "Move made! ";
			system("pause");
			system("cls");
		}*/

		a.play_game();

		system("pause");

	}
	
	_CrtDumpMemoryLeaks();
}