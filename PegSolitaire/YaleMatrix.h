#include "Matrix.h"
#include <vector>

//http://en.wikipedia.org/wiki/Sparse_matrix
//Inheritance not used as this format of sparse matrix does not need to,
//and will not do well in implementing all features of a general matrix.

template<typename T>
class YaleMatrix{

public:
	YaleMatrix();
	YaleMatrix(const YaleMatrix<T>& rhs);
	YaleMatrix(YaleMatrix<T>&& rval);
	YaleMatrix(const Matrix<T>& verbose);
	~YaleMatrix();

	YaleMatrix<T>& operator=(const YaleMatrix<T>& rhs);
	YaleMatrix<T>& operator=(YaleMatrix<T>&& rval);

	template<typename E>
	friend Vector<E> operator*(const YaleMatrix<E>& lhs, const Vector<E>& rhs);

	template<typename E, typename F>
	friend Vector<F> operator*(const YaleMatrix<E>& lhs, const Vector<F>& rhs);

	T at(int x, int y);

	int get_x_dim() const{ return x-1; };
	int get_y_dim() const{ return y; };
	int get_NNZ() const{ return IA[x-1]; };


	friend std::ostream& operator<<(std::ostream& os, const YaleMatrix<T>& ym){
		os << "A: [";
		for (int i=0; i<ym.IA[ym.x-1]; ++i)
			os << ym.A[i] << ",";
		os << "]\nIA: [";
		for (int i=0; i<ym.x; ++i)
			os << ym.IA[i] << ",";
		os << "]\nJA: [";
		for (int i=0; i<ym.IA[ym.x-1]; ++i)
			os << ym.JA[i] << ",";
		os << "]\nNon Zero Elements: " << ym.IA[ym.x-1];

		return os;
	};

private:
	T* A; //1D array of non zero elements in matrix
	int* IA; //1D array of indices of elements on each row i. IA[x-1] contains NNZ
	int* JA; //1D array of columns for each index of A
	int x;	//Size of IA. number of rows + 1
	int y;	//Number of columns

	struct KeyValue {
		const T* value;
		int yindex;

		KeyValue(const T* val, int y):value(val), yindex(y) {};
	};
};

#include "YaleMatrixDef.h"