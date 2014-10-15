#include "Solitaire.h"


Solitaire::Solitaire(bool Eng)
{
	if (Eng) boardShape = CreateEngBoard();
	else boardShape = CreateEurBoard();

	indexMatrix = CreateIndexMat(*boardShape);
	stateVector = CreateStateVec(*indexMatrix);
	jumpMatrix = CreateJumpMat(*boardShape, *indexMatrix, *stateVector);

	std::cout << *indexMatrix;
	std::cout << *stateVector << std::endl;
	std::cout << *jumpMatrix << std::endl;
}


Solitaire::~Solitaire(void)
{
	delete boardShape;
	delete indexMatrix;
	delete stateVector;
	delete jumpMatrix;
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
				(*temp)[i][j] = count++;
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


Matrix<int>* Solitaire::CreateJumpMat(const Matrix<bool>& boardShape, const Matrix<int>& indexMatrix, const Vector<bool>& stateVector){
	
	//Create a vector to store all the jumps possible on the board
	std::vector<jump> jumps = std::vector<jump>();
	jumps.reserve(stateVector.size() * 4);

	//Go through the board shape looking for possible jumps
	for (int i=0; i<boardShape.getXSize(); ++i){
		for (int j=0; j<boardShape.getYSize(); ++j){
			//If the location exists
			if (boardShape[i][j]){
				//Check jump right possible
				if (j+2<boardShape.getYSize())
				if (boardShape[i][j+1] && boardShape[i][j+2])
					//TODO: Not sure if this works?
					jumps.push_back(jump(indexMatrix[i][j], indexMatrix[i][j+1], indexMatrix[i][j+2]));

				//Check jump left possible
				if (j-2>-1)
				if (boardShape[i][j-1] && boardShape[i][j-2])
					jumps.push_back(jump(indexMatrix[i][j], indexMatrix[i][j-1], indexMatrix[i][j-2]));
				
				//Check jump down possible
				if (i+2<boardShape.getXSize())
				if (boardShape[i+1][j] && boardShape[i+2][j])
					jumps.push_back(jump(indexMatrix[i][j], indexMatrix[i+1][j], indexMatrix[i+2][j]));

				//Check jump up possible
				if (i-2>-1)
				if (boardShape[i-1][j] && boardShape[i-2][j])
					jumps.push_back(jump(indexMatrix[i][j], indexMatrix[i-1][j], indexMatrix[i-2][j]));
				
			}
		}
	}

	//Step 2: Fill matrix with values dependent on indexMatrix and boardShape
	Matrix<int>* jumpMat = new Matrix<int>(stateVector.size(), jumps.size());

	for (unsigned int i=0; i<jumps.size(); ++i){

		//1 if jump i removes peg at hole
		(*jumpMat)[jumps[i].startIndex][i]++;
		(*jumpMat)[jumps[i].midIndex][i]++;

		//-1 if jump i places peg at hole
		(*jumpMat)[jumps[i].endIndex][i]--;

	}

	std::cout << *jumpMat;

	return jumpMat;
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