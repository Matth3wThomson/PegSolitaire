#include "Pagoda.h"


//TODO: Test this
Pagoda::Pagoda(bool eng){
	//Create board
	if (eng) board = Solitaire::CreateEngBoard();
	else board = Solitaire::CreateEurBoard();

	//Create the index matrix and count number of pegholes
	pegHoles = Solitaire::CreateIndexMat(indexMat, board);

	//Create the jump matrix
	jumpMat = Solitaire::CreateJumpMat(board, indexMat, pegHoles);

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

	pegHoles = Solitaire::CreateIndexMat(indexMat, board);
	jumpMat = Solitaire::CreateJumpMat(board, indexMat, pegHoles);
}


Pagoda::~Pagoda(void)
{
}

//bool Pagoda::set_start_state(const Vector<int>& start){
//	//TODO: NOT SUITABLE CHECK YET
//	if (start.size() != startStateVec.size()) return false;
//
//	startStateVec = start;
//	return true;
//}
//
//bool Pagoda::set_end_state(const Vector<int>& end){
//	if (endStateVec.size() != end.size()) return false;
//
//	endStateVec = end;
//	return true;
//}

bool Pagoda::load_from_file(const std::string& filename, std::vector<PagodaCombination>& pcs){
	std::ifstream file = std::ifstream();

	file.open(filename.c_str());
	if (!file.is_open()) return false;

	std::string temp;
	file >> temp;

	while (!file.eof()){

		if (temp == "WIDTH"){

			int width, height;

			file >> width;

			file >> temp;

			if (temp == "HEIGHT"){
				file >> height;
			} else return false; //The format is incorrect!

			board = Matrix<bool>(height, width);
			file >> temp;

			for (int i=0; i<height; ++i){
				for (int j=0; j<width; ++j){
					file >> temp;
					if (temp == "T") board[i][j] = true;
					else if (temp == "F") board[i][j] = false;
					else return false; //The format is incorrect
				}
			}

			pegHoles = Solitaire::CreateIndexMat(indexMat, board);
			jumpMat = Solitaire::CreateJumpMat(board, indexMat, pegHoles);

			file >> temp;
		}


		if (temp == "START:"){
			pcs.push_back(PagodaCombination(pegHoles));
			load_vector_from_board(file, pcs.back().startState);
		}

		file >> temp;

		if (temp == "END:")
			load_vector_from_board(file, pcs.back().endState);

		file >> temp;

		if (temp == "PAGODA"){
			//Pagoda becomes aware of the pagoda function
			this->pagodaFunctions.push_back(new Vector<int>(pegHoles));

			//Allows pagoda combination to keep track of pagoda function
			pcs.back().pagodaFunction = this->pagodaFunctions.back();
			load_vector_from_board(file, *pcs.back().pagodaFunction);
		}

	} 

	return true;
}

//TODO: These functions dont request that vectors be of the correct length
template<typename E>
void Pagoda::load_vector_from_board(std::istream& is, Vector<E>& v){

	char getme;

	for (int i=0; i<board.get_x_dim(); ++i){
		for (int j=0; j<board.get_y_dim(); ++j){
			if (board[i][j])
				is >> v[indexMat[i][j]];
			is >> getme;
		}
	}

}

bool Pagoda::print_to_file(const std::string& filename, const PagodaCombination& pc, bool append){
	std::ofstream file = std::ofstream();

	if (append)
		file.open(filename.c_str(), std::ofstream::app);
	else
		file.open(filename.c_str());

	if (!file.is_open()) return false;

	printPagCom(file, pc, append);
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

std::ostream& Pagoda::printPagCom(std::ostream& os, const Pagoda::PagodaCombination& pc, const bool append){

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
	this->print_vector_as_board(os, pc.startState);

	os << "END: " << std::endl;
	this->print_vector_as_board(os, pc.endState);

	if (pc.pagodaFunction){
		os << "PAGODA: " << std::endl;
		this->print_vector_as_board(os, *pc.pagodaFunction);
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
//bool Pagoda::generate_pagoda(Vector<int>& pagoda){
//
//	pagoda = Vector<int>(endStateVec);
//
//	/*Vector<int> pagoda(endStateVec);*/
//	Vector<bool> fixedVector(pagoda.size()); //True if fixed, false if not. TODO: Native array?
//
//	//Brute force?
//
//	
//	//For each row of the jump matrix
//	for (int i=0; i<jumpMat.get_x_dim(); ++i){
//
//		int jumpIndices[3];
//		int index = 0;
//
//		//Find the indices of stateVector which are part of this row's jump
//		for (int j=0; j<jumpMat.get_y_dim(); ++j){
//			if (jumpMat.at(i,j)) //If this is an index
//				if (jumpMat.at(i,j) == -1)	//If this is the landing spot (Z)
//					jumpIndices[2] = j;
//				else jumpIndices[index++] = j; //Else this is jumped from or over (X or Y)
//		}
//
//		//Jump indices now contains all of the indices into a stateVector where there is a jump present
//		if (pagoda[jumpIndices[0]] + pagoda[jumpIndices[1]] < pagoda[jumpIndices[2]]){
//			//We have found a jump which does not solve the inequality
//			//X + Y >= Z
//
//			//Lambda solveInequality for modifying a property until the inequality is solved
//			auto solveInequality = [&] (int indexToMod, bool grow){
//
//				while (pagoda[jumpIndices[0]] + pagoda[jumpIndices[1]] < pagoda[jumpIndices[2]]){
//					if (grow) pagoda[jumpIndices[indexToMod]]++;
//					else pagoda[jumpIndices[indexToMod]]--;
//				}
//
//				//Let the fixed vector know that you have fixed a value
//				fixedVector[jumpIndices[indexToMod]] = true;
//			};
//			
//
//			if (!fixedVector[jumpIndices[2]]){ //Z value not fixed yet
//				solveInequality(2, false); //Fix Z
//			}	else if (!fixedVector[jumpIndices[1]]){ //Y value not fixed yet
//				solveInequality(1, true); //Fix Y
//			}	else if (!fixedVector[jumpIndices[0]]){ //X value not fixed yet
//				solveInequality(0, true); //Fix X
//			} else {
//				//Not possible to create a pagoda for this board
//				/*std::cout << "Pag: " << pagoda << std::endl;
//				std::cout << "End: " << endStateVec << std::endl;
//				std::cout << "Fix: " << fixedVector << std::endl << std::endl;*/
//
//				return false;
//			}
//		}
//	}
//
//	/*std::cout << "fixed vector: " << fixedVector << std::endl;*/
//
//	/*std::cout << "Pag: " << pagoda << std::endl;
//	std::cout << "End: " << endStateVec << std::endl;
//	std::cout << "Fix: " << fixedVector << std::endl << std::endl;*/
//	/*return true;*/
//	if (verify_pagoda(pagoda))
//		return true;
//	else return false;
//}

//TODO: ACCESS YALE MATRIX MORE EFFICIENTLY
bool Pagoda::generate_pagoda(Vector<int>& pagoda, const Vector<int>& endState){

	//TODO: Make this method pass by pointer, allowing NULLS. Then use the following code to
	// verify state.

	//We want to store all of our pagodas in the pagoda class, and have references to ones that work
	//for a pagoda combination stored as a pointer to the pagodas array of pagoda functions function that
	//works for that combination.

	/*if (pagoda){
		if (verify_pagoda(*pagoda)) return true;
		else delete pagoda;
	}
	
	pagodaFunctions.push_back(new Vector<int>(endState));
	pagoda = pagodaFunctions.back();
	*/


	pagoda = Vector<int>(pegHoles);
	
	/*Vector<int> pagoda(endStateVec);*/
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
				//Not possible to create a pagoda for this board
				/*std::cout << "Pag: " << pagoda << std::endl;
				std::cout << "End: " << endStateVec << std::endl;
				std::cout << "Fix: " << fixedVector << std::endl << std::endl;*/

				return false;
			}
		}
	}

	/*std::cout << "fixed vector: " << fixedVector << std::endl;*/

	/*std::cout << "Pag: " << pagoda << std::endl;
	std::cout << "End: " << endStateVec << std::endl;
	std::cout << "Fix: " << fixedVector << std::endl << std::endl;*/
	/*return true;*/
	if (verify_pagoda(pagoda))
		return true;
	else return false;
}


//TODO: Test this! THINK THIS IS NOW WORKING
bool Pagoda::verify_pagoda(const Vector<int>& pagoda){
	//NO NEED TO TRANSPOSE! (Jump matrix is already in jump*location format)
	Vector<int> x = (jumpMat * pagoda);

	for (int i=0; i<x.size(); ++i)
		if (x[i] < 0) return false;

	return true;
};

////TODO: Test this!
//bool Pagoda::prove_insolvable(const Vector<int>& pagoda){
//	//If (s-e).p is negative, then this problem is insolvable
//	return ((startStateVec - endStateVec)*pagoda < 0);
//};

bool Pagoda::prove_insolvable(const PagodaCombination& pc){
	return ((pc.startState - pc.endState)* *pc.pagodaFunction < 0);
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

//void Pagoda::randomize_start_and_end(){
//	startStateVec = create_random_state_vector(board);
//	endStateVec = create_random_state_vector(board);
//}
