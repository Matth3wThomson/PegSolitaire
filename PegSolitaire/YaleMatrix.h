#include "Matrix.h"
#include <vector>
//http://en.wikipedia.org/wiki/Sparse_matrix


//TODO: Put x at the end of the IA array... or find way to improve find function
//TODO: Make use of struct more efficient.
template<typename T>
class YaleMatrix{
public:
	YaleMatrix(const Matrix<T>& verbose);

	T& at(int x, int y);

	friend std::ostream& operator<<(std::ostream& os, const YaleMatrix<T>& ym){
		os << "A: [";
		for (int i=0; i<ym.NZ; ++i)
			os << ym.A[i] << ",";
		os << "]\nIA: [";
		for (int i=0; i<ym.x; ++i)
			os << ym.IA[i] << ",";
		os << "]\nJA: [";
		for (int i=0; i<ym.NZ; ++i)
			os << ym.JA[i] << ",";
		os << "]\nNon Zero Elements: " << ym.NZ;

		return os;
	};

private:
	T* A; //1D array of non zero elements in matrix
	int* IA; //1D array of indices of elements on each row i
	int* JA; //1D array of columns for each index of A
	int NZ;
	int x;	//Size of IA
	int y;	

	struct KeyValue {
		T value;
		int yindex;

		KeyValue(T& val, int y):value(val), yindex(y) {};
	};
};

#include "YaleMatrixDef.h"