#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

//Клас для опису матриці
class Matrix {
private:
    double** matrix;
    size_t size_;
public:
    Matrix() {
        matrix = nullptr;
        size_ = 0;
    }

    Matrix(size_t new_size) {//конструктор, у якому виділяється пам'ять для матриці
        size_ = new_size;
        matrix = new double* [size_];
        for (size_t i = 0; i < size_; ++i) {
            matrix[i] = new double[size_] {0};
        }
    }

    double* operator[](size_t idx) {//оператор [] для доступу до елементів матриці
        return matrix[idx];
    }

    const double* operator[](size_t idx) const { // константна версія оператора []
        return matrix[idx];
    }

    size_t Size() const { //метод для отримання порядку матриці
        return size_;
    }

    ~Matrix() { //деструктор для очищення пам'яті
        for (size_t i = 0; i < size_; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
};

//Допоміжна функція, яка друкує матрицю в консоль
void PrintMatrix(const Matrix& m) {
    for (size_t i = 0; i < m.Size(); ++i) {
        for (size_t j = 0; j < m.Size(); ++j) {
            std::cout << std::setprecision(5) << std::setw(12) << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

//Функція, яка виконує LU розклад, записуючи в l_matrix утворену нижньотрикутну матрицю, а в u_matrix - верхньотрикутну
void GetLUDecomposition(const Matrix& matrix, Matrix& l_matrix, Matrix& u_matrix) {
    int matrix_size = matrix.Size();

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
//Функція для знаходження матриці, добуток на яку з нижньотрикутною матрицею lower дає матрицю rhs_matrix
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

//Функція, яка виконує те ж саме, що й попередня, тільки матриця lower замінюється на верхньотрикутну upper
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

//Функція для знаходження оберненої матриці
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

int main_1(){

    std::ifstream input;
    int size;
    input.open("input.txt");

    input >> size;
    Matrix m(size);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            input >> m[i][j];
        }
    }

    GetInvertedMatrix(m);   
    return 0;
};