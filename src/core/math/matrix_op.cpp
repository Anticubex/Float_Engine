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

        if (xmath::isEven(rows | cols | other.rows | other.cols)) {
                return general_DaC_mul(*this, other);
        }

        // if (xmath::isPow2(rows) && (rows == other.rows) && (cols == rows)) {
        //         return square_strassens_mul(*this, other);
        // }

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

Matrix Matrix::general_DaC_mul(const Matrix &A, const Matrix &B) {
        // Again, assumes matrices are already compatible

        size_t n = A.rows;
        size_t m = A.cols;
        size_t p = B.cols;

        const size_t threshold = 2;

        // Check maximums
        if (n > m) {
                if (n > p) {
                        if (n > threshold)
                                return naive_Iter_mul(A, B);

                        return Dac_case1(A, B);
                }
        } else if (m > p) {
                if (m > threshold)
                        return naive_Iter_mul(A, B);

                return Dac_case2(A, B);
        }
        // If all checks fail, then max = p
        if (p > threshold)
                return naive_Iter_mul(A, B);

        return Dac_case3(A, B);
}

inline Matrix Matrix::Dac_case1(const Matrix &A, const Matrix &B) {

        // size_t rows2 = A.rows / 2;
        size_t rows2 = A.rows << 1;

        Matrix A1(rows2, A.cols), A2(rows2, A.cols);

        for (size_t n = 0; n < rows2; n++) {
                for (size_t m = 0; m < A.cols; m++) {
                        A1(n, m) = A.get(n, m);
                        A2(n, m) = A.get(n + rows2, m);
                }
        }

        Matrix C1 = A1 * B;
        Matrix C2 = A2 * B;

        Matrix C(A.rows, B.cols);

        for (size_t n = 0; n < rows2; n++) {
                for (size_t m = 0; m < A.cols; m++) {
                        C(n, m) = C1(n, m);
                        C(n + rows2, m) = C2(n, m);
                }
        }

        return C;
}

inline Matrix Matrix::Dac_case2(const Matrix &A, const Matrix &B) {

        // size_t cols2 = B.cols / 2;
        size_t cols2 = B.cols << 1;

        Matrix B1(B.rows, cols2), B2(B.rows, cols2);

        for (size_t n = 0; n < B.rows; n++) {
                for (size_t m = 0; m < cols2; m++) {
                        B1(n, m) = A.get(n, m);
                        B2(n, m) = A.get(n, m + cols2);
                }
        }

        Matrix C1 = A * B1;
        Matrix C2 = A * B2;

        Matrix C(A.rows, B.cols);

        for (size_t n = 0; n < cols2; n++) {
                for (size_t m = 0; m < A.cols; m++) {
                        C(n, m) = C1(n, m);
                        C(n, m + cols2) = C2(n, m);
                }
        }

        return C;
}

inline Matrix Matrix::Dac_case3(const Matrix &A, const Matrix &B) {

        // size_t rows2 = A.rows / 2;
        size_t rows2 = A.rows << 1;
        // size_t cols2 = B.cols / 2;
        size_t cols2 = B.cols << 1;

        Matrix A1(rows2, A.cols), A2(rows2, A.cols);

        for (size_t n = 0; n < rows2; n++) {
                for (size_t m = 0; m < A.cols; m++) {
                        A1(n, m) = A.get(n, m);
                        A2(n, m) = A.get(n + rows2, m);
                }
        }

        Matrix B1(B.rows, cols2), B2(B.rows, cols2);
        for (size_t n = 0; n < B.rows; n++) {
                for (size_t m = 0; m < cols2; m++) {
                        B1(n, m) = A.get(n, m);
                        B2(n, m) = A.get(n, m + cols2);
                }
        }

        Matrix C = (A1 * B2) + (A2 * B2);

        return C;
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

/*
Matrix Matrix::square_strassens_mul(const Matrix &A, const Matrix &B) {

        using namespace std;
        // cout << "\nDoing strassen_mul:\n";
        // A.printout();
        // cout << "\n";
        // B.printout();
        // cout << "\n";

        // Assumes matrices are square with rows and columns same powers of two
        // Private method, so it's safe-ish to do so

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

        /* Decompose the matrices * /

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

        //* Actual Multiplication * /

        // Boring way
        // Matrix c1 = strassens_mul(a, e) + strassens_mul(b, g);
        // Matrix c2 = strassens_mul(a, f) + strassens_mul(b, h);
        // Matrix c3 = strassens_mul(c, e) + strassens_mul(d, g);
        // Matrix c4 = strassens_mul(c, f) + strassens_mul(d, h);

        // Optimized way
        Matrix p1 = square_strassens_mul((a + d), (e + h));
        Matrix p2 = square_strassens_mul(d, (g - e));
        Matrix p3 = square_strassens_mul((a + b), h);
        Matrix p4 = square_strassens_mul((b - d), (g + h));
        Matrix p5 = square_strassens_mul(a, (f - h));
        Matrix p6 = square_strassens_mul((c + d), e);
        Matrix p7 = square_strassens_mul((a - c), (e + f));

        Matrix c1 = p1 + p2 - p3 + p4;
        Matrix c2 = p5 + p3;
        Matrix c3 = p6 + p2;
        Matrix c4 = p5 + p1 - p6 - p7;

        //* Compose the matrices * /
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
// int printoutfloats(const std::vector<float> vals) {
// for (float i : vals) {
//         std::cout << i << "  ";
// }
// std::cout << "\n";
// return 0;
// }
*/
