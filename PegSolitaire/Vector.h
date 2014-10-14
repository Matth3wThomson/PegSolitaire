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
	inline void insertElement(const T& element, const int location){ array[location] = element; };

	//Access
	inline T& at(const int location){ return array[location]; };
	inline T& operator[](int location){ return array[location]; };

	//Size of Vector
	inline int size() const{ return containerSize; };

	//Display
	friend std::ostream& operator<<(std::ostream& os, Vector<T>& v){
		for (int i=0; i<v.size(); ++i)
			os << v[i];
		return os;
	};

private:
	void init(const int size, const T& value = 0);
	int containerSize;

	//Array
	T* array;
};

#include "VectorDefinition.h"

