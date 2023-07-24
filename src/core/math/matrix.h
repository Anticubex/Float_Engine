#include "vector2.h"
#include <cstddef>
#include <vector>

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

        /// @brief Same as operator(), but returns an *immutable* float
        float get(const size_t row, const size_t col) const;

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

        /// @brief Iterate through a 2d array/matrix
        /// @param width Width of the array/matrix
        /// @param height Height of the array/matrix
        /// @param function Function to run on each entry. Base arguments are (size_t row, size_t column). Use templating to add extra arguments. Returns an int as a code: stops execution when not = 0, as a sort of `break`;
        /// @param args Additional arguments to pass into `function`
        /// @return Last return value of `function`
        template <class... Types>
        static int iter2D(const size_t width, const size_t height, int (*function)(const size_t, const size_t, Types...), Types... args);

        //       private:
        size_t size;
        std::vector<float> values;

        void printout();

      private:
        static int naive_Iter_mul(const size_t row, const size_t col, const Matrix &A, const Matrix &B, Matrix &product);
};