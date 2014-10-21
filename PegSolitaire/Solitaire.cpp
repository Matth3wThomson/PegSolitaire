#include "Solitaire.h"

//TODO: Tidy up
Solitaire::Solitaire(bool Eng)
{
	if (Eng) boardShape = CreateEngBoard();
	else boardShape = CreateEurBoard();

	indexMatrix = CreateIndexMat(boardShape);
	stateVector = CreateStateVec(boardShape);
	sparseJumpMat = YaleMatrix<int>(CreateJumpMat(boardShape, indexMatrix, stateVector.size()));

	
	/*sparseJumpMat = YaleMatrix<int>(jumpMatrix);*/

}

Solitaire::Solitaire(Matrix<bool>& boardShape){
	//Perform deep copy
	this->boardShape = boardShape;

	indexMatrix = CreateIndexMat(this->boardShape);
	stateVector = CreateStateVec(this->boardShape);

	sparseJumpMat = YaleMatrix<int>(CreateJumpMat(boardShape, indexMatrix, stateVector.size()));
	/*jumpMatrix = CreateJumpMat(this->boardShape, this->indexMatrix, this->stateVector);
	sparseJumpMat = YaleMatrix<int>(jumpMatrix);*/
}

Solitaire::~Solitaire(void)
{

}

Matrix<bool> Solitaire::CreateEngBoard(){
	Matrix<bool> temp = Matrix<bool>(7, 7);

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
	Matrix<int> temp = Matrix<int>(boardShape.get_x_dim(), boardShape.get_y_dim());

	int count = 0;

	for (int i=0; i<boardShape.get_x_dim(); ++i){
		for (int j=0; j<boardShape.get_y_dim(); ++j){
			temp[i][j] = -1;
			if (boardShape[i][j]){
				temp[i][j] = count++;
			}
		}
	}

	return temp;
}

//TODO: Have not used -1 to signify no peg hole.
int Solitaire::CreateIndexMat(Matrix<int>& indexMatrix, const Matrix<bool>& boardShape){
	indexMatrix = Matrix<int>(boardShape.get_x_dim(), boardShape.get_y_dim());

	int count = 0;

	for (int i=0; i<boardShape.get_x_dim(); ++i)
		for (int j=0; j<boardShape.get_y_dim(); ++j)
			if (boardShape[i][j]){
				indexMatrix[i][j] = count++;
			}

	return count;
}

Vector<bool> Solitaire::CreateStateVec(const Matrix<bool>& boardShape){
	int count = 0;

	for (int i=0; i<boardShape.get_x_dim(); ++i){
		for (int j=0; j<boardShape.get_y_dim(); ++j){
			if (boardShape[i][j]) ++count;
		}
	}

	Vector<bool> temp = Vector<bool>(count);

	for (int i=0; i<temp.size(); ++i)
		temp[i] = true;

	//TODO: Figure out what to do about this.
	temp[count/2] = false;

	return temp;
}

int Solitaire::create_index_and_state(const Matrix<bool>& boardShape, Matrix<int>& indexMat, Vector<bool>& stateVector){
	int count = 0;

	indexMat = Matrix<int>(boardShape.get_x_dim(), boardShape.get_y_dim());

	for (int i=0; i<boardShape.get_x_dim(); ++i)
		for (int j=0; j<boardShape.get_y_dim(); ++j)
			if (boardShape[i][j])
				indexMat[i][j] = count++;

	stateVector = Vector<bool>(count);

	return count;
			
}


Matrix<int> Solitaire::CreateJumpMat(const Matrix<bool>& boardShape, const Matrix<int>& indexMatrix, const int stateVectorLength){

	//Create a vector to store all the jumps possible on the board
	std::vector<int> jumpIndices = std::vector<int>();

	//Up to 4 * locations worth of jumps
	//3 ints per 2 jumps = 1.5
	// 4 * 1.5  = 6
	jumpIndices.reserve(stateVectorLength * 6);

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
	Matrix<int> jumpMat = Matrix<int>(jumpIndices.size()/3*2, stateVectorLength);
	//Matrix<int> jumpMat = Matrix<int>(stateVector.size(), jumpIndices.size()/3*2);

	//TODO: Inefficient, work out whether format is important or not.
	//		currently accessed in column major order... very slow!
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
	for (int i=0; i<boardShape.get_x_dim(); ++i)
		for (int j=0; j<boardShape.get_y_dim(); ++j)
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
						if (j+2<boardShape.get_y_dim())
							if (indexMatrix[i][j+1] > -1 && indexMatrix[i][j+2] > -1)
								if (stateVector[indexMatrix[i][j+1]] && !stateVector[indexMatrix[i][j+2]])
									return false;

						//See if jump up is possible
						if (i>2)
							if (indexMatrix[i-1][j] > -1 && indexMatrix[i-2][j] > -1)
								if (stateVector[indexMatrix[i-1][j]] && !stateVector[indexMatrix[i-2][j]])
									return false;

						//See if jump down is possible
						if (i+2<boardShape.get_x_dim())
							if (indexMatrix[i+1][j] > -1 && indexMatrix[i+2][j] > -1)
								if (stateVector[indexMatrix[i+1][j]] && !stateVector[indexMatrix[i+2][j]])
									return false;

					}
				}
				//No possible moves found, it must be the end of the game!
				return true;
}

//VERY LONG FOR LITTLE CODE!
bool Solitaire::perform_move(int x, int y, char direction){

	//Check dimensions are correct
	if (x < 0 || x>= boardShape.get_x_dim() || y < 0 || y >= boardShape.get_y_dim()) return false;

	int xdir = 0, ydir = 0;

	switch (direction){
	case 'U':
	case 'u':
		if (x>2)
			--xdir;
		/*if (boardShape[x][y] && boardShape[x-1][y] && boardShape[x-2][y])
		if (stateVector[indexMatrix[x][y]] && stateVector[indexMatrix[x-1][y]] && !stateVector[indexMatrix[x-2][y]]){
		stateVector[indexMatrix[x][y]] = false;
		stateVector[indexMatrix[x-1][y]] = false;
		stateVector[indexMatrix[x-2][y]] = true;
		return true;
		}*/
		break;
	case 'D':
	case 'd':
		if (x+2 < boardShape.get_x_dim())
			++xdir;
		/*if (boardShape[x][y] && boardShape[x+1][y] && boardShape[x+2][y])
		if (stateVector[indexMatrix[x][y]] && stateVector[indexMatrix[x+1][y]] && !stateVector[indexMatrix[x+2][y]]){
		stateVector[indexMatrix[x][y]] = false;
		stateVector[indexMatrix[x+1][y]] = false;
		stateVector[indexMatrix[x+2][y]] = true;
		return true;
		}*/

		break;
	case 'L':
	case 'l':
		if (y>2)
			--ydir;
		/*if (boardShape[x][y] && boardShape[x][y-1] && boardShape[x][y-2])
		if (stateVector[indexMatrix[x][y]] && stateVector[indexMatrix[x][y-1]] && !stateVector[indexMatrix[x][y-2]]){
		stateVector[indexMatrix[x][y]] = false;
		stateVector[indexMatrix[x][y-1]] = false;
		stateVector[indexMatrix[x][y-2]] = true;
		return true;
		}*/

		break;
	case 'R':
	case 'r':
		if (y+2 < boardShape.get_y_dim())
			++ydir;
		/*if (boardShape[x][y] && boardShape[x][y+1] && boardShape[x][y+2])
		if (stateVector[indexMatrix[x][y]] && stateVector[indexMatrix[x][y+1]] && !stateVector[indexMatrix[x][y+2]]){
		stateVector[indexMatrix[x][y]] = false;
		stateVector[indexMatrix[x][y+1]] = false;
		stateVector[indexMatrix[x][y+2]] = true;
		return true;
		}*/

		break;
	}

	//A movement has been found!
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

void Solitaire::play_game(){
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

		}while (!this->perform_move(x, y, dir));

		system("cls");
		std::cout << "Move made! " << std::endl;
	}

	std::cout << "Game over! " << std::endl;
}

void Solitaire::PrintBoard(std::ostream& os) const{

	os << "+---+";
	for (int j=0; j<indexMatrix.get_y_dim(); ++j)
		os << "---+";
	os << std::endl;

	os << "|X\\Y|";
	for (int i=0; i<indexMatrix.get_y_dim(); ++i)
		os << " " << i << " |";

	os << std::endl;

	for (int i=0; i<indexMatrix.get_x_dim(); ++i){

		os << "+---+";
		for (int j=0; j<indexMatrix.get_y_dim(); ++j)
			os << "---+";

		os << std::endl;
		os << "| " << i << " |";

		for (int j=0; j<indexMatrix.get_y_dim(); ++j){
			os << " ";
			if (boardShape[i][j]) os << ((stateVector[indexMatrix[i][j]]) ? "X" : "O");
			else os << " ";
			os << " |";
		}
		os << std::endl;
	}

	os << "+---+";
	for (int j=0; j<indexMatrix.get_y_dim(); ++j)
		os << "---+";
	os << std::endl;
}

//Spare code for drawing according to file spec
//for (int i=0; i<indexMatrix.get_x_dim(); ++i){
//		for (int j=0; j<indexMatrix.get_y_dim(); ++j){
//			if (boardShape[i][j]) os << stateVector[indexMatrix[i][j]];
//			else os << " ";
//			os << ",";
//		}
//		os << "\n";