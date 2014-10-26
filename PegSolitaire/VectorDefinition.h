//Construction and Destruction

template<typename T>
Vector<T>::Vector(const int size):
	containerSize(size)
{
	this->arr = new T[containerSize];
	memset(this->arr, 0, (containerSize * sizeof(T)));
}

template<typename T>
Vector<T>::~Vector(void)
{
	delete[] arr;
}

template<typename T>
Vector<T>::Vector(const Vector<T>& rhs):
	containerSize(rhs.containerSize)
{
	arr = new T[containerSize];
	memcpy_s(arr, (containerSize*sizeof(T)), rhs.arr, (containerSize*sizeof(T)));
}

template<typename T>
Vector<T>::Vector(Vector<T>&& rval):
	arr(rval.arr), containerSize(rval.containerSize){
		rval.arr = 0;
		rval.containerSize = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& rhs){
	if (this == &rhs) return (*this);

	//Release old memory
	delete[] arr;

	this->containerSize = rhs.containerSize;

	//allocate new memory
	arr = new T[containerSize];
	memset(this->arr, 0, (containerSize * sizeof(T)));

	//Copy Values
	memcpy_s(arr, (containerSize*sizeof(T)), rhs.arr, (containerSize*sizeof(T)));

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& rval){
	std::swap(arr, rval.arr);
	std::swap(containerSize, rval.containerSize);

	return *this;
}

template<typename T>
template<typename E>
Vector<T>::Vector(const Vector<E>& rhs):
	containerSize(rhs.size()){

		arr = new T[containerSize];

	for (int i=0; i<containerSize; ++i){
		arr[i] = rhs[i];
	}
}

template<typename E>
bool operator==(const Vector<E>& lhs, const Vector<E>& rhs){
	if (lhs.size() != rhs.size()) return false;

	for (int i=0; i<lhs.size(); ++i)
		if (lhs[i] != rhs[i]) return false;

	return true;
}

template<typename E>
bool operator!=(const Vector<E>& lhs, const Vector<E>& rhs){
	return !(lhs == rhs);
}

template<typename E>
bool operator<(const Vector<E>& lhs, const Vector<E>& rhs){
	if (lhs.size() < rhs.size()) return true;
	if (rhs.size() < lhs.size()) return false;

	for (int i=0; i<lhs.size(); ++i){
		if (lhs[i] < rhs[i]) return true;
		if (lhs[i] > rhs[i]) return false;
	}

	return false;
}

template<typename T>
T Vector<T>::magnitude(){
	E sum;
	for (int i=0; i<this->containerSize; ++i)
		sum += (*this)[i] * (*this)[i];
	
	return sqrt(sum);
}

//Arithmetic
//T
template<typename E>
Vector<E> operator*(const Vector<E>& lhs, const E& rhs){
	Vector<E> temp(lhs);
	
	for (int i=0; i<temp.containerSize; ++i)
		temp[i] *= rhs;

	return temp;
};

template<typename E>
Vector<E> operator*(const E& lhs, const Vector<E>& rhs){
	return rhs * lhs;
};

template<typename T>
Vector<T>& Vector<T>::operator*=(const T& rhs){
	return *this = *this * rhs;
}


//Vector
template<typename E>
E operator*(const Vector<E>& lhs, const Vector<E>& rhs){
	if (lhs.containerSize != rhs.containerSize) throw std::invalid_argument("Dot product requires vectors of equal size");

	E value = E();

	for (int i=0; i<rhs.containerSize; ++i)
		value += lhs[i] * rhs[i];

	return value;
};

template<typename E, typename F>
E operator*(const Vector<E>& lhs, const Vector<F>& rhs){
	if (lhs.containerSize != rhs.containerSize) throw std::invalid_argument("Dot product requires vectors of equal size");

	E value = E();

	for (int i=0; i<rhs.containerSize; ++i)
		value += lhs[i] * rhs[i];

	return value;
}

template<typename E>
Vector<E> operator+(const Vector<E>& lhs, const Vector<E>& rhs){
	if (lhs.containerSize != rhs.containerSize) throw 
		std::invalid_argument("Vector addition requires equal length");

	Vector<E> temp(lhs);

	for (int i=0; i<rhs.containerSize; ++i)
		temp[i] += rhs[i];

	return temp;
};

template<typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& rhs){
	return *this = *this + rhs;
};

template<typename E>
Vector<E> operator-(const Vector<E>& lhs, const Vector<E>& rhs){
	if (lhs.containerSize != rhs.containerSize) 
		throw std::invalid_argument("Vector addition requires equal length");

	Vector<E> temp(lhs);

	for (int i=0; i<rhs.containerSize; ++i)
		temp[i] -= rhs[i];

	return temp;
};

template<typename T>
Vector<T>& Vector<T>::operator-=(const Vector<T>& rhs){
	return *this = *this - rhs;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& v){
	for (int i=0; i<v.size(); ++i)
			os << v[i] << ",";
		return os;
}
