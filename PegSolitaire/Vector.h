#pragma once

#ifndef NULL
#define NULL 0
#endif

#include <iostream>
#include <stdexcept>

//Forward Declaration
template<typename T>
class YaleMatrix;

/*
	TODO:
		-Correctness testing for all vector operators

*/

template<typename T>
class Vector
{
public:
	static const int default_size = 5;

	Vector(const int size = default_size);
	Vector(const Vector<T>& rhs);
	Vector(Vector<T>&& rval); //<-- Move constructor... AMAZING!
	~Vector(void);

	//TODO: Make assignment operators more efficient if data is of the same size
	Vector<T>& operator=(const Vector<T>& rhs);
	Vector<T>& operator=(Vector<T>&& rval);

	//TODO: Implement a copy constructor that takes an alternate type.
	template<typename E>
	Vector(const Vector<E>& rhs);

	//Arithmetic
	T magnitude();

	//T
	template<typename E>
	friend Vector<E> operator*(const Vector<E>& lhs, const E& rhs);
	template<typename E>
	friend Vector<E> operator*(const E& lhs, const Vector<E>& rhs);

	//Vector
	template<typename E>
	friend E operator*(const Vector<E>& lhs, const Vector<E>& rhs);

	template<typename E>
	friend Vector<E> operator+(const Vector<E>& lhs, const Vector<E>& rhs);
	Vector<T>& operator+=(const Vector<T>& rhs);

	template<typename E>
	friend Vector<E> operator-(const Vector<E>& lhs, const Vector<E>& rhs);
	Vector<T>& operator-=(const Vector<T>& rhs);

	//Insertion
	inline void insert_element(const T& element, const int location){ arr[location] = element; };

	//Access
	inline T& at(const int location){ return arr[location]; };
	inline T& operator[](int location) const{ return arr[location]; };

	//Size of Vector
	inline int size() const{ return container_size; };

	//Display
	friend std::ostream& operator<<(std::ostream& os, Vector<T>& v){
		for (int i=0; i<v.size(); ++i)
			os << v[i] << ",";
		return os;
	};

private:
	int container_size;

	//arr
	T* arr;
};

#include "VectorDefinition.h"

