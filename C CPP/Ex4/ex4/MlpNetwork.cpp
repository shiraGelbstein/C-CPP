#include "MlpNetwork.h"

/**
 * Constructs an MLP network with the given weights and biases.
 * The MLP network consists of four dense layers with specific
 * activation functions.
 * @param weights An array of matrices representing the weights
 * of each dense layer.
 * @param biases An array of matrices representing the biases of
 * each dense layer.
 * @throw std::length_error if the bias matrix of any dense layer is not/git a
 * vector (i.e., number of columns is not 1).
 * @throw std::length_error if the number of rows in the weights matrix
 * of any dense layer does not match the number of rows in the bias matrix.
 */
MlpNetwork:: MlpNetwork(const Matrix weights[MLP_SIZE], const Matrix
biases[MLP_SIZE]):
layer1 (weights[0], biases[0], activation::relu),
layer2 (weights[1], biases[1], activation::relu),
layer3 (weights[2], biases[2], activation::relu),
layer4 (weights[3], biases[3], activation::softmax)
//weights{weights[0],weights[1],weights[2],weights[3]},
//biases{biases[0],biases[1],biases[2],biases[3]}
{}


/**
 * Applies the MLP network to the input matrix, which includes forward
 * /propagation
 * through the four dense layers.
 * The input matrix is vectorized, and then each dense layer is applied
 * successively.
 * The output of the final dense layer is converted to a digit structure
 * containing
 * the predicted index and probability.
 * @param matrix The input matrix to which the MLP network is applied.
 * @return A digit structure containing the predicted index and probability.
 */
digit MlpNetwork::operator()(const Matrix& matrix)
{
  Matrix img = matrix;
  img.vectorize();
  Matrix matrix1 =(layer1) (img);
  Matrix matrix2 = (layer2) (matrix1);
  Matrix matrix3 = (layer3) (matrix2);
  Matrix matrix4= (layer4) (matrix3);
  unsigned int the_index= matrix4.argmax();
  float the_probability = matrix4[(int)the_index];
  digit res= {the_index, the_probability};
  return res;
}
