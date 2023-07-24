#include <cstddef>
#include <vector2.h>
#include <vector>

namespace xmath {

using std::vector;

float fastInvSqrt(const float x);
float precise_fastInvSqrt(const float x, const size_t rounds);

class Matrix {

      public:
        /// @brief Generates a matrix of set rows and columns
        /// @param rows The number of rows of a matrix
        /// @param cols The number of columns of a matrix
        Matrix(const size_t rows, const size_t cols);

        /// @brief Gets a reference to the value at a certain row and colum
        /// @param row Row of the entry
        /// @param col Column of the entry
        /// @return A reference to the float at the entry, allowing setting
        float &operator()(const size_t row, const size_t col);

        /// @brief Same as operator(), but returns a const reference
        const float &get(const size_t row, const size_t col);

        /// @brief Add two matrices
        /// @param other Other matrix to add
        /// @return The sum of the matrices
        Matrix operator+(const Matrix &other);

        /// @brief Multiply two matrices
        /// @param other Other matrix to multiply
        /// @return The product of the matrices
        Matrix operator*(const Matrix &other);

        size_t rows;
        size_t cols;

        static int iter2D(size_t width, size_t height, int (*function)(size_t, size_t));

        //       private:
        size_t size;
        vector<float> values;
};

}; // namespace xmath
