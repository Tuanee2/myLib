//
//  Matrix.hpp
//  Matrix
//
//  Created by tuan on 29/02/2024.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#define Matrix_Version "0.0.1"  // sửa 1/3/2024

#include <iostream>
#include <stdio.h>
#include <vector>

class Matrix {
private:
    std::vector<std::vector<double>> data;
    size_t rows;
    size_t cols;
public:
    Matrix(size_t rows, size_t cols);
    size_t getRow(),
           getCol();
    
    // Khai báo tham số ma trận
    void setValue(size_t row, size_t col, double value),
         setMatrix(const double* arr, size_t arrSize),
         setMatrix(const std::vector<double>& arr);
    
    // Gán giá trị tham số 2 ma trận cùng kích thước
    void assign(const Matrix& other);
    
    // lấy giá trị tham số ma trận 
    double getValue(size_t row, size_t col);
    
    // tính định thức ma trận
    Matrix subMatrix(size_t rowToRemove, size_t colToRemove) const; // lấy ra ma trận con với kích thước (rowToRemove x colToRemove)
    double determinant() const;
    
    // tìm hạng của ma trận
    size_t rank() const;
    
    // Các toán tử ma trận
    Matrix operator*(const Matrix& other) const, // nhân 2 ma trận
           operator*(double scalar) const,       // nhân ma trận với 1 số
           operator+(const Matrix& other) const, // cộng 2 ma trận
           operator-(const Matrix& other) const; // trừ 2 ma trận
    Matrix& operator=(const Matrix& other);
    // Các phép biến đổi ma trận
    Matrix transpose() const,  // phép chuyển vị ma trận
           inverse() const,    // tính ma trận nghịch đảo
           convertToTriangular(); // chuyển về tam giác để tính rank mà ko làm thay đổi giá trị ma trận
    
    void swapRows(size_t row1, size_t row2), // đổi vị trí 2 hàng
         swapCols(size_t col1, size_t col2); // đổi vị trí 2 cột
    
    // Các phép kiểm tra ma trận
    bool isSingleElementMatrix() const; // kiểm tra xem ma trận có kích thước 1x1 hay ko
    double getSingleElementValue() const; // chuyển thành kiểu double khi ma trận có kích thước 1x1
    // Các ma trận đặc biệt
    static Matrix identity(size_t size), // ma trận đơn vị cỡ (size x size)
                  zeros(size_t rows, size_t cols); // ma trận 0 cỡ (rows x cols)
    
    // In ma trận
    void print() const;
};

class MatrixArray {
private:
    std::vector<Matrix> matrices; // Sử dụng vector để lưu trữ một mảng các ma trận

public:
    // Phương thức thêm ma trận vào mảng
    void addMatrix(const Matrix& mat);
    // Phương thức lấy ma trận từ mảng
    Matrix& getMatrix(size_t index);
    // Phương thức in tất cả các ma trận trong mảng
    void printAllMatrices() const;
};

#endif /* Matrix_hpp */
