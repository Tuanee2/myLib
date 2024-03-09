//
//  Matrix.cpp
//  Matrix
//
//  Created by tuan on 29/02/2024.
//

#include "Matrix.hpp"

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    // Khởi tạo ma trận với kích thước rows x cols, tất cả các phần tử được đặt là 0.
    data.resize(rows, std::vector<double>(cols, 0.0));
}

size_t Matrix::getRow(){
    return rows;
}

size_t Matrix::getCol(){
    return cols;
}

void Matrix::setValue(size_t row, size_t col, double value) {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Index out of range");
    }
    data[row][col] = value;
}

void Matrix::setMatrix(const double* arr, size_t arrSize) {
    if (arrSize != rows * cols) {
        throw std::invalid_argument("Number of elements in array does not match matrix dimensions");
    }

    size_t index = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = arr[index++];
        }
    }
}

void Matrix::setMatrix(const std::vector<double>& arr) {
    if (arr.size() != rows * cols) {
        throw std::invalid_argument("Number of elements in array does not match matrix dimensions");
    }

    size_t index = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = arr[index++];
        }
    }
}

void Matrix::assign(const Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices must have the same dimensions for assignment");
    }

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

double Matrix::getValue(size_t row, size_t col){
    return data[row][col];
}

Matrix Matrix::subMatrix(size_t rowToRemove, size_t colToRemove) const {
    if (rowToRemove >= rows || colToRemove >= cols) {
        throw std::out_of_range("Row or column index out of range");
    }

    Matrix result(rows - 1, cols - 1);
    size_t rowIndex = 0;
    for (size_t i = 0; i < rows; ++i) {
        if (i == rowToRemove) continue; // Skip the row to remove
        size_t colIndex = 0;
        for (size_t j = 0; j < cols; ++j) {
            if (j == colToRemove) continue; // Skip the column to remove
            result.data[rowIndex][colIndex] = data[i][j];
            ++colIndex;
        }
        ++rowIndex;
    }
    return result;
}

double Matrix::determinant() const {
    if (rows != cols) {
        throw std::invalid_argument("Matrix must be square to compute its determinant");
    }
    size_t n = rows;
    
    if(n == 1) return data[0][0];
    
    double det = 0;
    
    for (size_t j = 0; j < n; ++j) {
            // Get submatrix by excluding the first row and jth column
            Matrix submatrix(n - 1, n - 1);
            for (size_t row = 1; row < n; ++row) {
                for (size_t col = 0; col < n; ++col) {
                    if (col < j) {
                        submatrix.data[row - 1][col] = data[row][col];
                    } else if (col > j) {
                        submatrix.data[row - 1][col - 1] = data[row][col];
                    }
                }
            }
            // Calculate the determinant recursively
            double sub_det = submatrix.determinant();
            // Add or subtract the determinant of the submatrix times the corresponding element of the first row
            det += (j % 2 == 0 ? 1 : -1) * data[0][j] * sub_det;
        }
    return det;
}

size_t Matrix::rank() const{
    if (rows == cols){
        if(determinant() != 0){
            return rows;
        }
    }
    size_t result = 0;
    Matrix swap = *this;
    swap.assign(swap.convertToTriangular());
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if((swap.data[i][j]>1e-5) || (swap.data[i][j]<-1e-5)){
                result++;
                break;
            }
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Number of columns in the first matrix must be equal to the number of rows in the second matrix");
    }

    Matrix result(rows, other.cols);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < other.cols; ++j) {
            for (size_t k = 0; k < cols; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }

    return result;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices must have the same dimensions for addition");
    }

    Matrix result(rows, cols);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }

    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrices must have the same dimensions for subtraction");
    }

    Matrix result(rows, cols);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }

    return result;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) { // Kiểm tra để tránh tự gán
        rows = other.rows;
        cols = other.cols;
        data = other.data;
    }
    return *this;
}

Matrix Matrix::transpose() const {
    Matrix transposed(cols, rows); // Chuyển vị sẽ có kích thước ngược lại
        for (size_t i = 0; i < cols; ++i) {
            for (size_t j = 0; j < rows; ++j) {
                transposed.data[i][j] = data[j][i]; // Đảo vị trí hàng và cột
            }
        }
    return transposed;
}

Matrix Matrix::inverse() const{
    if (rows != cols) {
        throw std::invalid_argument("Matrix must be square to compute its inverse");
    }
    double det = determinant();

    if (det == 0.0) {
        throw std::invalid_argument("Matrix is singular, cannot compute its inverse");
    }

    size_t n = rows;
    Matrix inv(n, n);

        // Calculate adjugate matrix
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            // Get submatrix by excluding ith row and jth column
            Matrix submatrix = this->subMatrix(i, j);
            // Calculate determinant of submatrix
            double sub_det = submatrix.determinant();
            // Calculate cofactor and adjugate
            double cofactor = (i + j) % 2 == 0 ? sub_det : -sub_det;
            inv.data[j][i] = cofactor / det;
        }
    }

    return inv;
}

Matrix Matrix::convertToTriangular(){
    Matrix trans = *this;
    size_t check = 0;
    for(int i=0;i<cols;i++){
        size_t pivot_row = check ;
        while ((pivot_row<rows) && (data[pivot_row][i] == 0)){
            pivot_row++;
        }
        
        if(pivot_row == rows){
            continue;
        }
        std::swap(trans.data[check],trans.data[pivot_row]);

        double pivot_value = trans.data[check][i];
        for(size_t j=check+1;j<rows;j++){
            double factor = trans.data[j][i]/pivot_value;
            for(size_t k=i;k<cols;k++){
                trans.data[j][k] -= trans.data[check][k]*factor;
            }
        }
        
        check ++;
    }
    return trans;
}

void Matrix::swapRows(size_t row1, size_t row2) {
    if (row1 < rows && row2 < rows) {
        std::swap(data[row1], data[row2]);
    } else {
        std::cerr << "Invalid row indices!" << std::endl;
    }
}

// Phương thức đổi hai cột của ma trận
void Matrix::swapCols(size_t col1, size_t col2) {
    if (col1 < cols && col2 < cols) {
        for (size_t i = 0; i < rows; ++i) {
            std::swap(data[i][col1], data[i][col2]);
        }
    } else {
        std::cerr << "Invalid column indices!" << std::endl;
    }
}


bool Matrix::isSingleElementMatrix() const {
    return (rows == 1 && cols == 1);
}

double Matrix::getSingleElementValue() const {
    if (!isSingleElementMatrix()) {
        throw std::logic_error("Matrix is not a single element matrix");
    }
    return data[0][0];
}

Matrix Matrix::identity(size_t size) {
    Matrix identity(size, size);
    for (size_t i = 0; i < size; ++i) {
        identity.data[i][i] = 1.0; // Diagonal elements set to 1
    }
    return identity;
}

Matrix Matrix::zeros(size_t rows, size_t cols) {
    return Matrix(rows, cols); // Automatically initializes to 0
}

void Matrix::print() const {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// LỚP MATRIX_ARRAY
void MatrixArray::addMatrix(const Matrix& mat) {
    matrices.push_back(mat);
}

Matrix& MatrixArray::getMatrix(size_t index) {
    if (index >= matrices.size()) {
        throw std::out_of_range("Index out of range");
    }
    return matrices[index];
}

void MatrixArray::printAllMatrices() const {
    for (size_t i = 0; i < matrices.size(); ++i) {
        std::cout << "Matrix " << i + 1 << ":" << std::endl;
        matrices[i].print();
        std::cout << std::endl;
    }
}
