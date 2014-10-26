#pragma once

#include "Solitaire.h"
#include <string>
#include <fstream>
#include <set>
#include <mutex>


/*
	NOTES:
		-Implementation of the copy constructor and assignment operators was necessary due to
		 the use of a mutex
		-Implicit move constructors do not work with the VS2012 compiler, so have been written
		 for BoardPair
		-A work around has been used for the lack of sharable mutex support in ++11. set of known 
		 pagoda functions is only locked when generation is possible. This is dangerous if not used
		 appropriately.
		-Boards have been represented as integers rather than bools to reduce the overhead of
		 calculations using the board state.
*/

class Pagoda
{
public:

	//This struct represents a start and end state, with a pagoda.
	struct BoardPair {
		Vector<double> pagoda;
		Vector<int> startState;
		Vector<int> endState;

		bool hasPagoda;

		BoardPair(): 
			hasPagoda(false){};

		BoardPair(int size):
			hasPagoda(false){
				pagoda = Vector<double>();
				startState = Vector<int>(size);
				endState = Vector<int>(size);
		};

		BoardPair(BoardPair&& rval){
			std::swap(pagoda, rval.pagoda);
			std::swap(startState, rval.startState);
			std::swap(endState, rval.endState);
			std::swap(hasPagoda, rval.hasPagoda);
		}

		~BoardPair(){  }
	};

	Pagoda(bool eng = 1);
	Pagoda(const Matrix<bool>& boardShape);
	Pagoda(const Pagoda& rhs);					//Copy
	Pagoda& operator=(const Pagoda& rhs);		//Copy
	~Pagoda(void);

	//IO FUNCTIONALITY
	void loadFromFile(const std::string& filename, std::vector<BoardPair>& bps);
	bool printToFile(const std::string& filename, const BoardPair& bp, bool append = false);

	bool savePagodaFunctions(const std::string& filename);
	bool loadPagodaFunctions(const std::string& filename);

	friend std::ostream& operator<<(std::ostream& os, const Pagoda& p);
	std::ostream& printBoardPair(std::ostream& os, const BoardPair& bp, const bool append = false);

	//PAGODA FUNCTION
	bool generatePagoda(Vector<double>& pagoda, const Vector<int>& endState, const bool saveResults = false);
	bool verifyPagoda(const Vector<double>& pagoda, const bool saveResults = false);
	
	//Will just try to prove that your board is insolvable with the pagoda function
	bool proveInsolvable(const BoardPair& bp);

	//Will use all known pagodas to try prove insolvability of your states
	bool proveInsolvWithSaved(BoardPair& bp);

	//STATE CREATION
	static Vector<int> createRandomStateVector(const Matrix<bool>& board);
	BoardPair createRandomBoardPair(int i) const;

	//Necessary Getters
	int getNoPegHoles() const { return pegHoles; };
	int getNoUniquePag() const { return pagodaFunctions.size(); };

	//Set the generation mode
	void setGenerationMode(bool a){ possibleGeneration = a; }

private:
	Matrix<bool> board;

	Matrix<int> indexMat;
	YaleMatrix<int> jumpMat;
	int pegHoles;

	bool possibleGeneration;

	//THIS ONLY STORES VALID PAGODA FUNCTIONS, thread safe if generation mode is on
	std::set<Vector<double>> pagodaFunctions;
	void savePagoda(const Vector<double>& pagoda);
	std::mutex pagodaFuncsMut;

	template<typename E>
	void print_vector_as_board(std::ostream& os, const Vector<E>& v);

	template<typename E>
	void loadVectorFromBoard(std::istream& is, Vector<E>& v);
	
};

