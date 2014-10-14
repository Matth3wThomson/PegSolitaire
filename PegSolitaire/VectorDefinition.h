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

