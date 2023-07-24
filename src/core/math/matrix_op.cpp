#include "matrix.h"
#include "xmath.h"
#include <iostream>
#include <stdexcept>

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

        // Iterative (naive) algorithm
        // Abuse computer's base-2 number system + bitwise opertaions to quickly
        // check if the matrices are both square, equal in size, and powers of
        // two in size, to see if we can use strassen's algorithm

        if ((rows == other.rows) && (cols == other.cols) && xmath::isPow2(rows) && xmath::isPow2(cols)) {
                return strassens_mul(*this, other);
        }

        return naive_Iter_mul(*this, other);
}

int Matrix::naive_Iter_Mul_Func(const size_t row, const size_t col, const Matrix &A, const Matrix &B, Matrix &product) {
        float sum = 0;
        for (size_t i = 0; i < A.cols; i++) {
                sum += A.get(row, i) * B.get(i, col);
        }
        product(row, col) = sum;
        return 0;
}

Matrix Matrix::naive_Iter_mul(const Matrix &A, const Matrix &B) {
        size_t newRows = A.rows;
        size_t newCols = B.cols;
        Matrix product(newRows, newCols);
        iter2D<const Matrix &, const Matrix &, Matrix &>(newRows, newCols, &naive_Iter_Mul_Func, A, B, product);
        return product;
}

Matrix Matrix::strassens_mul(const Matrix &A, const Matrix &B) {

        std::cout << "\nDoing strassen_mul:\n";
        A.printout();
        std::cout << "\n";
        B.printout();
        std::cout << "\n";

        // Assumes matrices are square with rows and columns same powers of two
        // Private method, so it's safe-ish to do so

        /************************************************************************************************
         * Strassen's Decomposition Matrix Multiplaction Algorithm / Divide and Conquer Multiplication *
         * Core idea:                                                                                  *
         * | a b | * | e f | = | ae+bg af+bh | = | c1 c2 |                                             *
         * | c d |   | g h |   | ce+dg cf+dh |   | c3 c4 |                                             *
         * Both when a,b,c,d,e,f,g,h are matrices and scalars                                          *
         * Thus, one can divide the matrices into submatrices and solve recursively                    *
         ************************************************************************************************/

        // Pick an arbitrary size
        size_t size = A.rows;

        // Base case 1x1 / 2x2 for perf
        if (size <= 2) {
                if (size == 2) {
                        Matrix result(2, 2);
                        result(0, 0) = A.get(0, 0) * B.get(0, 0) + A.get(0, 1) * B.get(1, 0);
                        result(0, 1) = A.get(0, 0) * B.get(0, 1) + A.get(0, 1) * B.get(1, 1);
                        result(1, 0) = A.get(1, 0) * B.get(0, 0) + A.get(1, 1) * B.get(1, 0);
                        result(1, 1) = A.get(1, 0) * B.get(0, 1) + A.get(1, 1) * B.get(1, 1);
                        return result;
                }
                Matrix result(1, 1);
                result(0, 0) = A.get(0, 0) * B.get(0, 0);
                return result;
        }

        // size_t s2 = size / 2;
        size_t s2 = size >> 1;
        // I think this is where the difference between snake/hilbert/z-curve gets big,
        // If I go through the work of creating whole different "unsafe" algs for each method

        // Naive/safe method //

        /* Decompose the matrices */

        Matrix a(s2, s2), b(s2, s2), c(s2, s2), d(s2, s2), e(s2, s2), f(s2, s2), g(s2, s2), h(s2, s2);

        for (size_t m = 0; m < s2; m++) {
                for (size_t n = 0; n < s2; n++) {
                        a(m, n) = A.get(m, n);
                        b(m, n + s2) = A.get(m, n + s2);
                        c(m + s2, n) = A.get(m + s2, n);
                        d(m + s2, n + s2) = A.get(m + s2, n + s2);

                        e(m, n) = B.get(m, n);
                        f(m, n + s2) = B.get(m, n + s2);
                        g(m + s2, n) = B.get(m + s2, n);
                        h(m + s2, n + s2) = B.get(m + s2, n + s2);
                }
        }

        /* Actual Multiplication */
        Matrix c1 = strassens_mul(a, e) + strassens_mul(b, g);
        Matrix c2 = strassens_mul(a, f) + strassens_mul(b, h);
        Matrix c3 = strassens_mul(c, e) + strassens_mul(d, g);
        Matrix c4 = strassens_mul(c, f) + strassens_mul(d, h);

        /* Compose the matrices */
        Matrix result(size, size);
        for (size_t m = 0; m < s2; m++) {
                for (size_t n = 0; n < s2; n++) {
                        result(m, n) = c1(m, n);
                        result(m, n + s2) = c2(m, n);
                        result(m + s2, n) = c3(m, n);
                        result(m + s2, n + s2) = c4(m, n);
                }
        }

        // std::cout << "Result:";
        // result.printout();

        return result;
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