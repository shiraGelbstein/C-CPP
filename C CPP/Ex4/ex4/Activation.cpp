#include "Activation.h"

/**
 * Applies the Rectified Linear Unit (ReLU) activation function
 * /element-wise to the matrix.
 * ReLU sets negative values to zero and leaves positive values unchanged.
 * @param matrix The vector to which the activation function is applied.
 * @return A new matrix with the ReLU-applied values.
 */
Matrix activation::relu (const Matrix &matrix)
{
  Matrix new_mat (matrix);
  int len = new_mat.get_rows () * new_mat.get_cols ();
  for (int i = 0; i < len; i++)
  {
    if (new_mat[i] < 0)
    {
      new_mat[i] = 0;
    }
  }
  return new_mat;
}

/**
 * Applies the Softmax activation function to the matrix.
 * Softmax function exponent each element, normalizes the values
 * by dividing by the sum of exponent values,
 * and returns a new matrix with the softmax-applied values.
 * @param matrix The matrix to which the activation function is applied.
 * @return A new matrix with the softmax-applied values.
 */
Matrix activation::softmax (const Matrix &matrix)
{
  Matrix new_mat(matrix.get_rows(),matrix.get_cols());
  int len = matrix.get_cols() * matrix.get_rows();
  float exp_counter=0;
  for (int i = 0; i < len; i++)
  {
    new_mat[i] = std::exp(matrix[i]);
    exp_counter += new_mat[i];
  }
  if (exp_counter == 0) {
    throw std::runtime_error("division by zero");
  }
  for(int j =0; j<len; j++)
  {
    new_mat[j] *= 1/exp_counter;
  }
  return new_mat;
}

