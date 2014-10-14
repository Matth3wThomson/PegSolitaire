//Construction and Destruction

template<typename T>
Vector<T>::Vector(const int size, const T& value)
{
	init(size, value);
}

template<typename T>
void Vector<T>::init(const int size, const T& value){
	this->array = new T[size];
	memset(this->array, value, (size * sizeof(T)));
	containerSize = size;
}

template<typename T>
Vector<T>::~Vector(void)
{
	delete[] array;
}

template<typename T>
Vector<T>::Vector(const Vector<T>& rhs){
	init(rhs.size);
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Matrix<T>& rhs){
	if (this == &rhs) return (*this);

	//Release old memory
	delete[] array;

	//allocate new memory
	array = new T[containerSize];
	memset(this->array, 0, (size * sizeof(T)));
	
	//Copy Values
	for (int i=0; i<rhs.containerSize; ++i){
		array[i] = rhs[i];
	}
}
