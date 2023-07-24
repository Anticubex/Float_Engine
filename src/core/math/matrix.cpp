#include "matrix.h"
#include <iostream>

Matrix::Matrix(const size_t rows, const size_t cols) : rows(rows), cols(cols), size(rows * cols), values(size, (float)0) {
        if (values.size() != size) {
                throw std::runtime_error("Invalid vector initializer for matrix");
        }
}

Matrix::Matrix(const size_t rows, const size_t cols, const std::vector<float> vec) : rows(rows), cols(cols), size(rows * cols), values(vec) {
}

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

void Matrix::printout() const {
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
