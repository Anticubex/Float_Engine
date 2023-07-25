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

Matrix Matrix::operator-(const Matrix &other) const {
        if (rows != other.rows || cols != other.cols) {
                throw std::runtime_error("Matrix Sizes Incompatible");
        }
        Matrix sum(rows, cols);
        for (size_t i = 0; i < size; i++) {
                sum.values[i] = values[i] - other.values[i];
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

        if (xmath::isPow2(rows) && (rows == other.rows) && (cols == other.cols) && (cols == rows)) {
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

int printoutfloats(const std::vector<float> vals);

Matrix Matrix::strassens_mul(const Matrix &A, const Matrix &B) {

        using namespace std;
        // cout << "\nDoing strassen_mul:\n";
        // A.printout();
        // cout << "\n";
        // B.printout();
        // cout << "\n";

        // Assumes matrices are square with rows and columns same powers of two
        // Private method, so it's safe-ish to do so

        /***********************************************************************************************
         * Strassen's Decomposition Matrix Multiplaction Algorithm / Divide and Conquer Multiplication *
         * Core idea:                                                                                  *
         * | a b | * | e f | = | ae+bg af+bh | = | c1 c2 |                                             *
         * | c d |   | g h |   | ce+dg cf+dh |   | c3 c4 |                                             *
         * Both when a,b,c,d,e,f,g,h are matrices and scalars                                          *
         * Thus, one can divide the matrices into submatrices and solve recursively                    *
         * -                                                                                           *
         * A second possible optimization can be achieve like so:                                      *
         * ae + bg = (a + d)(e + h) + d(g - e) - (a + b)h + (b - d)(g + h)                             *
         * af + bh = a(f - h) + (a + b)h                                                               *
         * ce + dg = (c + d)e + d(g - e)                                                               *
         * cf + dh = a(f - h) + (a + d)(e + h) - (c + d)e - (a - c)(e + f)                             *
         * -                                                                                           *
         * Thus, only 7 multiplications are necesary:                                                  *
         * p1 = (a + d)(e + h)          p5 = a(f - h)                                                  *
         * p2 = d(g - e)                p6 = (c + d)e                                                  *
         * p3 = (a + b)h                p7 = (a - c)(e + f)                                            *
         * p4 = (b - d)(g + h)                                                                         *
         ***********************************************************************************************/

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

        // Naive/safe/snake-curve major method //

        /* Decompose the matrices */

        Matrix a(s2, s2), b(s2, s2), c(s2, s2), d(s2, s2), e(s2, s2), f(s2, s2), g(s2, s2), h(s2, s2);
        // cout << "A: ";
        // a.printout();
        // cout << "B: ";
        // b.printout();
        // cout << "C: ";
        // c.printout();
        // cout << "D: ";
        // d.printout();
        // cout << "E: ";
        // e.printout();
        // cout << "F: ";
        // f.printout();
        // cout << "G: ";
        // g.printout();
        // cout << "H: ";
        // h.printout();

        // cout << s2 << "  " << size << "\n";

        for (size_t m = 0; m < s2; m++) {
                for (size_t n = 0; n < s2; n++) {
                        a(m, n) = A.get(m, n);
                        // cout << "assigned a\n";
                        // printoutfloats(a.values);
                        b(m, n) = A.get(m, n + s2);
                        // cout << "assigned b\n";
                        // printoutfloats(b.values);
                        c(m, n) = A.get(m + s2, n);
                        // cout << "assigned c\n";
                        // printoutfloats(c.values);
                        d(m, n) = A.get(m + s2, n + s2);
                        // cout << "assigned d\n";
                        // printoutfloats(d.values);

                        e(m, n) = B.get(m, n);
                        // cout << "assigned e\n";
                        // printoutfloats(e.values);
                        f(m, n) = B.get(m, n + s2);
                        // cout << "assigned f\n";
                        // printoutfloats(f.values);
                        g(m, n) = B.get(m + s2, n);
                        // cout << "assigned g\n";
                        // printoutfloats(g.values);
                        h(m, n) = B.get(m + s2, n + s2);
                        // cout << "assigned h\n";
                        // printoutfloats(h.values);

                        // cout << m << ", " << n << "\n";

                        // cout << "A: ";
                        // a.printout();
                        // cout << "B: ";
                        // b.printout();
                        // cout << "C: ";
                        // c.printout();
                        // cout << "D: ";
                        // d.printout();
                        // cout << "E: ";
                        // e.printout();
                        // cout << "F: ";
                        // f.printout();
                        // cout << "G: ";
                        // g.printout();
                        // cout << "H: ";
                        // h.printout();
                }
        }

        // cout << "Decomposed into following:\n";
        // cout << "A: ";
        // a.printout();
        // cout << "B: ";
        // b.printout();
        // cout << "C: ";
        // c.printout();
        // cout << "D: ";
        // d.printout();
        // cout << "E: ";
        // e.printout();
        // cout << "F: ";
        // f.printout();
        // cout << "G: ";
        // g.printout();
        // cout << "H: ";
        // h.printout();

        /* Actual Multiplication */
        // Boring way
        // Matrix c1 = strassens_mul(a, e) + strassens_mul(b, g);
        // Matrix c2 = strassens_mul(a, f) + strassens_mul(b, h);
        // Matrix c3 = strassens_mul(c, e) + strassens_mul(d, g);
        // Matrix c4 = strassens_mul(c, f) + strassens_mul(d, h);

        // Optimized way
        Matrix p1 = (a + d) * (e + h);
        Matrix p2 = d * (g - e);
        Matrix p3 = (a + b) * h;
        Matrix p4 = (b - d) * (g + h);
        Matrix p5 = a * (f - h);
        Matrix p6 = (c + d) * e;
        Matrix p7 = (a - c) * (e + f);

        Matrix c1 = p1 + p2 - p3 + p4;
        Matrix c2 = p5 + p3;
        Matrix c3 = p6 + p2;
        Matrix c4 = p5 + p1 - p6 - p7;

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

        // cout << "Result:";
        // result.printout();

        return result;
}

int printoutfloats(const std::vector<float> vals) {
        for (float i : vals) {
                std::cout << i << "  ";
        }
        std::cout << "\n";
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