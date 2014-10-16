#include "Solitaire.h"


Solitaire::Solitaire(bool Eng)
{
	if (Eng) boardShape = CreateEngBoard();
	else boardShape = CreateEurBoard();

	indexMatrix = CreateIndexMat(*boardShape);
	stateVector = CreateStateVec(*boardShape);
	jumpMatrix = CreateJumpMat(*boardShape, *indexMatrix, *stateVector);

	/*std::cout << *indexMatrix;
	std::cout << *stateVector << std::endl;
	std::cout << *jumpMatrix << std::endl;*/
}

Solitaire::Solitaire(Matrix<bool>& boardShape){
	//Perform deep copy
	this->boardShape = new Matrix<bool>(boardShape);

	indexMatrix = CreateIndexMat(*this->boardShape);
	stateVector = CreateStateVec(*this->boardShape);

	jumpMatrix = CreateJumpMat(*this->boardShape, *this->indexMatrix, *this->stateVector);

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

	for (int i=0; i<temp->get_x_dim(); ++i){
		for (int j=0; j<temp->get_y_dim(); ++j){
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
	Matrix<int>* temp = new Matrix<int>(boardShape.get_x_dim(), boardShape.get_y_dim(), -1);

	int count = 0;

	for (int i=0; i<boardShape.get_x_dim(); ++i){
		for (int j=0; j<boardShape.get_y_dim(); ++j){
			if (boardShape[i][j]){
				(*temp)[i][j] = count++;
			}
		}
	}
	return temp;
}


Vector<bool>* Solitaire::CreateStateVec(const Matrix<bool>& boardShape){
	int count = 0;

	for (int i=0; i<boardShape.get_x_dim(); ++i){
		for (int j=0; j<boardShape.get_y_dim(); ++j){
			if (boardShape[i][j]) ++count;
		}
	}

	Vector<bool>* temp = new Vector<bool>(count, true);

	//TODO: Figure out what to do about this.
	(*temp)[count/2] = false;

	return temp;
}


Matrix<int>* Solitaire::CreateJumpMat(const Matrix<bool>& boardShape, const Matrix<int>& indexMatrix, const Vector<bool>& stateVector){

	//Create a vector to store all the jumps possible on the board
	std::vector<jump> jumps = std::vector<jump>();
	jumps.reserve(stateVector.size() * 4);

	//Go through the board shape looking for possible jumps
	for (int i=0; i<boardShape.get_x_dim(); ++i){
		for (int j=0; j<boardShape.get_y_dim(); ++j){
			//If the location exists
			if (boardShape[i][j]){
				//Check jump right possible
				if (j+2<boardShape.get_y_dim())
					if (boardShape[i][j+1] && boardShape[i][j+2]){
						//If so, push back jump right, and jump left from landed spot
						//TODO: Dont add all to vector, just flip when setting matrix
						jumps.push_back(jump(indexMatrix[i][j], indexMatrix[i][j+1], indexMatrix[i][j+2]));
						//jumps.push_back(jump(indexMatrix[i][j+2], indexMatrix[i][j+1], indexMatrix[i][j]));
					}

					//Check jump down possible
					if (i+2<boardShape.get_x_dim())
						if (boardShape[i+1][j] && boardShape[i+2][j]){
							//If so, push back jump down, and jump up from landed spot
							jumps.push_back(jump(indexMatrix[i][j], indexMatrix[i+1][j], indexMatrix[i+2][j]));
							//jumps.push_back(jump(indexMatrix[i+2][j], indexMatrix[i+1][j], indexMatrix[i][j]));
						}
			}
		}
	}

	//Step 2: Fill matrix with values dependent on indexMatrix and boardShape
	Matrix<int>* jumpMat = new Matrix<int>(stateVector.size(), jumps.size()*2);

	//TODO: Inefficient, work out whether format is important or not.
	//		currently accessed in column major order... very slow!
	int location = -1;
	for (unsigned int i=0; i<jumps.size(); ++i){

		//Jump from start to end
		//1 if jump i removes peg at hole
		(*jumpMat)[jumps[i].startIndex][++location]++;
		(*jumpMat)[jumps[i].midIndex][location]++;

		//-1 if jump i places peg at hole
		(*jumpMat)[jumps[i].endIndex][location]--;

		////Jump from end to start
		////And reverse!
		(*jumpMat)[jumps[i].endIndex][++location]++;
		(*jumpMat)[jumps[i].midIndex][location]++;

		(*jumpMat)[jumps[i].startIndex][location]--;

	}

	return jumpMat;
}

void Solitaire::PrintBoard(std::ostream& os) const{
	for (int i=0; i<indexMatrix->get_x_dim(); ++i){
		for (int j=0; j<indexMatrix->get_y_dim(); ++j){
			if ((*boardShape)[i][j]) os << (*stateVector)[(*indexMatrix)[i][j]];
			else os << " ";
			os << ",";
		}
		os << "\n";
	}
}