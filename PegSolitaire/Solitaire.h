#pragma once

#include "Matrix.h"
#include "Vector.h"
#include <vector>

/**
	A class to represent a game of peg solitaire.

	The board will be structured with the top left peg being 0,0.

	TODO:
		-Put matricies onto the stack, rather than heap. Use pass by
		 reference in existing methods to facilitate this change?
		-Having indexMatrix and State Vector computed in the same function
		 will be much quicker
		-Make sure that all method names are of the same style
		-Find out wether use of STL::vector is allowed
		-Jump matrix is calculated irrespective of whether peg holes are filled?
*/

class Solitaire
{
public:
	Solitaire(bool Eng = 1);
	~Solitaire(void);
	
	//Creates given boards on the heap
	static Matrix<bool>* CreateEurBoard();
	static Matrix<bool>* CreateEngBoard();
	
	static Matrix<int>* CreateIndexMat(const Matrix<bool>& boardShape);
	static Vector<bool>* CreateStateVec(const Matrix<int>& indexMatrix);

	static Matrix<int>* CreateJumpMat(const Matrix<bool>& boardShape,
		const Matrix<int>& indexMatrix, const Vector<bool>& stateVector);

	friend std::ostream& operator<<(std::ostream& os, const Solitaire& s){
		s.PrintBoard(os);
		return os;
	};

private:

	Matrix<bool>* boardShape;
	Vector<bool>* stateVector;
	Matrix<int>* indexMatrix;
	Matrix<int>* jumpMatrix;

	struct jump {
		int startIndex, midIndex, endIndex;

		jump(int start = 0, int mid = 0, int end = 0):
		startIndex(start), midIndex(mid), endIndex(end){ };
	};

	void PrintBoard(std::ostream& os) const;


};

