#include "Pagoda.h"


Pagoda::Pagoda(bool eng){
	if (eng) board = Solitaire::CreateEngBoard();
	else board = Solitaire::CreateEurBoard();

	indexMat = Solitaire::CreateIndexMat(board);
	startStateVec = create_random_state_vector(board);
	endStateVec = create_random_state_vector(board);
	jumpMat = Solitaire::CreateJumpMat(board, indexMat, startStateVec.size());

	//TODO: Remove
	/*std::cout << "Board Shape:\n" << board << std::endl;
	std::cout << "Index Matrix:\n" << indexMat << std::endl;
	std::cout << "Jump Matrix:\n" << jumpMat << std::endl;
	std::cout << "Start State:\n" << startStateVec << std::endl;
	std::cout << "End State:\n" << endStateVec << std::endl;*/
}

Pagoda::Pagoda(const Matrix<bool>& boardShape)
{
	board = boardShape;

	indexMat = Solitaire::CreateIndexMat(board);
	startStateVec = create_random_state_vector(board);
	endStateVec = create_random_state_vector(board);
	jumpMat = Solitaire::CreateJumpMat(board, indexMat, startStateVec.size());
}


Pagoda::~Pagoda(void)
{
}

bool Pagoda::load_from_file(const std::string& filename){
	std::ifstream file(filename.c_str());

	file.open(filename.c_str());
	if (!file.is_open()) return false;

	std::string temp;

	while (!file.eof()){
		file >> temp;

		if (temp == "WIDTH"){
			int width, height;

			file >> width;

			file >> temp;

			if (temp == "HEIGHT"){
				file >> height;
			} else return false;

			for (int i=0; i<height; ++i)
				file >> temp;
		} else return false;
	}

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

	os << "START: " << std::endl;
	for (int i=0; i<p.board.get_x_dim(); ++i){
		for (int j=0; j<p.board.get_y_dim(); ++j)
			if (p.board[i][j])
				os << p.startStateVec[p.indexMat[i][j]] << ",";
			else 
				os << " ,";

		os << std::endl;
	}

	os << "END: " << std::endl;
	for (int i=0; i<p.board.get_x_dim(); ++i){
		for (int j=0; j<p.board.get_y_dim(); ++j)
			if (p.board[i][j])
				os << p.endStateVec[p.indexMat[i][j]] << ",";
			else
				os << " ,";

		os << std::endl;
	}

	os << "PAGODA: " << std::endl;
	//TODO: FINISH THIS

	return os;
}

//TODO: ACCESS YALE MATRIX MORE EFFICIENTLY
bool Pagoda::generate_pagoda(Vector<int>& pagoda){

	
	pagoda = Vector<int>(endStateVec);

	/*Vector<int> pagoda(endStateVec);*/
	Vector<bool> fixedVector(pagoda.size()); //True if fixed, false if not

	//Brute force?

	//For each row of the jumpagoda matrix
	for (int i=0; i<jumpMat.get_x_dim(); ++i){

		int jumpIndices[3];
		int index = 0;

		//Find the indices of stateVector which are part of this row's jump
		for (int j=0; j<jumpMat.get_y_dim(); ++j){
			if (jumpMat.at(i,j)) //If this is an index
				if (jumpMat.at(i,j) == -1)	//If this is the landing spot
					jumpIndices[2] = jumpMat.at(i,j);
				else jumpIndices[index++] = jumpMat.at(i,j); //Else this is jumped from or over
		}

		int x;

		//Jump indices now contains all of the indices into a stateVector where there is a jump present
		if (pagoda[jumpIndices[0]] + pagoda[jumpIndices[1]] < pagoda[jumpIndices[2]]){
			//We have found a jump which does not solve the inequality
			//X + Y >= Z

			//Lambda Function for modifying a property until the inequality is solved
			auto function = [&] (int indexToMod, bool grow){
				while (pagoda[jumpIndices[0]] + pagoda[jumpIndices[1]] < pagoda[jumpIndices[2]]){
					if (grow) pagoda[jumpIndices[indexToMod]]++;
					else pagoda[jumpIndices[indexToMod]]--;
				}
				//Let the fixed vector know that you have fixed a value
				fixedVector[jumpIndices[indexToMod]] = true;
			};


			if (!fixedVector[pagoda[jumpIndices[2]]]){ //Z value not fixed yet
				function(2, false); //Fix Z
			}	else if (!fixedVector[pagoda[jumpIndices[1]]]){ //Y value not fixed yet
				function(1, true); //Fix Y
			}	else if (!fixedVector[pagoda[jumpIndices[0]]]){ //X value not fixed yet
				function(0, true); //Fix X
			} else {
				//Not possible to fix this function!
				return false;
			}
		}
	}

	std::cout << "Pagoda: " << pagoda << std::endl;
	std::cout << "End State Vec: " << endStateVec << std::endl;
	std::cout << "Fixed Vector: " << fixedVector << std::endl;

	return true;
}

//TODO: Test this!
bool Pagoda::verify_pagoda(const Vector<int>& pagoda){
	//NO NEED TO TRANSPOSE! (Jump matrix is already in jump*location format)
	Vector<int> x = jumpMat * pagoda;

	for (int i=0; i<x.size(); ++i)
		if (x[i] < 0) return false;

	return true;
};

//TODO: Test this!
bool Pagoda::prove_insolvable(const Vector<int>& pagoda){
	//If (s-e).p is negative, then this problem is insolvable
	if ((startStateVec - endStateVec)*pagoda < 0) return true;

	//Else this problem hasnt been proved insolvable by this pagoda
	return false;
};

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

void Pagoda::randomize_start_and_end(){
	startStateVec = create_random_state_vector(board);
	endStateVec = create_random_state_vector(board);
}
