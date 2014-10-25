#pragma once

#include "Solitaire.h"
#include <string>
#include <fstream>
#include <set>
#include <mutex>

//TODO: Refactor code so that start and end state can be represented as bools?
//TODO: Provide guards on all methods to prevent incorrect usage

/*
	NOTES:
		-Implementation of the copy constructor and assignment operators was necessary due to
		 the use of a mutex
		-Implicit move constructors do not work with the VS2012 compiler, so have been written
		 for BoardPair
		-A work around has been used for the lack of sharable mutex support in ++11. set of known 
		 pagoda functions is only locked when generation is possible. This is dangerous if not used
		 appropriately.
*/

class Pagoda
{
public:

	//TODO: Move the board pair constructors outside of pagoda?
	struct BoardPair {
		//A pair of start and end states, and a pointer to a pagoda function that solves them
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

		/*BoardPair& operator=(const BoardPair& rhs){
			this->pagoda = rhs.pagoda;
			this->startState = rhs.startState;
			this->endState = rhs.endState;
			this->hasPagoda = rhs.hasPagoda;
			return *this;
		}*/

		~BoardPair(){  }
	};

	Pagoda(bool eng = 1);
	Pagoda(const Matrix<bool>& boardShape);
	Pagoda(const Pagoda& rhs);					//Copy
	Pagoda& operator=(const Pagoda& rhs);		//Copy
	~Pagoda(void);

	

	/*bool set_start_state(const Vector<int>& start);
	bool set_end_state(const Vector<int>& end);*/

	//IO FUNCTIONALITY //TODO: Have load from file accept a boolean and store verified pagodas
	void load_from_file(const std::string& filename, std::vector<BoardPair>& bps);
	bool print_to_file(const std::string& filename, const BoardPair& bp, bool append = false);

	bool save_pagoda_functions(const std::string& filename);
	bool load_pagoda_functions(const std::string& filename);

	friend std::ostream& operator<<(std::ostream& os, const Pagoda& p);
	std::ostream& printPagCom(std::ostream& os, const BoardPair& bp, const bool append = false);

	//PAGODA
	bool generate_pagoda(Vector<double>& pagoda, const Vector<int>& endState, const bool saveResults = false);
	bool verify_pagoda(const Vector<double>& pagoda, const bool saveResults = false);
	
	//Will just try to prove that your board is insolvable with the pagoda function
	bool prove_insolvable(const BoardPair& bp);

	//Will use all known pagodas to try prove insolvability of your states
	bool prove_insolv_with_saved(BoardPair& bp);

	//STATE CREATION
	static Vector<int> create_random_state_vector(const Matrix<bool>& board);
	BoardPair create_random_board_pair(int i) const;

	/*void randomize_start_and_end();*/

	//Gets //TODO: Is passing this by reference safe?
	Matrix<bool>& get_board(){ return board; };
	int get_no_peg_holes() const { return pegHoles; };
	int get_no_unique_pag() const { return pagodaFunctions.size(); };

	void setGenerationMode(bool a){ possibleGeneration = a; }

private:
	Matrix<bool> board;

	Matrix<int> indexMat;
	YaleMatrix<int> jumpMat;
	int pegHoles;

	bool possibleGeneration;

	//THIS ONLY STORES VALID PAGODA FUNCTIONS
	std::set<Vector<double>> pagodaFunctions;
	void savePagoda(const Vector<double>& pagoda);
	std::mutex pagodaFuncsMut;

	template<typename E>
	void print_vector_as_board(std::ostream& os, const Vector<E>& v);

	template<typename E>
	void load_vector_from_board(std::istream& is, Vector<E>& v);

	//TODO: Currently ints for simple multiplication in prove_insolvable... CHANGE TO BOOLS
	//and provide a casting method? 
	
};

