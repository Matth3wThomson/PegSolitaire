#include "Pagoda.h"


Pagoda::Pagoda(bool eng){
	//Create board
	if (eng) board = Solitaire::CreateEngBoard();
	else board = Solitaire::CreateEurBoard();

	//Create the index matrix and count number of pegholes
	pegHoles = Solitaire::CreateIndexMat(indexMat, board);

	//Create the jump matrix
	jumpMat = Solitaire::CreateJumpMat(board, indexMat, pegHoles);

}

Pagoda::Pagoda(const Matrix<bool>& boardShape)
{
	board = boardShape;

	pegHoles = Solitaire::CreateIndexMat(indexMat, board);
	jumpMat = Solitaire::CreateJumpMat(board, indexMat, pegHoles);
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

//TODO: Flexibility?
void Pagoda::load_from_file(const std::string& filename, std::vector<BoardPair>& bps){
	std::ifstream file = std::ifstream();

	file.open(filename.c_str());
	if (!file.is_open()) return throw std::invalid_argument("File not found");

	std::string temp;
	file >> temp;

	while (!file.eof()){

		if (temp == "WIDTH"){

			int width, height;

			file >> width;

			file >> temp;

			if (temp == "HEIGHT"){
				file >> height;
			} else throw std::invalid_argument("File format incorrect"); //The format is incorrect!

			board = Matrix<bool>(height, width);
			file >> temp;

			for (int i=0; i<height; ++i){
				for (int j=0; j<width; ++j){
					file >> temp;
					if (temp == "T") board[i][j] = true;
					else if (temp == "F") board[i][j] = false;
					else throw std::invalid_argument("File format incorrect"); //The format is incorrect
				}
			}

			pegHoles = Solitaire::CreateIndexMat(indexMat, board);
			jumpMat = Solitaire::CreateJumpMat(board, indexMat, pegHoles);

			file >> temp;
		}


		if (temp == "START:"){
			bps.push_back(BoardPair(pegHoles));
			load_vector_from_board(file, bps.back().startState);

			file >> temp;
		}

		if (temp == "END:"){
			load_vector_from_board(file, bps.back().endState);

			file >> temp;
		}

		if (temp == "PAGODA:"){
			bps.back().pagoda = Vector<double>(pegHoles);
			bps.back().hasPagoda = true;
			load_vector_from_board(file, bps.back().pagoda);

			file >> temp;
		}

	}

	file.close();
}

//TODO: These functions dont request that vectors be of the correct length
template<typename E>
void Pagoda::load_vector_from_board(std::istream& is, Vector<E>& v){

	if (v.size() != pegHoles) throw std::invalid_argument("Cannot load vector from board. Vector must have correct size");

	char getme;

	for (int i=0; i<board.get_x_dim(); ++i){
		for (int j=0; j<board.get_y_dim(); ++j){
			if (board[i][j])
				is >> v[indexMat[i][j]];
			is >> getme;
		}
	}

}

bool Pagoda::save_pagoda_functions(const std::string& filename){

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

bool Pagoda::load_pagoda_functions(const std::string& filename){

	std::cout << "Loading pagodas from: '" << filename << "'." << std::endl;
	std::ifstream file;

	file.open(filename);

	if (!file.is_open()) return false;

	while (!file.eof()){
		Vector<double> pagoda(pegHoles);
		load_vector_from_board(file, pagoda);

		pagodaFunctions.insert(pagoda);
	}

	file.close();

	return true;
}

bool Pagoda::print_to_file(const std::string& filename, const BoardPair& bp, bool append){
	std::ofstream file = std::ofstream();

	if (append)
		file.open(filename.c_str(), std::ofstream::app);
	else
		file.open(filename.c_str());

	if (!file.is_open()) return false;

	printPagCom(file, bp, append);
	file.close();

	return true;
}

std::ostream& operator<<(std::ostream& os, const Pagoda& p){
	os << "WIDTH " << p.board.get_width() << std::endl;
	os << "HEIGHT " << p.board.get_height() << std::endl;
	os << "BOARD: " << std::endl;

	for (int i=0; i<p.board.get_x_dim(); ++i){
		for (int j=0; j<p.board.get_y_dim(); ++j)
			(p.board[i][j]) ? os << "T " : os << "F ";

		os << std::endl;
	}

	return os;
}

std::ostream& Pagoda::printPagCom(std::ostream& os, const Pagoda::BoardPair& bp, const bool append){

	if (!append){
		os << "WIDTH " << this->board.get_width() << std::endl;
		os << "HEIGHT " << this->board.get_height() << std::endl;
		os << "BOARD: " << std::endl;

		for (int i=0; i<this->board.get_x_dim(); ++i){
			for (int j=0; j<this->board.get_y_dim(); ++j)
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
void Pagoda::print_vector_as_board(std::ostream& os, const Vector<E>& v){
	for (int i=0; i<this->board.get_x_dim(); ++i){
		for (int j=0; j<this->board.get_y_dim(); ++j)
			if (this->board[i][j])
				os << v[this->indexMat[i][j]] << ",";
			else 
				os << " ,";

		os <<std::endl;
	}
}

//TODO: ACCESS YALE MATRIX MORE EFFICIENTLY
bool Pagoda::generate_pagoda(Vector<double>& pagoda, const Vector<int>& endState, bool saveResults){
	if (possibleGeneration){

	pagoda = Vector<double>(endState);

	Vector<bool> fixedVector(pagoda.size()); //True if fixed, false if not. TODO: Native array?

	//Brute force?

	//For each row of the jump matrix
	for (int i=0; i<jumpMat.get_x_dim(); ++i){

		int jumpIndices[3];
		int index = 0;

		//Find the indices of stateVector which are part of this row's jump
		for (int j=0; j<jumpMat.get_y_dim(); ++j){
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

	if (verify_pagoda(pagoda, saveResults))
		return true;
	}

	return false;
}

bool Pagoda::verify_pagoda(const Vector<double>& pagoda, const bool saveResults){
	//NO NEED TO TRANSPOSE! (Jump matrix is already in jump*location format)
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
	pagodaFunctions.insert(pagoda);
	pagodaFuncsMut.unlock();
}

bool Pagoda::prove_insolvable(const BoardPair& bp){
	return (bp.pagoda * (bp.startState - bp.endState) < 0);
}

bool Pagoda::prove_insolv_with_saved(BoardPair& bp){
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

//TODO: use true random?
Vector<int> Pagoda::create_random_state_vector(const Matrix<bool>& board){
	int count = 0;
	for (int i=0; i<board.get_x_dim(); ++i)
		for (int j=0; j<board.get_y_dim(); ++j)
			if (board[i][j]) ++count;	

	Vector<int> temp(count);
	for (int i=0; i<count; ++i)
		temp[i] = rand()%2;

	return temp;
}

//TODO: Passing out a board pair means creating a copy.
//There are lots of places where the copy constructor of board pair is called....
//Implement a move constructor for it?
Pagoda::BoardPair Pagoda::create_random_board_pair(int i) const{

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

//void Pagoda::printSet(){
//	for (auto itr = pagodaFunctions.begin(); itr != pagodaFunctions.end(); itr++)
//		std::cout << *itr << std::endl;
//}
