
//DEFAULT
template<typename T>
YaleMatrix<T>::YaleMatrix():
	A(NULL), IA(NULL), JA(NULL), x(0), y(0){

}

//COPY CONSTRUCTOR
template<typename T>
YaleMatrix<T>::YaleMatrix(const YaleMatrix<T>& rhs):
	x(rhs.x), y(rhs.y){


		IA = new int[x];
		memcpy_s(IA, (x*sizeof(int)), rhs.IA, (x*sizeof(int)));

		A = new T[IA[x-1]]();
		JA = new int[IA[x-1]]();

		memcpy_s(A, (IA[x-1]*sizeof(T)), rhs.A, (IA[x-1]*sizeof(T)));
		memcpy_s(JA, (IA[x-1]*sizeof(int)), rhs.JA, (IA[x-1]*sizeof(int)));
}

//MOVE CONSTRUCTOR
template<typename T>
YaleMatrix<T>::YaleMatrix(YaleMatrix<T>&& rval):
	A(rval.A), IA(rval.IA), JA(rval.JA), x(rval.x), y(rval.y){
		rval.A = NULL;
		rval.IA = NULL;
		rval.JA = NULL;
		rval.x = 0;
		rval.y = 0;
}

//ASSIGNMENT OPERATOR
template<typename T>
YaleMatrix<T>& YaleMatrix<T>::operator=(const YaleMatrix<T>& rhs){
	delete[] A;
	delete[] IA;
	delete[] JA;

	x = rhs.x;
	y = rhs.y;

	IA = new int[x];
	memcpy_s(IA, (x*sizeof(int)), rhs.IA, (x*sizeof(int)));

	A = new T[IA[x-1]]();
	JA = new int[IA[x-1]]();

	memcpy_s(A, (IA[x-1]*sizeof(T)), rhs.A, (IA[x-1]*sizeof(T)));
	memcpy_s(JA, (IA[x-1]*sizeof(int)), rhs.JA, (IA[x-1]*sizeof(int)));

	return *this;
}

//MOVE-ASSIGN OPERATOR
template<typename T>
YaleMatrix<T>& YaleMatrix<T>::operator=(YaleMatrix<T>&& rval){
	std::swap(A, rval.A);
	std::swap(IA, rval.IA);
	std::swap(JA, rval.JA);
	std::swap(x, rval.x);
	std::swap(y, rval.y);

	return *this;
}

//Use of vector over double loop works well for large sparse matrices,
//being out performed on small spare matrices. Since our jump matrices will tend
//to be large (33*n) where n>30, a vector was used.
template<typename T>
YaleMatrix<T>::YaleMatrix(const Matrix<T>& verbose):
	x(verbose.get_x_dim()+1), y(verbose.get_y_dim()){


		//TODO: What if the matrix passed is 0x0
		std::vector<KeyValue> nonZeroElements = std::vector<KeyValue>();

		//IA is of size equal to number of rows in the matrix
		IA = new int[x];
		//We must set the values to -1, as not to confuse a zero index flagging as not set
		memset(this->IA, -1, x*sizeof(int));
		int indexCounter = 0;

		//For all locations in the matrix
		for (int i=0; i<verbose.get_x_dim(); ++i){
			for (int j=0; j<verbose.get_y_dim(); ++j)
				if (verbose[i][j]){
					//If there exists an element
					//Add it to the vector of elements we know of

					nonZeroElements.push_back(KeyValue(&verbose[i][j], j));

					//Check to see if it is the first one on its row, if so set the IA value
					if (IA[i] < 0) IA[i] = indexCounter;
					indexCounter++;

				}
		}

		//We now know the size of A and JA
		IA[x-1] = nonZeroElements.size();
		A = new T[IA[x-1]]();
		JA = new int[IA[x-1]]();

		//Unset the -1 flag
		if (IA[0] == -1) IA[0] = 0;

		for (int i=0; i<IA[x-1]; ++i){
			//Set A to contain all of the Non zero elements
			A[i] = *nonZeroElements[i].value;

			//Set JA to contain all of the column identifiers for all non zero elements
			JA[i] = nonZeroElements[i].yindex;
		}

}

template<typename T>
YaleMatrix<T>::~YaleMatrix(){
	delete[] A;
	delete[] IA;
	delete[] JA;
}


template<typename T>
T YaleMatrix<T>::at(int x, int y){

	//Loop through the elements on the row to see if one is in the column requested
	for (int i=IA[x]; i<IA[x+1]; ++i)
		if (JA[i] == y) return A[i];

	return 0;
}

//template<typename T>
//Vector<T> YaleMatrix<T>::operator*(const Vector<T>& rhs){
//	if (this->y != rhs.size()) throw 
//		std::invalid_argument("Matrix/Vector multiplication requires same number of matrix columns to vector size");
//
//	Vector<T> temp(rhs.size());
//
//	//For each possible row
//	for (int i=0; i<x-1; ++i)
//		for (int j = IA[i]; j < IA[i+1]; ++j)
//			temp[i] += A[j] * rhs[JA[j]];
//		
//	return temp;
//}

template<typename E>
Vector<E> operator*(const YaleMatrix<E>& lhs, const Vector<E>& rhs){
	if (lhs.y != rhs.size()) throw 
		std::invalid_argument("Matrix/Vector multiplication requires same number of matrix columns to vector size");

	Vector<E> temp(lhs.get_x_dim());

	//For each possible row
	for (int i=0; i<lhs.x-1; ++i)
		//Loop through the elements on that row
		for (int j = lhs.IA[i]; j < lhs.IA[i+1]; ++j)
			//Adding the multiplication of the elements on that row with the 
			//vector at the location of the column of the element to be multiplied
			temp[i] += lhs.A[j] * rhs[lhs.JA[j]];
		
	return temp;
}

template<typename T>
Vector<T> operator*(const Vector<T>& lhs, const YaleMatrix<T>& rhs){
	return rhs*lhs;
}