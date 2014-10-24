#pragma once

#include "Solitaire.h"
#include <string>
#include <fstream>

//TODO: Refactor code so that start and end state can be represented as bools?
//TODO: Provide guards on all methods to prevent incorrect usage

class Pagoda
{
public:

	//TODO: Implicit move constructors do not work with our compiler
	struct BoardPair {
		//A pair of start and end states, and a pointer to a pagoda function that solves them
		Vector<int> pagoda;
		Vector<int> startState;
		Vector<int> endState;

		bool hasPagoda;

		BoardPair(): 
			hasPagoda(false){};

		BoardPair(int size):
			hasPagoda(false){
				pagoda = Vector<int>();
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
	~Pagoda(void);

	/*bool set_start_state(const Vector<int>& start);
	bool set_end_state(const Vector<int>& end);*/

	bool load_from_file(const std::string& filename, std::vector<BoardPair>& bps);
	bool print_to_file(const std::string& filename, const BoardPair& bp, bool append = false);

	friend std::ostream& operator<<(std::ostream& os, const Pagoda& p);
	std::ostream& printPagCom(std::ostream& os, const BoardPair& bp, const bool append = false);


	/*bool generate_pagoda(Vector<int>& pagoda);*/
	bool generate_pagoda(Vector<int>& pagoda, const Vector<int>& endState);
	bool verify_pagoda(const Vector<int>& pagoda);
	
	//Will just try to prove that your board is insolvable with the pagoda function
	bool prove_insolvable(const BoardPair& bp);

	//Will use all known pagodas to try prove insolvability of your states
	bool prove_insolv_with_saved(const BoardPair& bp);

	static Vector<int> create_random_state_vector(const Matrix<bool>& board);
	BoardPair create_random_board_pair(int i) const;

	/*void randomize_start_and_end();*/

	//Gets //TODO: Is passing this by reference safe?
	Matrix<bool>& get_board(){ return board; };
	int get_no_peg_holes() const { return pegHoles; };
	Vector<int>& getPagoda(int index){ return pagodaFunctions[index]; };

private:
	Matrix<bool> board;

	Matrix<int> indexMat;
	YaleMatrix<int> jumpMat;

	//THIS ONLY STORES VALID PAGODA FUNCTIONS
	std::vector<Vector<int>> pagodaFunctions;

	template<typename E>
	void print_vector_as_board(std::ostream& os, const Vector<E>& v);

	template<typename E>
	void load_vector_from_board(std::istream& is, Vector<E>& v);

	int pegHoles;

	//TODO: Currently ints for simple multiplication in prove_insolvable... CHANGE TO BOOLS
	//and provide a casting method? 
	/*Vector<int> startStateVec;
	Vector<int> endStateVec;*/
};

