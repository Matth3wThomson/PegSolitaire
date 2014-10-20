#pragma once

#include "Solitaire.h"
#include <string>
#include <fstream>

class Pagoda
{
public:
	Pagoda(bool eng = 1);
	Pagoda(const Matrix<bool>& boardShape);
	~Pagoda(void);

	bool load_from_file(const std::string& filename);
	friend std::ostream& operator<<(std::ostream& os, const Pagoda& p);

	bool generate_pagoda(Vector<int>& pagoda);
	bool verify_pagoda(const Vector<int>& pagoda);
	bool prove_insolvable(const Vector<int>& pagoda);

	static Vector<int> create_random_state_vector(const Matrix<bool>& board);

	void randomize_start_and_end();

private:
	Matrix<bool> board;

	Matrix<int> indexMat;
	YaleMatrix<int> jumpMat;

	//TODO: Currently ints for simple multiplication in prove_insolvable... CHANGE TO BOOLS
	//and provide a casting method? 
	Vector<int> startStateVec;
	Vector<int> endStateVec;
};

