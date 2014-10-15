//Construction and Destruction

template<typename T>
Vector<T>::Vector(const int size, const T& value):
	container_size(size)
{
	this->arr = new T[container_size];
	memset(this->arr, value, (container_size * sizeof(T)));
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
	arr = new T[];
	memcpy_s(arr, (container_size*sizeof(T)), rhs.arr, (container_size*sizeof(T)));
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Matrix<T>& rhs){
	if (this == &rhs) return (*this);

	//Release old memory
	delete[] arr;

	//allocate new memory
	arr = new T[container_size];
	memset(this->arr, 0, (size * sizeof(T)));
	
	//Copy Values
	for (int i=0; i<rhs.container_size; ++i){
		arr[i] = rhs[i];
	}
}
