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
	NOTES:
		-Moves semantics has been implemented into this class to improve performance when
		 creating copies

*/

template<typename T>
class Vector
{
public:
	static const int default_size = 5;

	Vector(const int size = default_size);
	Vector(const Vector<T>& rhs);
	Vector(Vector<T>&& rval); 
	~Vector(void);

	Vector<T>& operator=(const Vector<T>& rhs);
	Vector<T>& operator=(Vector<T>&& rval);

	template<typename E>
	Vector(const Vector<E>& rhs);

	//Boolean operators
	template<typename E>
	friend bool operator==(const Vector<E>& lhs, const Vector<E>& rhs);

	template<typename E>
	friend bool operator!=(const Vector<E>& lhs, const Vector<E>& rhs);

	template<typename E>
	friend bool operator<(const Vector<E>& lhs, const Vector<E>& rhs);

	//Arithmetic
	T magnitude();

	//T
	template<typename E>
	friend Vector<E> operator*(const Vector<E>& lhs, const E& rhs);
	template<typename E>
	friend Vector<E> operator*(const E& lhs, const Vector<E>& rhs);
	Vector<T>& operator*=(const T& rhs);

	//Vector
	template<typename E>
	friend E operator*(const Vector<E>& lhs, const Vector<E>& rhs);

	template<typename E, typename F>
	friend E operator*(const Vector<E>& lhs, const Vector<F>& rhs);

	template<typename E>
	friend Vector<E> operator+(const Vector<E>& lhs, const Vector<E>& rhs);
	Vector<T>& operator+=(const Vector<T>& rhs);

	template<typename E>
	friend Vector<E> operator-(const Vector<E>& lhs, const Vector<E>& rhs);
	Vector<T>& operator-=(const Vector<T>& rhs);

	//Insertion
	inline void insertElement(const T& element, const int location){ arr[location] = element; };

	//Access
	inline T& at(const int location){ return arr[location];	};

	inline T& operator[](int location) const{ return arr[location];	};

	//Size of Vector
	inline int size() const{ return containerSize; };

	//Display
	template<typename E>
	friend std::ostream& operator<<(std::ostream& os, const Vector<E>& v);

private:
	int containerSize;

	//arr
	T* arr;
};

#include "VectorDefinition.h"

