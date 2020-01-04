#include <iostream>
#include "matrix.h"

Matrix operator*(double &b, const Matrix& B){
  Matrix C = Matrix(B.m,B.n);
  int idx;
  for (int i = 0; i < B.m; i++){
    for (int j = 0; j < B.n; j++) {
      idx = i + j*B.m;
      C.A[idx] = B.A[idx]*b;
    }
  }
  return C;
}

Matrix operator+(Matrix &&A, const Matrix& B ) {
  A += B;
  return std::move(A);
}

Matrix operator-(Matrix &&A, const Matrix& B ) {
    A -= B;
    return std::move(A);
}

void Matrix::print_matrix(void) const {
  for (int i = 0; i < this->m; i++){
    std::cout << "[ ";
    for (int j = 0; j <this->n; j++){
      std:: cout <<  this->A[i + j*this->m] << " ";
    }
    std::cout << "]\n";
  }
}

void Matrix::fill_matrix(void){
  for (int i = 0; i < this->m; i++){
    for (int j = 0; j <this->n; j++){
      this->A[i + j*this->m] = i + j*this->m;
    }
  }
}


//int main(){
//  Matrix M(3,4);
//  int a = 3;
//  double b = 2.1;
//  M.print_matrix();
//  M.fill_matrix();
//  std::cout << "\n";
//  M.print_matrix();
//  std::cout << "\n";
//  Matrix M2 = M + M;
//  std::cout << "\n";
//  M2(1,0) = 42;
//  M2.print_matrix();
//  M2 = M2 * a;
//  M2.print_matrix();
//  std::cout << M(0,1) << " " << M(1,1) << std::endl;
//  std::cout << M(0,1) << " " << M(0,2) << std::endl;
//
//}
