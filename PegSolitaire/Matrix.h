#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include "Vector.h"
/*#include <stdlib.h>*/	//TODO: Work out if necessary

#ifndef NULL
#define NULL 0
#endif

/*TODO: 
		-Do all operators for matrix class.
		-Evaulate pass by reference used... Lots of matrices are bools (slower to pass 4 bytes than
		 1 bit) 
		-Test mathematical operators
		-Test determinant+ transpose
		-Implement Inverse
		-Implement vector/matrix functionality
		-Make assignment operators more efficient

NOTES:
		-Performance of exceptions that arent thrown or caught identical to functions that dont throw,
		 therefore exceptions used on functions where matrices must be of identical size, as program
		 crash is desired. (BENCHMARK: 10000 valid add operations)
*/




template<typename T>
class Matrix {

public:
	//Default Values
	static const int defaultX = 3;
	static const int defaultY = 3;

	//Construction
	Matrix(const int xSize = defaultX, const int ySize = defaultY);
	Matrix(const Matrix<T>& src); 
	Matrix(Matrix<T>&& rval); //<-- Move constructor
	~Matrix(void);
	Matrix<T>& operator=(const Matrix<T>& rhs);
	Matrix<T>& operator=(Matrix<T>&& rval); //<-- Move assignment operator

	//Arithmetic
	T determinant();
	Matrix<T> inverse();
	Matrix<T> transpose();

	//T arithmetic
	template<typename E>
	friend Matrix<E> operator*(const Matrix<E>& lhs, const E& rhs);
	template<typename E>
	friend Matrix<E> operator*(const E& lhs, const Matrix<E>& rhs);
	Matrix<T>& operator*=(const T& rhs);


	//Matrix arithmetic
	template<typename E>
	friend Matrix<E> operator*(const Matrix<E>& lhs, const Matrix<E>& rhs);
	Matrix<T>& operator*=(const Matrix<T>& rhs);
	
	template<typename E>
	friend Matrix<E> operator+(const Matrix<E>& lhs, const Matrix<E>& rhs);
	Matrix<T>& operator+=(const Matrix<T>& rhs);

	template<typename E>
	friend Matrix<E> operator-(const Matrix<E>& lhs, const Matrix<E>& rhs);
	Matrix<T>& operator-=(const Matrix<T>& rhs);

	//Vector arithmetic
	Vector<T> operator*(const Vector<T>& rhs);

	//Insertion
	inline void set_element(const int x, const int y, const T& elem){ mat[x][y] = elem; };

	//TODO: Remove guards

	//Access
	inline T& get_element(const int x, const int y){ return mat[x][y]; };
	inline T* operator[](const int location) const{ 
		if (location < 0 || location >= x)
			return mat[0];
		return mat[location]; 
	};

	//Size operations
	inline int get_x_dim() const{ return x; };
	inline int get_y_dim() const{ return y; };

	inline int get_height() const{ return x; };
	inline int get_width() const { return y; };

	//Display //TODO: Move into implementation file
	friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m){
		for (int i=0; i<m.x; ++i){
			for (int j=0; j<m.y; ++j) os << std::right << std::setw(3) <<  m[i][j];
			os << std::endl;
		}
		return os;
	};

private:
	int x;
	int y;
	T** mat;
};

#include "MatrixDefinition.h"