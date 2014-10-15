#pragma once

#ifndef NULL
#define NULL 0
#endif

template<typename T>
class Vector
{
public:
	static const int defaultSize = 5;

	Vector(const int size = defaultSize, const T& value = NULL);
	Vector(const Vector<T>& rhs);
	~Vector(void);
	Vector<T>& operator=(const Matrix<T>& rhs);

	//Insertion
	inline void insertElement(const T& element, const int location){ arr[location] = element; };

	//Access
	inline T& at(const int location){ return arr[location]; };
	inline T& operator[](int location){ return arr[location]; };

	//Size of Vector
	inline int size() const{ return containerSize; };

	//Display
	friend std::ostream& operator<<(std::ostream& os, Vector<T>& v){
		for (int i=0; i<v.size(); ++i)
			os << v[i];
		return os;
	};

private:
	int containerSize;

	//arr
	T* arr;
};

#include "VectorDefinition.h"

