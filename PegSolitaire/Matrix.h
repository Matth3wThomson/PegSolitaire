#pragma once

#include <iostream>
#include <iomanip>
#include <string>

#ifndef NULL
#define NULL 0
#endif

/*TODO: 
		-Do all operators for matrix class.
		-Evaulate pass by reference used... Lots of matrices are bools (slower to pass 4 bytes than
		 1 bit) 
		-Finish mathematical operators and test them!
*/




template<typename T>
class Matrix {

public:
	//Default Values
	static const int defaultX = 3;
	static const int defaultY = 3;

	//Construction
	Matrix(const int xSize = defaultX, const int ySize = defaultY, const T& value = NULL);
	Matrix(const Matrix<T>& src); 
	Matrix(Matrix<T>&& rval); //<-- Move constructor... AMAZING!
	~Matrix(void);
	Matrix<T>& operator=(const Matrix<T>& rhs);
	Matrix<T>& operator=(Matrix<T>&& rval); //<-- Move assignment operator

	//Arithmetic
	T determinant();
	Matrix<T> inverse();
	Matrix<T> transpose();
	
	template<typename E>
	friend Matrix<E> operator*(const Matrix<E>& lhs, const Matrix<E>& rhs);
	
	template<typename E>
	friend Matrix<E> operator+(const Matrix<E>& lhs, const Matrix<E>& rhs);

	template<typename E>
	friend Matrix<E> operator-(const Matrix<E>& lhs, const Matrix<E>& rhs);

	template<typename E>
	friend Matrix<E> operator/(const Matrix<E>& lhs, const Matrix<E>& rhs);
	

	//Insertion
	inline void set_element(const int x, const int y, const T& elem){ mat[x][y] = elem; };

	//Access
	inline T& get_element(const int x, const int y){ return mat[x][y]; };
	inline T* operator[](const int location) const{ return mat[location]; };

	//Size operations
	inline int get_x_dim() const{ return x; };
	inline int get_y_dim() const{ return y; };

	inline int get_height() const{ return x; };
	inline int get_width() const { return y; };

	//Display
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