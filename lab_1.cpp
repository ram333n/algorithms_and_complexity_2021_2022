#include <iostream>
#include <iomanip>
#include <vector>

class Matrix {
private:
    double** matrix;
    size_t size_;
public:
    Matrix() {
        matrix = nullptr;
        size_ = 0;
    }

    Matrix(size_t new_size) {
        size_ = new_size;
        matrix = new double* [size_];
        for (size_t i = 0; i < size_; ++i) {
            matrix[i] = new double[size_] {0};
        }
    }

    double* operator[](size_t idx) {
        return matrix[idx];
    }

    const double* operator[](size_t idx) const {
        return matrix[idx];
    }

    size_t Size() const {
        return size_;
    }

    ~Matrix() {
        for (size_t i = 0; i < size_; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
};

void PrintMatrix(const Matrix& m) {
    for (size_t i = 0; i < m.Size(); ++i) {
        for (size_t j = 0; j < m.Size(); ++j) {
            std::cout << std::setprecision(5) << std::setw(12) << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void GetLUDecomposition(const Matrix& matrix, Matrix& l_matrix, Matrix& u_matrix) {
    int matrix_size = matrix.Size();/*sizeof(matrix) / sizeof(double)*/

	for (int i = 0; i < matrix_size; ++i) {
		for (int j = 0; j < matrix_size; ++j) {
			u_matrix[i][j] = matrix[i][j];
		}
	}

	for (int i = 0; i < matrix_size; ++i) {
		for (int j = i; j < matrix_size; ++j) {
			l_matrix[j][i] = u_matrix[j][i] / u_matrix[i][i];
		}
	}

	for (int k = 1; k < matrix_size; ++k) {
		for (int i = k - 1; i < matrix_size; ++i) {
			for (int j = i; j < matrix_size; ++j) {
				l_matrix[j][i] = u_matrix[j][i] / u_matrix[i][i];
			}
		}

		for (int i = k; i < matrix_size; ++i) {
			for (int j = k - 1; j < matrix_size; j++) {
				u_matrix[i][j] = u_matrix[i][j] - l_matrix[i][k - 1] * u_matrix[k - 1][j];
			}
		}
	}
}

void SolveTheMatrixEquationForLower(const Matrix& lower, const Matrix& rhs_matrix, Matrix& result) {
    const int size = lower.Size();
    for (int k = 0; k < size; ++k) {
        result[0][k] = rhs_matrix[0][k] / lower[0][0];
        for (int i = 1; i < size; ++i) {
            double temp = 0;
            for (int j = 0; j < i; ++j) {
                temp += lower[i][j] * result[j][k];
            }
            result[i][k] = (rhs_matrix[k][i] - temp) / lower[i][i];
        }
    }
}


void SolveTheMatrixEquationForUpper(const Matrix& upper, const Matrix& rhs_matrix, Matrix& result) {
    const int size = upper.Size();
    for (int k = 0; k < size; ++k) {
        result[size - 1][k] = rhs_matrix[size - 1][k] / upper[size - 1][size - 1];
        for (int i = size - 2; i > -1; --i) {
            double temp = 0;
            for (int j = i + 1; j < size; ++j) {
                temp += upper[i][j] * result[j][k];
            }
            result[i][k] = (rhs_matrix[i][k] - temp) / upper[i][i];
        }
    }
}

//void MultilpyMatrices(const Matrix)

void GetInvertedMatrix(const Matrix& matrix) {
    const int size = matrix.Size();
    Matrix inverse_matrix(size);

    for (int i = 0; i < size; ++i) {
        inverse_matrix[i][i] = 1;
    }

    Matrix l_matrix(size), u_matrix(size);
    GetLUDecomposition(matrix, l_matrix, u_matrix);
    std::cout << "Matrix to inverse : " << std::endl;
    PrintMatrix(matrix);
    std::cout << std::endl << "L matrix : " << std::endl;
    PrintMatrix(l_matrix);
    std::cout << std::endl << "U matrix : " << std::endl;
    PrintMatrix(u_matrix);
    std::cout << std::endl;

    Matrix rhs_matrix(size);
    SolveTheMatrixEquationForLower(l_matrix, inverse_matrix, rhs_matrix);
    SolveTheMatrixEquationForUpper(u_matrix, rhs_matrix, inverse_matrix);

    std::cout <<"Inverted matrix"<< std::endl;
    PrintMatrix(inverse_matrix);
    std::cout << "-------------------------------------------" << std::endl;
}




//void PrintMatrix(double** m) {
//	int matrix_size = sizeof(m) / sizeof(double);
//
//	for (size_t i = 0; i < matrix_size; ++i) {
//		for (size_t j = 0; j < matrix_size; ++j) {
//			std::cout << m[i][j] << " ";
//		}
//		std::cout << std::endl;
//	}
//}

int main(){

    {
        Matrix m(2);
        m[0][0] = sqrt(2);
        m[0][1] = sqrt(3);

        m[1][0] = sqrt(12);
        m[1][1] = 5;

        GetInvertedMatrix(m);
    }

    {
        Matrix m(3);
        m[0][0] = 2.3;
        m[0][1] = 4;
        m[0][2] = 1;

        m[1][0] = 4.5;
        m[1][1] = 12;
        m[1][2] = 10.333;

        m[2][0] = 3;
        m[2][1] = -4;
        m[2][2] = -10.72;

        GetInvertedMatrix(m);
    }

    {
        Matrix m(4);

        m[0][0] = 2;
        m[0][1] = 3;
        m[0][2] = 1;
        m[0][3] = 5;

        m[1][0] = 6;
        m[1][1] = 13;
        m[1][2] = 5;
        m[1][3] = 19;

        m[2][0] = 2;
        m[2][1] = 19;
        m[2][2] = 10;
        m[2][3] = 23;

        m[3][0] = 4;
        m[3][1] = 10;
        m[3][2] = 11;
        m[3][3] = 31;


        GetInvertedMatrix(m);
    }

    {
        std::vector<double> source = {
            10.3, 12, 6 ,-7, 5.05,
            -10, 4.241 ,9, -134, 2,
            7.41, 5.13, 1.33, 55, -2,
            -2, 1, 3 ,5 ,6,
            9.41, 23, 9.413, 22, 4
        };

        Matrix m(5);

        for (size_t i = 0; i < source.size(); ++i) {
            m[i / m.Size()][i % m.Size()] = source[i];
        }   
        GetInvertedMatrix(m);
    }

}




//GetLUDecomposition(a, l, u);

//PrintMatrix(a);
//std::cout << std::endl;
//PrintMatrix(l);
//std::cout << std::endl;
//PrintMatrix(u);

//Matrix e(l.Size());

//for (int i = 0; i < e.Size(); ++i) {
//    e[i][i] = 1;
//}

//Matrix aboba(4);
//SolveTheMatrixEquationForUpper(u, e, aboba);
//PrintMatrix(aboba);