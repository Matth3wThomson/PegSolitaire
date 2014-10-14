#pragma once

#include <iostream>
#include <string>

#ifndef NULL
#define NULL 0
#endif

//TODO: Do all operators for matrix class.


template<typename T>
class Matrix {

public:
	//Default Values
	static const int defaultX = 3;
	static const int defaultY = 3;

	//Construction
	Matrix(const int xSize = defaultX, const int ySize = defaultY, const T& value = NULL);
	Matrix(const Matrix<T>& src);
	~Matrix(void);
	Matrix<T>& operator=(const Matrix<T>& rhs);

	//Arithmetic
	template<typename E>
	friend Matrix<E> operator*(const Matrix<E>& a, const Matrix<E>& b);

	//Insertion
	inline void setElement(const int x, const int y, const T& elem){ mat[x][y] = elem; };

	//Access
	inline T& getElement(const int x, const int y){ return mat[x][y]; };
	inline T* operator[](const int location) const{ return mat[location]; };

	//Size operations
	inline int getXSize() const{ return x; };
	inline int getYSize() const{ return y; };

	inline int getHeight() const{ return x; };
	inline int getWidth() const { return y; };

	//Display
	friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m){
		for (int i=0; i<m.x; ++i){
			for (int j=0; j<m.y; ++j) os << m[i][j] << " ";
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