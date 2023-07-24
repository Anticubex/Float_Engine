#include "xmath.h"
#include <cstdint>
#include <stdexcept>

float xmath::fastInvSqrt(const float x) {
        float xhalf = 0.5f * x;
        float y = x;

        uint32_t i = *(uint32_t *)&y;   // store floating-point bits in integer
        i = 0x5f3759df - (i >> 1);      // initial guess for Newton's method
        y = *(float *)&i;               // convert new bits into float
        y = y * (1.5f - xhalf * y * y); // One round of Newton's method
        return y;
}

float xmath::precise_fastInvSqrt(const float x, const size_t rounds) {
        float xhalf = 0.5f * x;
        float y = x;

        uint32_t i = *(uint32_t *)&y; // store floating-point bits in integer
        i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
        y = *(float *)&i;             // convert new bits into float
        for (size_t i = 0; i < rounds; i++) {
                y = y * (1.5f - xhalf * y * y); // One round of Newton's method
        }
        return y;
}

xmath::Matrix::Matrix(const size_t rows, const size_t cols) : rows(rows), cols(cols), size(rows * cols), values(size, (float)0) {}

// TODO: Create implementations for snake, hilbert, and z-curves + benchmark

float &xmath::Matrix::operator()(const size_t row, const size_t col) {
        // Snake
        return values[row + col * rows];
        // TODO: Hilbert
        // TODO: Z-curve
}

xmath::Matrix xmath::Matrix::operator+(const Matrix &other) {
        if (rows != other.rows || cols != other.cols) {
                throw std::runtime_error("Matrix Sizes Incompatible");
        }
        Matrix sum(rows, cols);
        for (size_t i = 0; i < size; i++) {
                sum.values[i] = values[i] + other.values[i];
        }
        return sum;
}

// TODO: implement multiple matrix multiplication algorithms

xmath::Matrix xmath::Matrix::operator*(const Matrix &other) {
        if (cols != other.rows) {
                throw std::runtime_error("Matrix Sizes Incompatible");
        }
        size_t newRows = rows;
        size_t newCols = other.cols;
        Matrix product(rows, cols);

        // Iterative (naive) algorithm

        return product;
}

int xmath::Matrix::iter2D(size_t width, size_t height, int (*function)(size_t, size_t)) {
        int i;
        for (size_t row = 0; row < width; row++) {
                for (size_t col = 0; col < height; col++) {
                        if (i = function(row, col))
                                return i;
                }
        }
}
