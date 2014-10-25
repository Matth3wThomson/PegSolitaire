//Construction and Destruction

template<typename T>
Vector<T>::Vector(const int size):
	container_size(size)
{
	this->arr = new T[container_size];
	memset(this->arr, 0, (container_size * sizeof(T)));
}

template<typename T>
Vector<T>::~Vector(void)
{
	delete[] arr;
}

template<typename T>
Vector<T>::Vector(const Vector<T>& rhs):
	container_size(rhs.container_size)
{
	arr = new T[container_size];
	memcpy_s(arr, (container_size*sizeof(T)), rhs.arr, (container_size*sizeof(T)));
}

template<typename T>
Vector<T>::Vector(Vector<T>&& rval):
	arr(rval.arr), container_size(rval.container_size){
		rval.arr = 0;
		rval.container_size = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& rhs){
	if (this == &rhs) return (*this);

	//Release old memory
	delete[] arr;

	this->container_size = rhs.container_size;

	//allocate new memory
	arr = new T[container_size];
	memset(this->arr, 0, (container_size * sizeof(T)));

	//Copy Values
	/*for (int i=0; i<rhs.container_size; ++i){
		arr[i] = rhs.arr[i];
	}*/

	memcpy_s(arr, (container_size*sizeof(T)), rhs.arr, (container_size*sizeof(T)));

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& rval){
	std::swap(arr, rval.arr);
	std::swap(container_size, rval.container_size);

	return *this;
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

	for (int i=0; i<lhs.size(); ++i)
		if (lhs[i] != rhs[i])
			return lhs[i] < rhs[i];
		
	return false;
}

template<typename T>
T Vector<T>::magnitude(){
	E sum;
	for (int i=0; i<this->container_size; ++i)
		sum += (*this)[i] * (*this)[i];
	
	return sqrt(sum);
}

//Arithmetic
//T
template<typename E>
Vector<E> operator*(const Vector<E>& lhs, const E& rhs){
	Vector<E> temp(lhs);
	
	for (int i=0; i<temp.container_size; ++i)
		temp[i] *= rhs;

	return temp;
};

template<typename E>
Vector<E> operator*(const E& lhs, const Vector<E>& rhs){
	return rhs * lhs;
};


//Vector
template<typename E>
E operator*(const Vector<E>& lhs, const Vector<E>& rhs){
	if (lhs.container_size != rhs.container_size) throw std::invalid_argument("Dot product requires vectors of equal size");

	E value = E();

	for (int i=0; i<rhs.container_size; ++i)
		value += lhs[i] * rhs[i];

	return value;
};

template<typename E, typename F>
E operator*(const Vector<E>& lhs, const Vector<F>& rhs){
	if (lhs.container_size != rhs.container_size) throw std::invalid_argument("Dot product requires vectors of equal size");

	E value = E();

	for (int i=0; i<rhs.container_size; ++i)
		value += lhs[i] * rhs[i];

	return value;
}

template<typename E>
Vector<E> operator+(const Vector<E>& lhs, const Vector<E>& rhs){
	if (lhs.container_size != rhs.container_size) throw 
		std::invalid_argument("Vector addition requires equal length");

	Vector<E> temp(lhs);

	for (int i=0; i<rhs.container_size; ++i)
		temp[i] += rhs[i];

	return temp;
};

template<typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& rhs){
	return *this = *this + rhs;
};

template<typename E>
Vector<E> operator-(const Vector<E>& lhs, const Vector<E>& rhs){
	if (lhs.container_size != rhs.container_size) 
		throw std::invalid_argument("Vector addition requires equal length");

	Vector<E> temp(lhs);

	for (int i=0; i<rhs.container_size; ++i)
		temp[i] -= rhs[i];

	return temp;
};

template<typename T>
Vector<T>& Vector<T>::operator-=(const Vector<T>& rhs){
	return *this = *this - rhs;
};
