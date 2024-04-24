#ifndef ACTIVATION_H
#define ACTIVATION_H
#include "Matrix.h"
#include <cmath>

typedef Matrix (*Activation_func_ptr)(const Matrix& matrix);

typedef Matrix (Activation_func)(const Matrix& matrix);

namespace activation
{
    Activation_func relu;
    Activation_func softmax;
}

#endif //ACTIVATION_H