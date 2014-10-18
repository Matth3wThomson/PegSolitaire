#pragma once

#include "Vector.h"
#include "YaleMatrix.h"
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

	QUESTIONS:
		-Does the orientation of the jump matrix matter? When creating, column by 
		 row is very slow. MAKE COMMENT
		-Is there an equation to know the maximum size of a jump matrix (no of jumps)? ASK LEFT
		-Is using uncaught exeptions in matrix classes a good idea (non uniform matrix addition)?
			-Vectors of different sizes?
		-What mathematical functions are necessary for our matrix classes PRIORITIZE
			-Complete class for better functionality?
			-Just what we need for smaller/more efficient codebase?
		-When initializing your state vector, should you set values?
		-Passing by reference to static functions good practice? will be quicker than individual
		 returning pointers. 
		-For the game, is coordinates necessary, as indices are nicer...
*/

class Solitaire
{
public:

	Solitaire(bool Eng = 1);
	Solitaire(Matrix<bool>& boardShape);
	~Solitaire(void);
	
	//Creates given boards on the heap
	static Matrix<bool> CreateEurBoard();
	static Matrix<bool> CreateEngBoard();
	
	static Matrix<int> CreateIndexMat(const Matrix<bool>& boardShape);
	static Vector<bool> CreateStateVec(const Matrix<bool>& boardShape);

	static Matrix<int> CreateJumpMat(const Matrix<bool>& boardShape,
		const Matrix<int>& indexMatrix, const Vector<bool>& stateVector);

	bool EOG();
	bool perform_move(int x, int y, char direction);
	void play_game();

	friend std::ostream& operator<<(std::ostream& os, const Solitaire& s){
		s.PrintBoard(os);
		return os;
	};

private:

	Matrix<bool> boardShape;
	Vector<bool> stateVector;
	Matrix<int> indexMatrix;
	Matrix<int> jumpMatrix;
	YaleMatrix<int> sparseJumpMat;

	/*struct jump {
		int startIndex, midIndex, endIndex;

		jump(int start = 0, int mid = 0, int end = 0):
		startIndex(start), midIndex(mid), endIndex(end){ };
	};*/

	void PrintBoard(std::ostream& os) const;


};

