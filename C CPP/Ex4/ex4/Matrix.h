// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#define DEFAULT_MAT_SIZE = 1
/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
	int rows, cols;
} matrix_dims;

// Insert Matrix class here...
class Matrix{
 private:
  float* mat;
  struct matrix_dims matrix_d;
  friend void swap_mat(Matrix& mat1, Matrix& mat2);
 public:
  Matrix(int rows,int cols);
  Matrix(): Matrix (1,1){};
  Matrix(const Matrix& matrix);
  ~Matrix();
  int get_rows() const {return matrix_d.rows;}
  int get_cols() const {return matrix_d.cols;}
  Matrix& transpose();
  Matrix& vectorize();
  void plain_print() const;
  Matrix dot(const Matrix& matrix) const;
  float norm() const;
  int argmax() const;
  float sum() const ;
  Matrix operator+(const Matrix& b) const;
  Matrix& operator+=(const Matrix& b);
  Matrix& operator=(const Matrix& b);
  Matrix operator *(const Matrix& b) const;
  friend Matrix operator*(float scalar, const Matrix& matrix);
  Matrix operator*(float scalar) const;
  float operator()(int row, int col) const;
  float& operator()(int row, int col);
  float operator[](int index) const;
  float& operator[](int index);
  friend std::ostream& operator<<(std::ostream &s,const Matrix& matrix);
  friend std::istream& operator>>(std::istream &s,Matrix& matrix);
};
#endif //MATRIX_H