#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

// Insert Dense class here...
class Dense{
 public:
  Dense(const Matrix &weights,const Matrix &bias, Activation_func_ptr
  ActivationFunction);
  Matrix get_weights() const{return weights;}
  Matrix get_bias() const {return bias;}
  Activation_func_ptr get_activation() const {return ActivationFunction;}
  Matrix operator()(const Matrix& matrix) const;
 private:
  Matrix weights;
  Matrix bias;
  Activation_func_ptr ActivationFunction;
};


#endif //DENSE_H
