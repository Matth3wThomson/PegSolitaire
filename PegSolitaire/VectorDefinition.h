//Construction and Destruction

template<typename T>
Vector<T>::Vector(const int size, const T& value):
	containerSize(size)
{
	this->arr = new T[containerSize];
	memset(this->arr, value, (containerSize * sizeof(T)));
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
	arr = new T[];
	memcpy_s(arr, (containerSize*sizeof(T)), rhs.arr, (containerSize*sizeof(T)));
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Matrix<T>& rhs){
	if (this == &rhs) return (*this);

	//Release old memory
	delete[] arr;

	//allocate new memory
	arr = new T[containerSize];
	memset(this->arr, 0, (size * sizeof(T)));
	
	//Copy Values
	for (int i=0; i<rhs.containerSize; ++i){
		arr[i] = rhs[i];
	}
}
