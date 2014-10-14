
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

		Solitaire b(1);
		
		std::cout << b;

		system("pause");
	}
	
	_CrtDumpMemoryLeaks();
}