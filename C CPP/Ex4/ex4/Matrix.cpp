#include "Matrix.h"
#include <algorithm>
#include <iostream>
#include <cmath>

//Numbers
#define FILL_WITH_ZERO 0.0f
#define PRETTY_PRINT_VALUE 0.1
#define DEFAULT_INITIALIZING_SIZE 1
#define INDEX_MIN_VALUE 0

//Errors:
#define INVALID_CONSTRUCTOR_ARGS_ERROR "matrix dimension cannot be les than 1"
#define DOT_ERROR "matrix dimension should be the same for dot method"
#define ADD_OPERATOR_ERROR "matrix dimension should be the same for + operator"
#define ADD_EQUALITY_OPERATOR_ERROR "matrix dimension should be the same \
for + operator"
#define MULT_OPERATOR_ERROR "left matrix cols should be the same as right \
rows when using * operator"
#define INDEX_OUT_OF_RANGE_ERROR "index out of the matrix range"
#define PRETTY_PRINT_FORMAT "**"
#define NOT_ENOUGH_ARGS_TO_IS_ERROR "expected numbers the size of the matrix"
#define FAILED_TO_READ_OS_ERROR "could not load output stream"

//Using:
using std::cout;
using std::endl;


//Methods:
/**
 * Constructs a Matrix object with the specified number of rows and columns.
 * The matrix is initialized with all elements set to zero.
 * Throws an exception if the number of rows or columns is less than 1.
 * @param rows The number of rows in the matrix.
 * @param cols The number of columns in the matrix.
 * @throw std::length_error if rows or cols is less than 1.
 */
Matrix::Matrix (int rows, int cols) :
    matrix_d{DEFAULT_INITIALIZING_SIZE, DEFAULT_INITIALIZING_SIZE}
{
  if (rows <= INDEX_MIN_VALUE or cols <= INDEX_MIN_VALUE)
  {
    throw std::length_error (INVALID_CONSTRUCTOR_ARGS_ERROR);
  }
  mat = new float[rows * cols];
  std::fill (mat, mat + rows * cols, FILL_WITH_ZERO);
  struct matrix_dims matrix_dim{rows, cols};
  matrix_d = matrix_dim;
}

/**
 * Copy constructor. Creates a new Matrix object that is a copy of the
 * /provided matrix.
 * @param matrix The matrix to be copied.
 */
Matrix::Matrix (const Matrix &matrix) : matrix_d{matrix.matrix_d.cols, matrix
    .matrix_d.cols}
{
  this->matrix_d = matrix.matrix_d;
  mat = new float[(matrix.matrix_d.cols) * (matrix.matrix_d.rows)];
  std::copy (matrix.mat, matrix.mat +
                         (matrix.matrix_d.cols * matrix.matrix_d.rows), mat);
}

/**
 * Destructor. Releases the memory occupied by the matrix.
 */
Matrix::~Matrix ()
{
  delete[] mat;
}

void swap_int (int &int1, int &int2)
{
  int temp = int1;
  int1 = int2;
  int2 = temp;
}

void swap_lists (float *&list1, float *&list2)
{
  float *temp = list1;
  list1 = list2;
  list2 = temp;
}

void swap_mat (Matrix &mat1, Matrix &mat2) //changed in 12:29
{
  swap_int (mat1.matrix_d.rows, mat2.matrix_d.rows);
  swap_int (mat1.matrix_d.cols, mat2.matrix_d.cols);
  swap_lists (mat1.mat, mat2.mat);
}

/**
 * Transposes the matrix, swapping its rows and columns.
 * @return Reference to the transposed matrix.
 */
Matrix &Matrix::transpose ()
{
  int len_mat = (matrix_d.cols) * (matrix_d.rows);
  auto *copy = new float[len_mat];
  std::copy (mat, mat + len_mat, copy);
  for (int i = 0; i < matrix_d.rows; i++)
  {
    for (int j = 0; j < matrix_d.cols; j++)
    {
      mat[j * matrix_d.rows + i] = copy[i * matrix_d.cols + j];
    }
  }
  swap_int (matrix_d.rows, matrix_d.cols);
  delete[] copy;
  return *this;
}

/**
 * Converts the matrix into a column vector by stacking all its elements
 * vertically.
 * The number of rows is set to the product of the original number of rows
 * and columns,
 * and the number of columns is set to 1.
 * @return Reference to the vectorized matrix.
 */
Matrix &Matrix::vectorize ()
{
  matrix_d.rows = matrix_d.rows * matrix_d.cols;
  matrix_d.cols = 1;
  return *this;
}

/**
 * Prints the matrix in plain format, row by row, with space-separated
 * elements.
 */
void Matrix::plain_print () const
{
  for (int i = 0; i < matrix_d.rows; i++)
  {
    for (int j = 0; j < matrix_d.cols; j++)
    {
      cout << mat[i * matrix_d.cols + j] << " ";
    }
    cout << endl;
  }
}

/**
 * Performs element-wise multiplication between two matrices of the same
 * dimensions.
 * Returns a new matrix containing the result.
 * Throws an exception if the matrices have different dimensions.
 * @param matrix The matrix to be multiplied element-wise.
 * @return A new matrix with the element-wise multiplication result.
 * @throw std::length_error if matrix dimensions do not match.
 */
Matrix Matrix::dot (const Matrix &matrix) const
{
  if (this->matrix_d.rows != matrix.matrix_d.rows ||
      this->matrix_d.cols != matrix.matrix_d.cols)
  {
    throw std::length_error (DOT_ERROR);
  }
  Matrix new_mat (*this);
  int len_mat = (matrix_d.cols) * (matrix_d.rows);
  int j = 0, i = 0;
  for (int p = 0; p < len_mat; p++)
  {
    new_mat.mat[i * matrix_d.cols + j] *= (matrix.mat[i * matrix_d.cols + j]);
    j++;
    if (j >= matrix_d.cols)
    {
      j = 0;
      i++;
    }
  }
  return new_mat;
}

/**
* Calculates the Frobenius norm of the matrix.
* @return The Euclidean norm of the matrix.
*/
float Matrix::norm () const
{
  int len_mat = (matrix_d.cols) * (matrix_d.rows);
  float summing = 0;
  for (int p = 0; p < len_mat; p++)
  {
    summing += (mat[p] * mat[p]);
  }
  return std::sqrt (summing);
}

/**
* Returns the index of the element with the maximum value in the matrix.
* If multiple elements have the same maximum value, the index of
 * the first occurrence is returned.
* @return The index of the maximum element in the matrix.
*/
int Matrix::argmax () const
{
  int len_mat = (matrix_d.cols) * (matrix_d.rows);
  int biggest = 0;
  for (int p = 0; p < len_mat; p++)
  {
    if (mat[p] > mat[biggest])
    {
      biggest = p;
    }
  }
  return biggest;
}

/**
 * Calculates the sum of all elements in the matrix.
 * @return The sum of all elements in the matrix.
 */
float Matrix::sum () const
{
  int len_mat = (matrix_d.cols) * (matrix_d.rows);
  float sum = 0;
  for (int p = 0; p < len_mat; p++)
  {
    sum += mat[p];
  }
  return sum;
}


//Operators:

/**
 * Copy assignment operator. Assigns the values from another matrix
 * to this matrix.
 * @param b The matrix to be assigned.
 * @return Reference to the assigned matrix.
 */
Matrix &Matrix::operator= (const Matrix &b)
{
  Matrix temp (b);
  swap_mat (*this, temp);
  return *this;
}

/**
 * Adds two matrices of the same dimensions and returns a new matrix with
 * the addition result.
 * Throws an exception if the matrices have different dimensions.
 * @param b The matrix to be added.
 * @return A new matrix with the element-wise addition result.
 * @throw std::length_error if matrix dimensions do not match.
 */
Matrix Matrix::operator+ (const Matrix &b) const
{
  if (this->matrix_d.rows != b.matrix_d.rows ||
      this->matrix_d.cols != b.matrix_d.cols)
  {
    throw std::length_error (ADD_OPERATOR_ERROR);
  }
  Matrix new_mat = *this; //in the future will be good.
  int len_mat = (matrix_d.cols) * (matrix_d.rows);
  for (int p = 0; p < len_mat; p++)
  {
    new_mat.mat[p] += b.mat[p];
  }
  return new_mat;
}

/**
 * Adds another matrix to this matrix element-wise.
 * Throws an exception if the matrices have different dimensions.
 * @param b The matrix to be added.
 * @return Reference to the modified matrix after addition.
 * @throw std::length_error if matrix dimensions do not match.
 */
Matrix &Matrix::operator+= (const Matrix &b)
{
  if (this->matrix_d.rows != b.matrix_d.rows ||
      this->matrix_d.cols != b.matrix_d.cols)
  {
    throw std::length_error (ADD_EQUALITY_OPERATOR_ERROR);
  }
  int len_mat = (matrix_d.cols) * (matrix_d.rows);
  for (int p = 0; p < len_mat; p++)
  {
    mat[p] += b.mat[p];
  }
  return *this;
}

/**
 * Multiplies two matrices and returns a new matrix with the matrix product.
 * Throws an exception if the number of columns in the left
 * matrix is not equal to the number of rows in the right matrix.
 * @param b The matrix to be multiplied.
 * @return A new matrix with the matrix product.
 * @throw std::length_error if matrix dimensions do not match.
 */
Matrix Matrix::operator* (const Matrix &b) const
{
  if (this->matrix_d.cols != b.matrix_d.rows)
  {
    throw std::length_error (MULT_OPERATOR_ERROR);
  }

  Matrix new_mat (this->matrix_d.rows, b.matrix_d.cols);

  for (int i = 0; i < b.matrix_d.cols; i++)
  {
    for (int j = 0; j < this->matrix_d.rows; j++)
    {
      float sum = 0;

      for (int k = 0; k < this->matrix_d.cols; k++)
      {
        sum += ((this->mat[j * this->matrix_d.cols + k]) *
                (b.mat[k * b.matrix_d.cols + i]));
      }

      new_mat.mat[j * b.matrix_d.cols + i] = sum;
    }
  }
  return new_mat;
}

/**
 * Multiplies a scalar value with a matrix and returns a new matrix with the
 * scaled values.
 * @param scalar The scalar value to be multiplied.
 * @param matrix The matrix to be scaled.
 * @return A new matrix with the scaled values.
 */
Matrix operator* (float scalar, const Matrix &matrix)
{
  Matrix new_mat (matrix);
  int len_mat = (matrix.matrix_d.cols) * (matrix.matrix_d.rows);
  for (int p = 0; p < len_mat; p++)
  {
    new_mat.mat[p] *= scalar;
  }
  return new_mat;
}

/**
 * Multiplies a matrix with a scalar value and returns a new matrix with
 * the scaled values.
 * @param scalar The scalar value to be multiplied.
 * @return A new matrix with the scaled values.
 */
Matrix Matrix::operator* (float scalar) const
{
  Matrix new_mat (matrix_d.rows, matrix_d.cols);
  int len_mat = (matrix_d.cols) * (matrix_d.rows);
  for (int p = 0; p < len_mat; p++)
  {
    new_mat.mat[p] = mat[p] * scalar;
  }
  return new_mat;
}

/**
 * Retrieves the value at the specified row and column in the matrix.
 * Throws an exception if the row or column index is out of range.
 * @param row The row index.
 * @param col The column index.
 * @return The value at the specified row and column.
 * @throw std::out_of_range if the row or column index is out of range.
 */
float Matrix::operator() (int row, int col) const
{
  if ((row >= matrix_d.rows) || (col >= matrix_d.cols) || (row < 0)
      || (col < 0))
  {
    throw std::out_of_range (INDEX_OUT_OF_RANGE_ERROR);
  }
  return mat[row * matrix_d.cols + col];
}

/**
 * Retrieves the reference to the value at the specified row and column in
 * the matrix.
 * Throws an exception if the row or column index is out of range.
 * @param row The row index.
 * @param col The column index.
 * @return Reference to the value at the specified row and column.
 * @throw std::out_of_range if the row or column index is out of range.
 */
float &Matrix::operator() (int row, int col)
{
  if ((row >= matrix_d.rows) || (col >= matrix_d.cols) || (col < 0)
      || (row < 0))
  {
    throw std::out_of_range (INDEX_OUT_OF_RANGE_ERROR);
  }
  return mat[row * matrix_d.cols + col];
}

/**
 * Retrieves the value at the specified index in the matrix.
 * Throws an exception if the index is out of range.
 * @param index The index of the mat list.
 * @return The value at the specified index.
 * @throw std::out_of_range if the index is out of range.
 */
float Matrix::operator[] (int index) const
{
  if ((index >= matrix_d.rows * matrix_d.cols) || (index < 0))
  {
    throw std::out_of_range (INDEX_OUT_OF_RANGE_ERROR);
  }
  return mat[index];
}

/**
 * Retrieves the reference to the value at the specified index in the matrix.
 * Throws an exception if the index is out of range.
 * @param index The index of the mat list.
 * @return Reference to the value at the specified index.
 * @throw std::out_of_range if the index is out of range.
 */
float &Matrix::operator[] (int index)
{
  if ((index >= matrix_d.rows * matrix_d.cols) || (index < 0))
  {
    throw std::out_of_range (INDEX_OUT_OF_RANGE_ERROR);
  }
  return mat[index];
}

/**
 * Prints the matrix to the specified output stream in the "pretty
 * print" format.
 * @param s The output stream.
 * @param matrix The matrix to be printed.
 * @return Reference to the output stream.
 */
std::ostream &operator<< (std::ostream &s, const Matrix &matrix)
{
  int len = matrix.get_cols () * matrix.get_rows ();
  int row_counter = 0;
  for (int i = 0; i < len; i++)
  {
    if (matrix[i] > PRETTY_PRINT_VALUE)
    {
      s << PRETTY_PRINT_FORMAT;
    }
    else
    {
      s << "  ";
    }
    row_counter++;
    if (row_counter >= matrix.get_rows ())
    {
      s << "\n";
      row_counter = 0;
    }
  }
  return s;
}

/**
 * Reads the matrix from the specified input stream.
 * Throws an exception if there is an error while reading the matrix.
 * @param s The input stream.
 * @param matrix The matrix to be read.
 * @return Reference to the input stream.
 * @throw std::runtime_error if there is an error while reading the matrix
 * and if the istream has not enough numbers to the matrix.
 */
std::istream &operator>> (std::istream &s, Matrix &matrix)
{
  int counter = 0;
  while (s.good () && counter < matrix.get_rows () * matrix.get_cols ())
  {
    s.read ((char *) &matrix[counter], sizeof (float));
    counter++;
  }
  if (counter < matrix.get_rows () * matrix.get_cols ())
  {
    throw std::runtime_error (NOT_ENOUGH_ARGS_TO_IS_ERROR);
  }
  if (s.fail ())
  {
    throw std::runtime_error (FAILED_TO_READ_OS_ERROR);
  }
  return s;
}


