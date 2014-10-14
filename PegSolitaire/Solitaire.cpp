#include "Solitaire.h"


Solitaire::Solitaire(bool Eng)
{
	if (Eng) boardShape = CreateEngBoard();
	else boardShape = CreateEurBoard();

	indexMatrix = CreateIndexMat(*boardShape);
	stateVector = CreateStateVec(*indexMatrix);


	std::cout << *indexMatrix;
	std::cout << *stateVector << std::endl;
}


Solitaire::~Solitaire(void)
{
	delete boardShape;
	delete indexMatrix;
	delete stateVector;
}

Matrix<bool>* Solitaire::CreateEngBoard(){
	Matrix<bool>* temp = new Matrix<bool>(7, 7, 0);

	for (int i=0; i<temp->getXSize(); ++i){
		for (int j=0; j<temp->getYSize(); ++j){
			if (!( ((i<2) || (i>4)) && ((j<2) || (j>4)) ))
				(*temp)[i][j] = true;
		}
	}

	return temp;
}

Matrix<bool>* Solitaire::CreateEurBoard(){
	Matrix<bool>* temp = CreateEngBoard();

	(*temp)[1][1] = true;
	(*temp)[1][5] = true;
	(*temp)[5][1] = true;
	(*temp)[5][5] = true;

	return temp;
}

Matrix<int>* Solitaire::CreateIndexMat(const Matrix<bool>& boardShape){
	Matrix<int>* temp = new Matrix<int>(boardShape.getXSize(), boardShape.getYSize(), -1);

	int count = 0;

	for (int i=0; i<boardShape.getXSize(); ++i){
		for (int j=0; j<boardShape.getYSize(); ++j){
			if (boardShape[i][j]){
				(*temp)[i][j] = count;
				++count;
			}
		}
	}
	return temp;
}

Vector<bool>* Solitaire::CreateStateVec(const Matrix<int>& indexMatrix){
	int count = 0;

	for (int i=0; i<indexMatrix.getXSize(); ++i){
		for (int j=0; j<indexMatrix.getYSize(); ++j){
			if (indexMatrix[i][j] >= 0) ++count;
		}
	}

	Vector<bool>* temp = new Vector<bool>(count, true);
	std::cout << count/2 << std::endl;
	(*temp)[count/2] = false;

	return temp;
}

void Solitaire::PrintBoard(std::ostream& os) const{
	for (int i=0; i<indexMatrix->getXSize(); ++i){
		for (int j=0; j<indexMatrix->getYSize(); ++j){
			if ((*boardShape)[i][j]) os << (*stateVector)[(*indexMatrix)[i][j]];
			else os << ",";
			os << " ";
		}
		os << "\n";
	}
}