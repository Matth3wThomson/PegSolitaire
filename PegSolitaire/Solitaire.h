#pragma once

#include "Vector.h"
#include "YaleMatrix.h"
#include <vector>

/**
	A class to represent a game of peg solitaire.

	The board will be structured with the top left peg being 0,0.

	NOTES:
		-This class contains a jump matrix simply because the coursework spec requires
		 computation of a jump matrix as part of section 1. It provides no functionaility
		 in this class.
		-Most of the functionality in this class is declared static as it is useful in the
		 pagoda class
*/

class Solitaire
{
public:

	Solitaire(bool Eng = 1);
	Solitaire(Matrix<bool>& boardShape);
	~Solitaire(void);
	
	//Creates given boards
	static Matrix<bool> createEurBoard();
	static Matrix<bool> createEngBoard();
	
	static Matrix<int> createIndexMat(const Matrix<bool>& boardShape);
	static int createIndexMat(Matrix<int>& indexMatrix, const Matrix<bool>& boardShape);
	static Vector<bool> createStateVec(const Matrix<bool>& boardShape);

	//Returns the number of pegs on the board. TODO: Untested
	static int createIndexAndState(const Matrix<bool>& boardShape, Matrix<int>& indexMat,
		Vector<bool>& stateVector);

	static Matrix<int> createJumpMat(const Matrix<bool>& boardShape,
		const Matrix<int>& indexMatrix, const int stateVectorLength);

	bool EOG();
	bool performMove(int x, int y, char direction);
	void playGame();

	friend std::ostream& operator<<(std::ostream& os, const Solitaire& s){
		s.PrintBoard(os);
		return os;
	};

private:

	Matrix<bool> boardShape;
	Vector<bool> stateVector;
	Matrix<int> indexMatrix;
	YaleMatrix<int> sparseJumpMat;

	void PrintBoard(std::ostream& os) const;
};

