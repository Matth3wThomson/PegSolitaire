#pragma once

#include "Solitaire.h"
#include <string>
#include <fstream>

//TODO: Refactor code so that start and end state can be represented as bools?

class Pagoda
{
public:

	struct PagodaCombination {
		//A pair of start and end states, and a pointer to a pagoda function that solves them
		Vector<int>* pagodaFunction;
		Vector<int> startState;
		Vector<int> endState;

		PagodaCombination():
			pagodaFunction(NULL){};

		PagodaCombination(int size):
			pagodaFunction(NULL){
				startState = Vector<int>(size);
				endState = Vector<int>(size);
		};

		//NOTE: No destructor here for the pointer, as pagodaCombination does not have ownership
		//over its pagoda functions
	};

	Pagoda(bool eng = 1);
	Pagoda(const Matrix<bool>& boardShape);
	~Pagoda(void);

	/*bool set_start_state(const Vector<int>& start);
	bool set_end_state(const Vector<int>& end);*/

	bool load_from_file(const std::string& filename, std::vector<PagodaCombination>& pcs);
	bool print_to_file(const std::string& filename, const PagodaCombination& pc, bool append = false);

	friend std::ostream& operator<<(std::ostream& os, const Pagoda& p);
	std::ostream& printPagCom(std::ostream& os, const PagodaCombination& pc, const bool append = false);
	

	/*bool generate_pagoda(Vector<int>& pagoda);*/
	bool generate_pagoda(Vector<int>& pagoda, const Vector<int>& endState);
	bool verify_pagoda(const Vector<int>& pagoda);
	/*bool prove_insolvable(const Vector<int>& pagoda);*/
	bool prove_insolvable(const PagodaCombination& pc);

	static Vector<int> create_random_state_vector(const Matrix<bool>& board);

	/*void randomize_start_and_end();*/

	//Gets //TODO: Is passing this by reference safe?
	Matrix<bool>& get_board(){ return board; };
	int get_no_peg_holes() const { return pegHoles; };

private:
	Matrix<bool> board;

	Matrix<int> indexMat;
	YaleMatrix<int> jumpMat;

	//Should this be pointers?
	std::vector<Vector<int>*> pagodaFunctions;

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

