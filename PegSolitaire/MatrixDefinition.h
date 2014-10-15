template<typename T>
Matrix<T>::Matrix(const int xSize, const int ySize, const T& value) :
x(xSize), y(ySize){
	mat = new T*[x];
	for (int i=0; i<x; ++i){
		mat[i] = new T[y];
		memset(mat[i], value, (y * sizeof(T)));
	}
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& src):
x(src.x), y(src.y)
{
	mat = new T*[x];
	for(int i = 0; i < x; ++i)	
	{
			mat[i] = new T[y_size];
			memcpy_s(mat[i], (y * sizeof(T)), src.mat[i], (y * sizeof(T)));
	}
}

template<typename T>
Matrix<T>::~Matrix(){
	for (int i = 0; i<x; i++){
		delete[] mat[i];
	}
	delete[] mat;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs){
	if (this == &rhs) return (*this);

	//Release Old memory
	for (int i=0; i < x; ++i){
		delete[] mat[i];
	}
	delete[] mat;

	x=rhs.x;
	y=rhs.y;

	//Allocate new memory
	mat = new T*[x];
	for (int i=0; i< x; ++i){
		mat[i] = new T[y];
		memset(cells[i], 0, (y * sizeof(T)));
	}

	//Copy Values
	for (int i=0; i<rhs.x; ++i){
		for (int j=0; j<rhs.y; ++j){
			mat[i][j] = rhs.mat[i][j];
		}
	}

	return *this;
}

template<typename E>
Matrix<E> operator*(const Matrix<E>& lhs, const Matrix<E>& rhs){
	Matrix<E> result(lhs.x, rhs.y);
	for (int i=0; i<lhs.x; ++i){
		for (int j=0; j<lhs.x; ++j){
			for (int k = 0; k < lhs.x; ++k){
				result[i][j] += lhs[k][j] * rhs[i][k];
			}
		}
	}
	return result;
}