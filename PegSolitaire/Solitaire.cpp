#include "Solitaire.h"


Solitaire::Solitaire(bool Eng)
{
	if (Eng) boardShape = CreateEngBoard();
	else boardShape = CreateEurBoard();

	//TODO: Check this is working as expected (MOVE SEMANTICS)
	indexMatrix = CreateIndexMat(boardShape);
	stateVector = CreateStateVec(boardShape);
	jumpMatrix = CreateJumpMat(boardShape, indexMatrix, stateVector);

	std::cout << indexMatrix;

}

Solitaire::Solitaire(Matrix<bool>& boardShape){
	//Perform deep copy
	this->boardShape = boardShape;

	indexMatrix = CreateIndexMat(this->boardShape);
	stateVector = CreateStateVec(this->boardShape);

	jumpMatrix = CreateJumpMat(this->boardShape, this->indexMatrix, this->stateVector);
}

Solitaire::~Solitaire(void)
{
	
}

Matrix<bool> Solitaire::CreateEngBoard(){
	Matrix<bool> temp = Matrix<bool>(7, 7, 0);

	for (int i=0; i<temp.get_x_dim(); ++i){
		for (int j=0; j<temp.get_y_dim(); ++j){
			if (!( ((i<2) || (i>4)) && ((j<2) || (j>4)) ))
				temp[i][j] = true;
		}
	}

	return temp;
}

Matrix<bool> Solitaire::CreateEurBoard(){
	Matrix<bool> temp = CreateEngBoard();

	temp[1][1] = true;
	temp[1][5] = true;
	temp[5][1] = true;
	temp[5][5] = true;

	return temp;
}

Matrix<int> Solitaire::CreateIndexMat(const Matrix<bool>& boardShape){
	Matrix<int> temp = Matrix<int>(boardShape.get_x_dim(), boardShape.get_y_dim(), -1);

	int count = 0;

	for (int i=0; i<boardShape.get_x_dim(); ++i){
		for (int j=0; j<boardShape.get_y_dim(); ++j){
			if (boardShape[i][j]){
				temp[i][j] = count++;
			}
		}
	}
	return temp;
}


Vector<bool> Solitaire::CreateStateVec(const Matrix<bool>& boardShape){
	int count = 0;

	for (int i=0; i<boardShape.get_x_dim(); ++i){
		for (int j=0; j<boardShape.get_y_dim(); ++j){
			if (boardShape[i][j]) ++count;
		}
	}

	Vector<bool> temp = Vector<bool>(count, true);

	//TODO: Figure out what to do about this.
	temp[count/2] = false;

	return temp;
}


Matrix<int> Solitaire::CreateJumpMat(const Matrix<bool>& boardShape, const Matrix<int>& indexMatrix, const Vector<bool>& stateVector){

	//Create a vector to store all the jumps possible on the board
	std::vector<int> jumpIndices = std::vector<int>();

	//Up to 4 * locations worth of jumps
	//3 ints per 2 jumps = 1.5
	// 4 * 1.5  = 6
	jumpIndices.reserve(stateVector.size() * 6);

	//Go through the board shape looking for possible jumps
	for (int i=0; i<boardShape.get_x_dim(); ++i){
		for (int j=0; j<boardShape.get_y_dim(); ++j){
			//If the location exists
			if (boardShape[i][j]){
				//Check jump right possible
				if (j+2<boardShape.get_y_dim())
					if (boardShape[i][j+1] && boardShape[i][j+2]){
						//If so, push back jump right, and jump left from landed spot
						jumpIndices.push_back(indexMatrix[i][j]);
						jumpIndices.push_back(indexMatrix[i][j+1]);
						jumpIndices.push_back(indexMatrix[i][j+2]);
					}

					//Check jump down possible
					if (i+2<boardShape.get_x_dim())
						if (boardShape[i+1][j] && boardShape[i+2][j]){
							//If so, push back jump down, and jump up from landed spot
							jumpIndices.push_back(indexMatrix[i][j]);
							jumpIndices.push_back(indexMatrix[i+1][j]);
							jumpIndices.push_back(indexMatrix[i+2][j]);

						}
			}
		}
	}

	//Step 2: Fill matrix with values dependent on indexMatrix and boardShape
	Matrix<int> jumpMat = Matrix<int>(stateVector.size(), jumpIndices.size()/3*2);

	//TODO: Inefficient, work out whether format is important or not.
	//		currently accessed in column major order... very slow!
	int location = -1;
	for (unsigned int i=0; i<jumpIndices.size(); i+=3){

		//Jump from start to end
		//1 if jump removes peg at hole (jumpIndices[i])
		jumpMat[jumpIndices[i]][++location]++;
		jumpMat[jumpIndices[i+1]][location]++;

		//-1 if jump places peg at hole jumpIndices[i]
		jumpMat[jumpIndices[i+2]][location]--;

		////Jump from end to start
		////And reverse!
		jumpMat[jumpIndices[i+2]][++location]++;
		jumpMat[jumpIndices[i+1]][location]++;

		jumpMat[jumpIndices[i]][location]--;

	}

	return jumpMat;
}

//Calculate whether the game has ended
bool Solitaire::EOG(){
	for (int i=0; i<boardShape.get_x_dim(); ++i)
		for (int j=0; j<boardShape.get_y_dim(); ++j)
			if ((boardShape)[i][j]){
				if ((stateVector)[(indexMatrix)[i][j]]){
					
				}
			}
			return true;
}

void Solitaire::PrintBoard(std::ostream& os) const{
	for (int i=0; i<indexMatrix.get_x_dim(); ++i){
		for (int j=0; j<indexMatrix.get_y_dim(); ++j){
			if (boardShape[i][j]) os << stateVector[indexMatrix[i][j]];
			else os << " ";
			os << ",";
		}
		os << "\n";
	}
}