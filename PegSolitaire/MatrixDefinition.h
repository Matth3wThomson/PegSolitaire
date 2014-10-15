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
		mat[i] = new T[y];
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
		memset(mat[i], 0, (y * sizeof(T)));
	}

	//Copy Values
	for (int i=0; i<rhs.x; ++i){
		for (int j=0; j<rhs.y; ++j){
			mat[i][j] = rhs.mat[i][j];
		}
	}

	return *this;
}

//TODO: Untested
template<typename T>
T Matrix<T>::determinant(){
	if (x != y) return; //TODO: Throw exception (If not square)
	T pos;
	T neg;

	for (int i=0; i<x; ++i){
		pos *= mat[i][i];
		neg *= mat[i][x-(i+1)];
	}

	return pos - neg;
}

//TODO: Implement
template<typename T>
Matrix<T> Matrix<T>::inverse(){
	if (determinant() == 0) return; //TODO: Throw exception
}

//TODO: Untested
template<typename T>
Matrix<T> Matrix<T>::transpose(){
	Matrix<T> trans(y, x);
	
	for (int i=0; i<x; ++i)
		for (int j=0; j<y; ++j)
			trans[j][i] = mat[i][j];

	return trans;
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

//TODO: Safeguards And exceptions for matrix sizes
template<typename E>
Matrix<E> operator+(const Matrix<E>& lhs, const Matrix<E>& rhs){

	Matrix<E> temp();
	for (int i=0; i<lhs.get_x_dim(); ++i)
		for (int j=0; j<lhs.get_y_dim(); ++j)
			temp[i][j] = lhs[i][j] + rhs[i][j];
	
	return temp;
}

template<typename E>
Matrix<E> operator-(const Matrix<E>& lhs, const Matrix<E>& rhs){

	Matrix<E> temp();
	for (int i=0; i<lhs.get_x_dim(); ++i)
		for (int j=0; j<lhs.get_y_dim(); j++)
			temp[i][j] = lhs[i][j] - rhs[i][j];

	return temp;
}

template<typename E>
Matrix<E> operator/(const Matrix<E>& lhs, const Matrix<E>& rhs){
	Matrix<E> temp();

	
}