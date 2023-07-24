#include "matrix.h"
#include <stdexcept>

#include <iostream>

Matrix::Matrix(const size_t rows, const size_t cols) : rows(rows), cols(cols), size(rows * cols), values(size, (float)0) {}

// TODO: Create implementations for snake, hilbert, and z-curves + benchmark

float &Matrix::operator()(const size_t row, const size_t col) {
        // Snake
        return values[row + col * rows];
        // TODO: Hilbert
        // TODO: Z-curve
}

float Matrix::get(const size_t row, const size_t col) const {
        // Snake
        return values[row + col * rows];
        // TODO: Hilbert
        // TODO: Z-curve
}

Matrix Matrix::operator+(const Matrix &other) const {
        if (rows != other.rows || cols != other.cols) {
                throw std::runtime_error("Matrix Sizes Incompatible");
        }
        Matrix sum(rows, cols);
        for (size_t i = 0; i < size; i++) {
                sum.values[i] = values[i] + other.values[i];
        }
        return sum;
}

// TODO: implement multiple matrix multiplication algorithms and benchmark

Matrix Matrix::operator*(const Matrix &other) const {
        if (cols != other.rows) {
                throw std::runtime_error("Matrix Sizes Incompatible");
        }
        size_t newRows = rows;
        size_t newCols = other.cols;
        // std::cout << "(" << rows << ", " << cols << ") * ("
        // << other.rows << ", " << other.cols << ") = ("
        // << newRows << ", " << newCols << ")";
        Matrix product(newRows, newCols);

        // Iterative (naive) algorithm
        iter2D<const Matrix &, const Matrix &, Matrix &>(newRows, newCols, &naive_Iter_mul, *this, other, product);

        return product;
}

int Matrix::naive_Iter_mul(const size_t row, const size_t col, const Matrix &A, const Matrix &B, Matrix &product) {
        float sum = 0;
        for (size_t i = 0; i < A.cols; i++) {
                sum += A.get(row, i) * B.get(i, col);
        }
        product(row, col) = sum;
        return 0;
}

template <class... Types>
int Matrix::iter2D(const size_t width, const size_t height, int (*function)(const size_t, const size_t, Types...), Types... args) {
        int i = 0;
        for (size_t row = 0; row < width; row++) {
                for (size_t col = 0; col < height; col++) {
                        if ((i = function(row, col, args...)))
                                return i;
                }
        }
        return i;
}

void Matrix::printout() {
        using namespace std;
        cout << "(" << rows << ", " << cols << ") Matrix:\n";
        for (size_t row = 0; row < rows; row++) {
                cout << "|\t";
                for (size_t col = 0; col < cols; col++) {
                        cout << this->get(row, col) << "\t";
                }
                cout << "|\n";
        }
}