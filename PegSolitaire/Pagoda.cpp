#include "Pagoda.h"


Pagoda::Pagoda(bool eng){
	//Create board
	if (eng) board = Solitaire::createEngBoard();
	else board = Solitaire::createEurBoard();

	//Create the index matrix and count number of pegholes
	pegHoles = Solitaire::createIndexMat(indexMat, board);

	//Create the jump matrix
	jumpMat = Solitaire::createJumpMat(board, indexMat, pegHoles);

}

Pagoda::Pagoda(const Matrix<bool>& boardShape)
{
	board = boardShape;

	pegHoles = Solitaire::createIndexMat(indexMat, board);
	jumpMat = Solitaire::createJumpMat(board, indexMat, pegHoles);
}

Pagoda::Pagoda(const Pagoda& rhs){
	board = rhs.board;
	indexMat = rhs.indexMat;
	jumpMat = rhs.jumpMat;

	pagodaFunctions = rhs.pagodaFunctions;
	pegHoles = rhs.pegHoles;
}

Pagoda& Pagoda::operator=(const Pagoda& rhs){
	if (this == &rhs) return *this;

	board = rhs.board;
	indexMat = rhs.indexMat;
	jumpMat = rhs.jumpMat;

	pagodaFunctions = rhs.pagodaFunctions;
	pegHoles = rhs.pegHoles;

	return *this;
}

Pagoda::~Pagoda(void)
{
}


void Pagoda::loadFromFile(const std::string& filename, std::vector<BoardPair>& bps){
	std::ifstream file = std::ifstream();

	file.open(filename.c_str());
	if (!file.is_open()) return throw std::invalid_argument("File not found");

	std::string temp;
	file >> temp;

	//Keep going until we reach the end of the file
	while (!file.eof()){

		//The following must be the definition of a board
		if (temp == "WIDTH"){

			int width, height;

			file >> width;

			file >> temp;

			if (temp == "HEIGHT"){
				file >> height;
			} else throw std::invalid_argument("File format incorrect"); //The format is incorrect!

			board = Matrix<bool>(height, width);
			file >> temp;

			//Loop through information given to load in the board
			for (int i=0; i<height; ++i){
				for (int j=0; j<width; ++j){
					file >> temp;
					if (temp == "T") board[i][j] = true;
					else if (temp == "F") board[i][j] = false;
					else throw std::invalid_argument("File format incorrect"); //The format is incorrect
				}
			}

			pegHoles = Solitaire::createIndexMat(indexMat, board);
			jumpMat = Solitaire::createJumpMat(board, indexMat, pegHoles);

			file >> temp;
		}

		//The following is a definition of a start and end state
		if (temp == "START:"){
			bps.push_back(BoardPair(pegHoles));
			loadVectorFromBoard(file, bps.back().startState);

			file >> temp;
		}

		//The following is a definition of an end state
		if (temp == "END:"){
			loadVectorFromBoard(file, bps.back().endState);

			file >> temp;
		}

		//The following is a definition of a pagoda
		if (temp == "PAGODA:"){
			bps.back().pagoda = Vector<double>(pegHoles);
			bps.back().hasPagoda = true;
			loadVectorFromBoard(file, bps.back().pagoda);

			file >> temp;
		}

	}

	file.close();
}

//Taking a given stream containing a matrix of a vector, this function creates a vecor from it,
//storing it in the one supplied
template<typename E>
void Pagoda::loadVectorFromBoard(std::istream& is, Vector<E>& v){

	if (v.size() != pegHoles) throw std::invalid_argument("Cannot load vector from board. Vector must have correct size");

	char getme;

	for (int i=0; i<board.getXDim(); ++i){
		for (int j=0; j<board.getYDim(); ++j){
			if (board[i][j])
				is >> v[indexMat[i][j]];
			is >> getme;
		}
	}

}

//This function saves all stored unique pagodas to a file supplied
bool Pagoda::savePagodaFunctions(const std::string& filename){

	std::cout << "Saving pagodas found to: '" << filename << "'." << std::endl;

	std::ofstream file;

	file.open(filename);

	if (!file.is_open()) return false;

	for (auto itr = pagodaFunctions.begin(); itr != pagodaFunctions.end(); itr++){
		print_vector_as_board(file, *itr);
		file << std::endl;
	}

	file.close();
	return true;
}

//This function loads all stored pagodas into our set of pagodas
bool Pagoda::loadPagodaFunctions(const std::string& filename){

	std::cout << "Loading pagodas from: '" << filename << "'." << std::endl;
	std::ifstream file;

	file.open(filename);

	if (!file.is_open()) return false;

	while (!file.eof()){
		Vector<double> pagoda(pegHoles);
		loadVectorFromBoard(file, pagoda);

		pagodaFunctions.insert(pagoda);
	}

	file.close();

	return true;
}

//This prints a board pair to file, appending to the end if the boolean append is set.
bool Pagoda::printToFile(const std::string& filename, const BoardPair& bp, bool append){
	std::ofstream file = std::ofstream();

	if (append)
		file.open(filename.c_str(), std::ofstream::app);
	else
		file.open(filename.c_str());

	if (!file.is_open()) return false;

	printBoardPair(file, bp, append);
	file.close();

	return true;
}

//This outputs the pagoda class to a given outstream.
//this only contains board information.
std::ostream& operator<<(std::ostream& os, const Pagoda& p){
	os << "WIDTH " << p.board.getWidth() << std::endl;
	os << "HEIGHT " << p.board.getHeight() << std::endl;
	os << "BOARD: " << std::endl;

	for (int i=0; i<p.board.getXDim(); ++i){
		for (int j=0; j<p.board.getYDim(); ++j)
			(p.board[i][j]) ? os << "T " : os << "F ";

		os << std::endl;
	}

	return os;
}

//This function prints a given board pair to file. If append is set to true, the board is not printed
std::ostream& Pagoda::printBoardPair(std::ostream& os, const Pagoda::BoardPair& bp, const bool append){

	if (!append){
		os << "WIDTH " << this->board.getWidth() << std::endl;
		os << "HEIGHT " << this->board.getHeight() << std::endl;
		os << "BOARD: " << std::endl;

		for (int i=0; i<this->board.getXDim(); ++i){
			for (int j=0; j<this->board.getYDim(); ++j)
				(this->board[i][j]) ? os << "T " : os << "F ";

			os << std::endl;
		}
	}

	os << "START: " << std::endl;
	this->print_vector_as_board(os, bp.startState);

	os << "END: " << std::endl;
	this->print_vector_as_board(os, bp.endState);

	if (bp.hasPagoda){
		os << "PAGODA: " << std::endl;
		this->print_vector_as_board(os, bp.pagoda);
	}

	return os;
}

template<typename E>
//This function prints a vector in the shape of the pagoda classes stored board
void Pagoda::print_vector_as_board(std::ostream& os, const Vector<E>& v){
	for (int i=0; i<this->board.getXDim(); ++i){
		for (int j=0; j<this->board.getYDim(); ++j)
			if (this->board[i][j])
				os << v[this->indexMat[i][j]] << ",";
			else 
				os << " ,";

		os <<std::endl;
	}
}


bool Pagoda::generatePagoda(Vector<double>& pagoda, const Vector<int>& endState, bool saveResults){
	//Only attempt to generate if generation mode is on. (Part of work around for lack of sharable locks)
	if (possibleGeneration){

		//Make the pagoda equal to our endState
		pagoda = Vector<double>(endState);

		Vector<bool> fixedVector(pagoda.size()); //True if fixed, false if not.

		//For each row of the jump matrix
		for (int i=0; i<jumpMat.getXDim(); ++i){

			int jumpIndices[3];
			int index = 0;

			//Find the indices of stateVector which are part of this row's jump
			for (int j=0; j<jumpMat.getYDim(); ++j){
				if (jumpMat.at(i,j)) //If this is an index
					if (jumpMat.at(i,j) == -1)	//If this is the landing spot (Z)
						jumpIndices[2] = j;
					else jumpIndices[index++] = j; //Else this is jumped from or over (X or Y)
			}

			//Jump indices now contains all of the indices into a stateVector where there is a jump present
			if (pagoda[jumpIndices[0]] + pagoda[jumpIndices[1]] < pagoda[jumpIndices[2]]){
				//We have found a jump which does not solve the inequality
				//X + Y >= Z

				//Lambda solveInequality for modifying a property until the inequality is solved
				auto solveInequality = [&] (int indexToMod, bool grow){

					while (pagoda[jumpIndices[0]] + pagoda[jumpIndices[1]] < pagoda[jumpIndices[2]]){
						if (grow) pagoda[jumpIndices[indexToMod]]++;
						else pagoda[jumpIndices[indexToMod]]--;
					}

					//Let the fixed vector know that you have fixed a value
					fixedVector[jumpIndices[indexToMod]] = true;
				};


				if (!fixedVector[jumpIndices[2]]){ //Z value not fixed yet
					solveInequality(2, false); //Fix Z
				}	else if (!fixedVector[jumpIndices[1]]){ //Y value not fixed yet
					solveInequality(1, true); //Fix Y
				}	else if (!fixedVector[jumpIndices[0]]){ //X value not fixed yet
					solveInequality(0, true); //Fix X
				} else {
					return false;
				}
			}
		}

		//After we have finished creating, determine if it is a valid pagoda
		if (verifyPagoda(pagoda, saveResults))
			return true;
	}

	return false;
}

//This function will determine if the pagoda passed is valid. If save results is true
// it will attempt to save the pagoda into the set of known pagodas
bool Pagoda::verifyPagoda(const Vector<double>& pagoda, const bool saveResults){
	
	Vector<double> x = (jumpMat * pagoda);

	for (int i=0; i<x.size(); ++i)
		if (x[i] < 0) return false;

	if (saveResults){
		savePagoda(pagoda);
	}

	return true;
};

void Pagoda::savePagoda(const Vector<double>& pagoda){
	pagodaFuncsMut.lock();
	pagodaFunctions.insert(pagoda); //Locks the pagoda function set to insert a pagoda
	pagodaFuncsMut.unlock();
}

//Determine if a supplied board pair and its partnering pagoda prove insolvability
bool Pagoda::proveInsolvable(const BoardPair& bp){
	if (bp.hasPagoda)
		return (bp.pagoda * (bp.startState - bp.endState) < 0);

	return false;
}

//Attempts to solve the board pair with its stored pagoda (if it has one) and if not
//uses all stored pagodas to solve
bool Pagoda::proveInsolvWithSaved(BoardPair& bp){
	Vector<int> preCalc = bp.startState - bp.endState;

	if (bp.hasPagoda){
		double test = bp.pagoda * (bp.startState - bp.endState); //TODO: delete this shit
		if (bp.pagoda * preCalc  < 0)
			return true;

	}

	if (possibleGeneration) pagodaFuncsMut.lock();
	for (auto itr = pagodaFunctions.begin(); itr != pagodaFunctions.end(); itr++)
		if (*itr * preCalc  < 0){
			bp.pagoda = *itr;
			bp.hasPagoda = true;
			if (possibleGeneration) pagodaFuncsMut.unlock();
			return true;
		}

		if (possibleGeneration) pagodaFuncsMut.unlock();
		return false;

}

//Creates a random state vector given a board
Vector<int> Pagoda::createRandomStateVector(const Matrix<bool>& board){
	int count = 0;
	for (int i=0; i<board.getXDim(); ++i)
		for (int j=0; j<board.getYDim(); ++j)
			if (board[i][j]) ++count;	

	Vector<int> temp(count);
	for (int i=0; i<count; ++i)
		temp[i] = rand()%2;

	return temp;
}


//Creates a random board pair given an integer based on the spec. for this coursework.
Pagoda::BoardPair Pagoda::createRandomBoardPair(int i) const{

	//Start configurations of 33-i pegs
	//End configurations of i pegs
	// 1 < i <= 16
	i %= (pegHoles / 2);
	++i;

	int start = i;
	int end = i;

	//Create a boardPair to start with
	BoardPair b = BoardPair(pegHoles);

	//Fill in all our start pegs
	for (int i=0; i<pegHoles; ++i)
		b.startState[i] = 1;

	//Keep looking for random places in the vector to take a peg from
	while (start > 0){
		int randomNo = rand() % pegHoles;

		//Only decrement if it wasnt already empty
		if (b.startState[randomNo] != 0){
			b.startState[randomNo] = 0;
			start--;
		}
	}

	//Keep looking for random places in the vector to insert a peg
	while (end > 0){
		int randomNo = rand() % pegHoles;

		if (b.endState[randomNo] != 1){
			b.endState[randomNo] = 1;
			end--;
		}
	}

	return b;
}
