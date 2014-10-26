#include "Solitaire.h"


Solitaire::Solitaire(bool Eng)
{
	if (Eng) boardShape = createEngBoard();
	else boardShape = createEurBoard();

	indexMatrix = createIndexMat(boardShape);
	stateVector = createStateVec(boardShape);
	sparseJumpMat = YaleMatrix<int>(createJumpMat(boardShape, indexMatrix, stateVector.size()));

	

}

Solitaire::Solitaire(Matrix<bool>& boardShape){
	
	this->boardShape = boardShape;

	indexMatrix = createIndexMat(this->boardShape);
	stateVector = createStateVec(this->boardShape);

	sparseJumpMat = YaleMatrix<int>(createJumpMat(boardShape, indexMatrix, stateVector.size()));
	
}

Solitaire::~Solitaire(void)
{

}

//Creates a english board
Matrix<bool> Solitaire::createEngBoard(){
	Matrix<bool> temp = Matrix<bool>(7, 7);

	for (int i=0; i<temp.getXDim(); ++i){
		for (int j=0; j<temp.getYDim(); ++j){
			if (!( ((i<2) || (i>4)) && ((j<2) || (j>4)) ))
				temp[i][j] = true;
		}
	}

	return temp;
}

//Creates a european board
Matrix<bool> Solitaire::createEurBoard(){
	Matrix<bool> temp = createEngBoard();

	temp[1][1] = true;
	temp[1][5] = true;
	temp[5][1] = true;
	temp[5][5] = true;

	return temp;
}

//Creates and returns an index matrix given a supplied boardshape
Matrix<int> Solitaire::createIndexMat(const Matrix<bool>& boardShape){
	Matrix<int> temp = Matrix<int>(boardShape.getXDim(), boardShape.getYDim());

	int count = 0;

	for (int i=0; i<boardShape.getXDim(); ++i){
		for (int j=0; j<boardShape.getYDim(); ++j){
			temp[i][j] = -1;
			if (boardShape[i][j]){
				temp[i][j] = count++;
			}
		}
	}

	return temp;
}

//Creates an index matrix passed by reference from a given board shape returning the number of peg holes
// in the board
int Solitaire::createIndexMat(Matrix<int>& indexMatrix, const Matrix<bool>& boardShape){
	indexMatrix = Matrix<int>(boardShape.getXDim(), boardShape.getYDim());

	int count = 0;

	for (int i=0; i<boardShape.getXDim(); ++i)
		for (int j=0; j<boardShape.getYDim(); ++j)
			if (boardShape[i][j]){
				indexMatrix[i][j] = count++;
			}

	return count;
}

//Creates a state vector given a board shape (useful only for typical solitaire usage)
Vector<bool> Solitaire::createStateVec(const Matrix<bool>& boardShape){
	int count = 0;

	for (int i=0; i<boardShape.getXDim(); ++i){
		for (int j=0; j<boardShape.getYDim(); ++j){
			if (boardShape[i][j]) ++count;
		}
	}

	Vector<bool> temp = Vector<bool>(count);

	for (int i=0; i<temp.size(); ++i)
		temp[i] = true;

	temp[count/2] = false;

	return temp;
}

//Creates an index matrix and a state vector passed by reference in one function. Quicker than using the
//individual methods as this only has to count once. Returns the number of peg holes.
int Solitaire::createIndexAndState(const Matrix<bool>& boardShape, Matrix<int>& indexMat, Vector<bool>& stateVector){
	int count = 0;

	indexMat = Matrix<int>(boardShape.getXDim(), boardShape.getYDim());

	for (int i=0; i<boardShape.getXDim(); ++i)
		for (int j=0; j<boardShape.getYDim(); ++j)
			if (boardShape[i][j])
				indexMat[i][j] = count++;

	stateVector = Vector<bool>(count);

	return count;
			
}

//Creates a jump matrix based on a given board shape index matrix and number of pegs
Matrix<int> Solitaire::createJumpMat(const Matrix<bool>& boardShape, const Matrix<int>& indexMatrix, const int stateVectorLength){

	//Create a vector to store all the jumps possible on the board
	std::vector<int> jumpIndices = std::vector<int>();

	//Up to 4 * locations worth of jumps
	//3 ints per 2 jumps = 1.5
	// 4 * 1.5  = 6 MAXIMUM space required for indices vector
	jumpIndices.reserve(stateVectorLength * 6);

	//Go through the board shape looking for possible jumps
	for (int i=0; i<boardShape.getXDim(); ++i){
		for (int j=0; j<boardShape.getYDim(); ++j){
			//If the location exists
			if (boardShape[i][j]){
				//Check jump right possible
				if (j+2<boardShape.getYDim())
					if (boardShape[i][j+1] && boardShape[i][j+2]){
						//If so, push back jump right, and jump left from landed spot
						jumpIndices.push_back(indexMatrix[i][j]);
						jumpIndices.push_back(indexMatrix[i][j+1]);
						jumpIndices.push_back(indexMatrix[i][j+2]);
					}

					//Check jump down possible
					if (i+2<boardShape.getXDim())
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
	Matrix<int> jumpMat = Matrix<int>(jumpIndices.size()/3*2, stateVectorLength);
	//Matrix<int> jumpMat = Matrix<int>(stateVector.size(), jumpIndices.size()/3*2);

	//Jump matrix is stored as the transpose of the coursework document due to efficiency
	//being better this way. (Row major access faster than column major)
	int location = -1;
	for (unsigned int i=0; i<jumpIndices.size(); i+=3){

		//Jump from start to end
		//1 if jump removes peg at hole (jumpIndices[i])
		jumpMat[++location][jumpIndices[i]]++;
		jumpMat[location][jumpIndices[i+1]]++;

		//-1 if jump places peg at hole jumpIndices[i]
		jumpMat[location][jumpIndices[i+2]]--;

		////Jump from end to start
		////And reverse!
		jumpMat[++location][jumpIndices[i+2]]++;
		jumpMat[location][jumpIndices[i+1]]++;

		jumpMat[location][jumpIndices[i]]--;
		

	}

	return jumpMat;
}

//Calculate whether the game has ended
bool Solitaire::EOG(){
	//For every possible location on the board
	for (int i=0; i<boardShape.getXDim(); ++i)
		for (int j=0; j<boardShape.getYDim(); ++j)
			//Check if the board has a peg hole there
				if (boardShape[i][j]){
					//Check if there is a peg in the hole
					if (stateVector[indexMatrix[i][j]]){
						//See if jump left is possible
						//Make sure you're not jumping off the matrix
						if (j>2)
							//Make sure you're not jumping off the board
								if (indexMatrix[i][j-1] > -1 && indexMatrix[i][j-2] > -1)
									//See if the required pegs for a jump are present (jump over adjacent, into adjacent+1)
										if (stateVector[indexMatrix[i][j-1]] && !stateVector[indexMatrix[i][j-2]])
											return false; //If so, the game isn't over yet!

						//See if jump right is possible
						if (j+2<boardShape.getYDim())
							if (indexMatrix[i][j+1] > -1 && indexMatrix[i][j+2] > -1)
								if (stateVector[indexMatrix[i][j+1]] && !stateVector[indexMatrix[i][j+2]])
									return false;

						//See if jump up is possible
						if (i>2)
							if (indexMatrix[i-1][j] > -1 && indexMatrix[i-2][j] > -1)
								if (stateVector[indexMatrix[i-1][j]] && !stateVector[indexMatrix[i-2][j]])
									return false;

						//See if jump down is possible
						if (i+2<boardShape.getXDim())
							if (indexMatrix[i+1][j] > -1 && indexMatrix[i+2][j] > -1)
								if (stateVector[indexMatrix[i+1][j]] && !stateVector[indexMatrix[i+2][j]])
									return false;

					}
				}
				//No possible moves found, it must be the end of the game!
				return true;
}

//Attempts to perform a move defined by a start point and a direction to move
bool Solitaire::performMove(int x, int y, char direction){

	//Check dimensions are correct
	if (x < 0 || x>= boardShape.getXDim() || y < 0 || y >= boardShape.getYDim()) return false;

	int xdir = 0, ydir = 0;

	switch (direction){
	case 'U':
	case 'u':
		if (x>2)
			--xdir;
		break;
	case 'D':
	case 'd':
		if (x+2 < boardShape.getXDim())
			++xdir;
		break;
	case 'L':
	case 'l':
		if (y>2)
			--ydir;
		break;
	case 'R':
	case 'r':
		if (y+2 < boardShape.getYDim())
			++ydir;
		break;
	}

	//A movement has been found
	if (xdir != 0 || ydir != 0){
		if (boardShape[x][y] && boardShape[x+xdir][y+ydir] && boardShape[x+2*xdir][y+2*ydir])
			if (stateVector[indexMatrix[x][y]] && stateVector[indexMatrix[x+xdir][y+ydir]] && !stateVector[indexMatrix[x+2*xdir][y+2*ydir]]){
				stateVector[indexMatrix[x][y]] = false;
				stateVector[indexMatrix[x+xdir][y+ydir]] = false;
				stateVector[indexMatrix[x+2*xdir][y+2*ydir]] = true;
				return true;
			}
	}
	return false;
}

//Nice interface and encapsulation for playing a game of solitaire
void Solitaire::playGame(){
	while (!EOG()){
		std::cout << *this << std::endl;

		int x, y;
		char dir;

		do {
			std::cout << "Please enter an X coordinate: ";
			std::cin >> x;

			std::cout << "Please enter a Y coordinate: ";
			std::cin >> y;

			std::cout << "Please enter a direction; (U)p, (D)own, (L)eft, (R)ight: ";
			std::cin >> dir;

		}while (!this->performMove(x, y, dir));

		system("cls");
		std::cout << "Move made! " << std::endl;
	}

	std::cout << *this << std::endl;
	std::cout << "Game over! " << std::endl;
}

//Prints the board to a given outstream
void Solitaire::PrintBoard(std::ostream& os) const{

	os << "+---+";
	for (int j=0; j<indexMatrix.getYDim(); ++j)
		os << "---+";
	os << std::endl;

	os << "|X\\Y|";
	for (int i=0; i<indexMatrix.getYDim(); ++i)
		os << " " << i << " |";

	os << std::endl;

	for (int i=0; i<indexMatrix.getXDim(); ++i){

		os << "+---+";
		for (int j=0; j<indexMatrix.getYDim(); ++j)
			os << "---+";

		os << std::endl;
		os << "| " << i << " |";

		for (int j=0; j<indexMatrix.getYDim(); ++j){
			os << " ";
			if (boardShape[i][j]) os << ((stateVector[indexMatrix[i][j]]) ? "X" : "O");
			else os << " ";
			os << " |";
		}
		os << std::endl;
	}

	os << "+---+";
	for (int j=0; j<indexMatrix.getYDim(); ++j)
		os << "---+";
	os << std::endl;
}