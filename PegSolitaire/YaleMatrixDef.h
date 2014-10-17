

template<typename T>
YaleMatrix<T>::YaleMatrix(const Matrix<T>& verbose):
	x(verbose.get_x_dim()), y(verbose.get_y_dim()){

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
					nonZeroElements.push_back(KeyValue(verbose[i][j], j));

					//Check to see if it is the first one on its row, if so set the IA value
					if (IA[i] < 0) IA[i] = indexCounter;
					indexCounter++;

				}
		}


		//We now know the size of A and JA
		NZ = nonZeroElements.size();
		A = new T[NZ]();
		JA = new int[NZ]();

		//Unset the -1 flag
		if (IA[0] == -1) IA[0] = 0;

		for (unsigned int i=0; i<nonZeroElements.size(); ++i){
			//Set A to contain all of the Non zero elements
			A[i] = nonZeroElements[i].value;

			//Set JA to contain all of the column identifiers for all non zero elements
			JA[i] = nonZeroElements[i].yindex;
		}

}


template<typename T>
T& YaleMatrix<T>::at(int x, int y){
	//Work out how many elements are on that row
	int k = (IA[x+1]) - (IA[x]);

	//Loop through the elements on the row to see if one is in the column requested
	for (int i=IA[x]; i<IA[x]+k; ++i)
		if (JA[i] == y) return A[i];

	return 0;
}