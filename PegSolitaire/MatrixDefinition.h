template<typename T>
Matrix<T>::Matrix(const int xSize, const int ySize, const T& value) :
x(xSize), y(ySize){
	mat = new T*[x];
	for (int i=0; i<y; ++i){
		mat[i] = new T[y];
		memset(mat[i], value, (y * sizeof(T)));
	}
}

template<typename T>
Matrix<T>::~Matrix(){
	for (int i = 0; i<x; i++){
		delete[] mat[i];
	}
	delete[] mat;
}
