
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

		Solitaire a(true);

		a.play_game();

		system("pause");

	}
	
	_CrtDumpMemoryLeaks();
}