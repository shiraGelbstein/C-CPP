#include "Dense.h"

/**
 * Constructs a Dense layer with the given weights, bias matrix, and
 * activation function.
 * The weights and bias matrices represent the parameters of the dense layer.
 * The activation function is a pointer to a function that applies a
 * /specific activation function to the layer output.
 * @param weights The weights matrix of the dense layer.
 * @param bias The bias matrix of the dense layer.
 * @param ActivationFunction A pointer to the activation function to be
 * applied to the layer output.
 * @throw std::length_error if the bias matrix is not a vector
 * (i.e., number of columns is not 1).
 * @throw std::length_error if the number of rows in the weights matrix
 * /does not
 * match the number of rows in the bias matrix.
 */
Dense::Dense (const Matrix &weights, const Matrix &bias, Activation_func_ptr
ActivationFunction)
{
  this->weights = weights;
  this->bias = bias;
  if (bias.get_cols () != 1)
  {
    throw std::length_error ("Activation functions gets only vectors.");
  }
  if (weights.get_rows () != bias.get_rows ())
  {
    throw std::length_error ("weights rows should be as the bias rows");
  }
  this->ActivationFunction = ActivationFunction;
}

/**
 * Applies the dense layer to the input matrix, including the weights,
 * bias, and activation function.
 * The dense layer computes the dot product of the weights and the input
 * matrix, adds the bias vector,
 * applies the activation function to the resulting matrix, and returns
 * the output matrix.
 * @param matrix The input matrix to which the dense layer is applied.
 * @return The output matrix after applying the dense layer.
 */
Matrix Dense::operator() (const Matrix &matrix) const
{
  Matrix temp = ActivationFunction ((weights * matrix) += bias);
  return temp;
}
